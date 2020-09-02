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


#include <iostream>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include <mwglobal.h>
#include <mwlogger.h>

#include "mwconfig.h"

int MWConfig::Parse(char *line) {
    int len = strlen(line);
    while (len != 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
        len = strlen(line);
    }
    if (line[0] == '\0' || line[0] == '\n' || line[0] == '#' || line[0] == EOF) {
        return 0;
    }

    char *pos = strstr(line, CONF_KEY_VAL_SEPER);
    if (pos == NULL) {
        LOGERROR("sepper not found in conf");
        return -1;
    }

    char key[LINE_LEN], val[LINE_LEN];
    strncpy(key, line, pos - line);
    key[pos - line] = '\0';

    strncpy(val, pos + 1, line + len - pos - 1);
    val[line + len - pos - 1] = '\0';

    return Insert(key, val);
}

int MWConfig::Insert(const std::string &key, const std::string &val) {
    std::map<std::string, ConfVal>::iterator iter = _key_vals.find(key);
    if (iter != _key_vals.end()) {
        LOGWARN("key:" << key <<
                " has two or more values, latter val ignored");
        return 0;
    }

    _key_vals.insert(std::pair<std::string, ConfVal>(key, ConfVal(val, _num++)));

    return 0;
}

void MWConfig::Clear() {
    _num = 0;
    _key_vals.clear();
}

MWConfig::MWConfig(const std::string &file_name)
    : _file_name(file_name)
    , _num(0) {
}

int MWConfig::Load() {
    FILE *conf = fopen(_file_name.c_str(), "r");
    if (conf == NULL) {
        std::cout << "open " << _file_name << " error, " << strerror(errno) << std::endl;
        return -1;
    }
    int ret = ParseLines(conf);
    if (fclose(conf) != 0 || ret < 0) {
        return -2;
    }
    return 0;
}

int MWConfig::ParseLines(FILE *conf) {
    char buf[LINE_LEN + 1], *ret;
    while (true) {
        errno = 0;
        ret = fgets(buf, LINE_LEN, conf);
        if (errno != 0) {
            std::cout << "fgets " << _file_name << " error, " << strerror(errno) << std::endl;
            return -2;
        }
        if (ret == NULL) {
            break;
        }
        if (Parse(ret) < 0) {
            std::cout << "parse line: " << ret << " error" << std::endl;
            return -3;
        }
    }
    return 0;
}

std::set<std::string> MWConfig::GetKeys() const {
    std::set<std::string> keys;
    for (auto iter = _key_vals.begin();
            iter != _key_vals.end(); ++iter) {
        keys.insert(iter->first);
    }
    return keys;
}

int MWConfig::GetKeysNum() const {
    return _key_vals.size();
}

int MWConfig::GetIntVal(const std::string &key) const {
    std::string val = GetStringVal(key);
    if (val == "") {
        return 0;
    }
    return atoi(val.c_str());
}

std::string MWConfig::GetStringVal(const std::string &key) const {
    auto iter = _key_vals.find(key);
    if (iter == _key_vals.end()) {
        return "";
    }
    return iter->second._val;
}
