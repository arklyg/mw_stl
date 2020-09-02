// Micro Wave Standard Template Library
// Copyright (c) 2015-2020, Ark Lee
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// You must obey the GNU General Public License in all respects for
// all of the code used.  If you modify file(s) with this exception, 
// you may extend this exception to your version of the file(s), but 
// you are not obligated to do so.  If you do not wish to do so, delete 
// this exception statement from your version. If you delete this exception 
// statement from all source files in the program, then also delete it here.
//
// Contact:  Ark Lee <arklee@houduan.online>
//           Beijing, China


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <mwlogger.h>

#include "csvfile.h"


char CSVFile::__buf[CSVFILE_BUF_LEN * 2];

MWString CSVFile::__big_str;

int CSVFile::Open(const MWString &csv_name, int mode) {
    _csv_name = csv_name;
    if (mode & O_CREAT) {
        _fd = ::open(csv_name.c_str(), mode, S_IRUSR | S_IWUSR);
    } else {
        _fd = ::open(csv_name.c_str(), mode);
    }

    if (_fd < 0) {
        LOGERROR("fopen file: " << csv_name << 
                 " error, " << strerror(errno));
        return -1;
    }

    _off = 0;
    _end = 0;
    _off_in_file = 0;
    _to_end = false;

    return 0;
}

int64_t CSVFile::GetLine() {
    int i;
    // 从内存中扫一行
    for (i = _off; i < _end; ++i) {
        // 如果扫到了，返回位置
        if (__buf[i] == '\n') {
            return ++i;
        }
    }

    if (_to_end) {
        return -1;
    }

    int ret = ::read(_fd, __buf + _end, CSVFILE_BUF_LEN - _end);
    if (ret < 0) {
        LOGERROR("read file " << _csv_name << 
                 " error, " << strerror(errno));
        return -2;
    }
    _to_end = ret < CSVFILE_BUF_LEN - _end;

    _end += ret;

    for ( ; i < _end; ++i) {
        if (__buf[i] == '\n') {
            return ++i;
        }
    }

    if (_to_end) {
        return -1;
    }

    ret = ::read(_fd, __buf, _off);
    if (ret < 0) {
        LOGERROR("read file " << _csv_name << 
                 " error, " << strerror(errno));
        return -2;
    }
    _to_end = ret < _off;

    _end = ret;

    for (i = 0; i < _end; ++i) {
        if (__buf[i] == '\n') {
            return ++i;
        }
    }

    if (_to_end) {
        return -1;
    }

    LOGERROR(CSVFILE_BUF_LEN << " too short for csv file: " << _csv_name);
    return -2;
}

// -1, to the end; -2 error; >= 0 normal read
int64_t CSVFile::NextLine(std::vector<MWString> &segs, MWString *line) {
    int ret = GetLine();
    if (ret < 0) {
        return ret;
    }

    uint32_t len;
    if (ret <= _off) {
        memcpy(__buf + CSVFILE_BUF_LEN, __buf, ret);
        len = ret + CSVFILE_BUF_LEN - _off;
    } else {
        len = ret - _off;
    }

    ParseCSV(__buf + _off, len, segs);
    if (line != NULL) {
        char line_ch[len + 1];
        memcpy(line_ch, __buf + _off, len);
        line_ch[len] = '\0';
        *line = line_ch;
    }

    _off = (_off + len) % CSVFILE_BUF_LEN;
    if (_off == 0) {
        _end = 0;
    }
    _off_in_file += len;

    return _off_in_file;
}

int CSVFile::WriteLine(std::vector<MWString> &segs) {
    __big_str = "";
    GetCSVLine(segs, __big_str);
    ssize_t ret = write(_fd, __big_str.c_str(), __big_str.length());
    if (ret < (ssize_t) __big_str.length()) {
        LOGERROR("write to csv file error, " << strerror(errno));
        return -1;
    }

    return ret;
}

int CSVFile::ParseCSV(const char *line_ch, uint32_t len, std::vector<MWString> &res,
                       uint32_t start) {
    int state = 0;
    char ch;
    MWString cur = "";
    size_t res_original_size = res.size(), res_size_cnt = 0;

    len += start;
    for (uint32_t i = start; i < len; ++i) {
        ch = line_ch[i];
        if (ch == '\n') {
            break;
        }
        switch (state) {
        case 0:
            if (ch == '"') {
                state = 1;
            } else if (ch == ',') {
                if (res_size_cnt < res_original_size) {
                    res[res_size_cnt] = cur;
                    ++res_size_cnt;
                } else {
                    res.push_back(cur.c_str());
                }
                cur = "";
                state = 0;
            } else {
                cur += ch;
                state = 2;
            }
            break;
        case 1:
            if (ch == '"') {
                state = 3;
            } else if (ch == ',') {
                state = 1;
                cur += ch;
            } else {
                state = 1;
                cur += ch;
            }
            break;
        case 2:
            if (ch == '"') {
                state = 2;
                cur += ch;
            } else if (ch == ',') {
                state = 0;
                if (res_size_cnt < res_original_size) {
                    res[res_size_cnt] = cur;
                    ++res_size_cnt;
                } else {
                    res.push_back(cur);
                }
                cur = "";
            } else {
                state = 2;
                cur += ch;
            }
            break;
        case 3:
            if (ch == '"') {
                state = 1;
                cur += ch;
            } else if (ch == ',') {
                state = 0;
                if (res_size_cnt < res_original_size) {
                    res[res_size_cnt] = cur;
                    ++res_size_cnt;
                } else {
                    res.push_back(cur);
                }
                cur = "";
            } else {
                return -1;
            }
            break;
        default:
            return -1;
        }
    }

    if (res_size_cnt < res_original_size) {
        res[res_size_cnt] = cur;
        ++res_size_cnt;
        for (size_t j = 0; j < res_original_size - res_size_cnt; ++j) {
            res.pop_back();
        }
    } else {
        res.push_back(cur);
    }

    return res_size_cnt;
}

int CSVFile::GetCSVLine(std::vector<MWString> &segs, std::string &ret, bool next_line) {
    uint32_t seg_len;
    size_t count = 0, len = segs.size();
    for (auto iter = segs.begin(); iter != segs.end(); ++iter) {
        const char *seg = (*iter).c_str();
        seg_len = (*iter).length();
        ret += "\"";
        for (uint32_t i = 0; i < seg_len; ++i) {
            if (seg[i] == '"') {
                ret += "\"";
            }
            ret += seg[i];
        }
        ret += "\"";
        if (++count < len) {
            ret += ",";
        }
    }

    if (next_line) {
        ret += "\n";
    }

    return 0;
}
