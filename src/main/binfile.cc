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


#include <errno.h>
#include <string.h>

#include <mwlogger.h>

#include "binfile.h"

BinFile::BinFile()
    : _file_handle(NULL) {
}

BinFile::BinFile(const MWString &file_name)
    : _file_handle(NULL)
    , _file_name(file_name) {
}

BinFile::~BinFile() {
    if (_file_handle != NULL) {
        Close();
    }
}

void BinFile::SetFileName(const MWString &file_name) {
    _file_name = file_name;
}

const MWString &BinFile::GetFileName() const {
    return _file_name;
}

bool BinFile::Open(const MWString &mode) {
    if (_file_handle != NULL) {
        Close();
        LOGWARN("_file_handle != NULL, closed eventually");
    }

    _file_handle = fopen(_file_name.c_str(), mode.c_str());
    if (_file_handle == NULL) {
        LOGERROR("fopen(" << _file_name << 
                 ", \"" << mode << 
                 "\") error, " << strerror(errno) << 
                 ", return false");
        return false;
    }

    return true;
}

bool BinFile::OpenForRead() {
    return Open("r");
}

bool BinFile::OpenForRead(const MWString &file_name) {
    _file_name = file_name;
    return OpenForRead();
}

bool BinFile::OpenForWrite() {
    return Open("w");
}

bool BinFile::OpenForWrite(const MWString &file_name) {
    _file_name = file_name;
    return OpenForWrite();
}

bool BinFile::Close() {
    if (_file_handle != NULL) {
        if (fclose(_file_handle) != 0) {
            LOGERROR("fclose(_file_handle) error " << strerror(errno));
            return false;
        }

        _file_handle = NULL;
    } else {
        LOGWARN("fclose(NULL) means nothing, but true returned");
    }

    return true;
}

void BinFile::SetOff(size_t off) const {
    fseek(_file_handle, off, SEEK_SET);
}

int BinFile::Read(size_t char_arr_len, char *char_arr) const {
    if (_file_handle == NULL) {
        LOGERROR("_file_handle == NULL, can not read, file_name = " << _file_name << 
                 ", empty vector returned " << strerror(errno));
        return -1;
    }

    return fread(char_arr, sizeof(char), char_arr_len, _file_handle);
}

std::vector<char> BinFile::Read(size_t len) const {
    std::vector<char> ret;
    if (_file_handle == NULL) {
        LOGERROR("_file_handle == NULL, can not read, file_name = " << _file_name << 
                 ", empty vector returned " << strerror(errno));
        return ret;
    }

    size_t char_arr_len(len == 0 ? MW_STL_BIN_FILE_ONE_TIME_READ_LEN : len),
           content_len, i;
    char char_arr[char_arr_len];

    do {
        content_len = fread(char_arr, sizeof(char), char_arr_len, _file_handle);

        for (i = 0; i < content_len; ++ i) {
            ret.push_back(char_arr[i]);
        }
    } while (content_len != 0 && len == 0);

    return ret;
}

std::vector<char> BinFile::Read(const MWString &file_name, size_t len) {
    _file_name = file_name;
    return Read(len);
}

bool BinFile::Write(const std::vector<char> &vector_char) const {
    if (_file_handle == NULL) {
        LOGERROR("_file_handle == NULL, can not Write, file_name = " << _file_name << 
                 ", " << strerror(errno) << 
                 ", false returned");
        return false;
    }

    size_t write_len;
    for (size_t i = 0; i < vector_char.size(); ++ i) {
        write_len = fwrite(&(vector_char[i]), sizeof(char), 1, _file_handle);
        if (write_len < 1) {
            LOGERROR("write file " << _file_name << 
                     " error, " << strerror(errno) << 
                     ", return false");
            return false;
        }
    }

    return true;
}

bool BinFile::Write(const MWString &file_name,
                    const std::vector<char> &vector_char) {
    _file_name = file_name;
    return Write(vector_char);
}
