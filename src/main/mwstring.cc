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


#include <stdlib.h>
#include <sstream>
#include <string.h>
#include <iconv.h>

#include "mwstring.h"

MWString::MWString()
    : BaseString() {
}

MWString::~MWString() {
}

MWString::MWString(const MWString &s)
    : BaseString(s) {
}

MWString::MWString(const BaseString &s)
    : BaseString(s) {
}

MWString::MWString(size_t length, const char &ch)
    : BaseString(length, ch) {
}

MWString::MWString(const char *str)
    : BaseString(str) {
}

MWString::MWString(const char *str, size_t length)
    : BaseString(str, length) {
}

MWString::MWString(const MWString &str, size_t index, size_t length)
    : BaseString(str, index, length) {
}

MWString::MWString(const size_t num)
    : BaseString() {
    *this = num;
}

const MWString &MWString::operator =(size_t num) {
    std::stringstream s;
    s << num;
    s >> *this;
    return *this;
}

const MWString &MWString::operator =(double num) {
    std::stringstream s;
    s << num;
    s >> *this;
    return *this;
}

MWString MWString::Replace(const MWString &wildcard,
                           const std::vector<MWString> &replacement_vector) const {
    if (replacement_vector.empty()) {
        return *this;
    }

    std::vector<MWString> segs = Split(wildcard);
    if (segs.empty()) {
        return *this;
    }

    MWString ret = segs[0];
    for (size_t i = 1; i < segs.size() && i - 1 < replacement_vector.size(); ++ i) {
        ret += (replacement_vector[i - 1] + segs[i]);
    }

    return ret;
}

MWString MWString::Substr(size_t index, size_t num) {
    return static_cast<MWString>(BaseString::substr(index, num));
}

MWString MWString::Encode(MWSTRING_TYPE_ENCODING encoding_from,
                          MWSTRING_TYPE_ENCODING encoding_to) {
    iconv_t cd(iconv_open(encoding_to, encoding_from));
    if (cd == reinterpret_cast<iconv_t>(- 1)) {
        return "";
    }

    size_t src_len(this->length()), dst_len(6 * this->length());
    char src[src_len + 1], *src_pt(reinterpret_cast<char*>(&src));
    strcpy(src, this->c_str());
    char dst[dst_len + 1], *dst_pt(reinterpret_cast<char*>(&dst));
    memset(dst, 0, dst_len + 1);

    iconv(cd, &src_pt, &src_len, &dst_pt, &dst_len);
    return MWString(dst);
}

std::vector<MWString> MWString::Split(const char *str) const {
    MWString s(str);
    return Split(s);
}

std::vector<MWString> MWString::Split(const MWString &s) const {
    std::vector<MWString> ret;
    MWString str(*this);
    size_t start_pos(0), end_pos;
    while (true) {
        end_pos = this->find(s, start_pos);
        if (end_pos != BaseString::npos) {
            ret.push_back(str.substr(start_pos, end_pos - start_pos));
            start_pos = end_pos + s.length();
        } else {
            break;
        }
    }
    ret.push_back(str.substr(start_pos));

    return ret;
}

void MWString::AddSequence() {
    return;
}

std::vector<char> MWString::Serialize() {
    std::vector<char> ret;
    const size_t size = this->size();
    const size_t size_t_size = sizeof(size_t);
    char char_arr[size_t_size + size];
    memcpy(char_arr, &size, size_t_size);

    size_t i;
    for (i = 0; i < size_t_size; ++ i) {
        ret.push_back(char_arr[i]);
    }

    do {
        ret.push_back(this->c_str()[i - size_t_size]);
    } while (++ i < size_t_size + size);

    return ret;
}

size_t MWString::Deserialize(const std::vector<char> &char_vector, size_t off) {
    const size_t size_t_size = sizeof(size_t);
    size_t size;
    char chs[size_t_size];
    for (size_t i = 0; i < size_t_size; ++ i) {
        chs[i] = char_vector[off + i];
    }
    memcpy(&size, chs, size_t_size);

    char char_arr[size + 1];
    for (size_t i = 0; i < size; ++ i) {
        char_arr[i] = char_vector[off + size_t_size + i];
    }
    char_arr[size] = '\0';

    *this = char_arr;
    return size_t_size + size;
}
