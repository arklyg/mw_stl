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


#ifndef _MW_STL_MW_STRING_
#define _MW_STL_MW_STRING_

#include <string>

#include <mwglobal.h>

#include "mwserializable.h"

typedef const char *MWSTRING_TYPE_ENCODING;

#define BaseString std::string
#define MWSTRING_ENCODING_UTF8 "UTF-8"
#define MWSTRING_ENCODING_GB18030 "GB18030"

#define MWSTRING_SIZET_MINNOTACCEPTED 9223372036854775807

class MWString : public BaseString, public MWSerializable {
    public:
        inline static MWString ToString(size_t num);
        inline static MWString ToString(double num);

        MWString();
        ~MWString();
        MWString(const MWString &s);
        MWString(const BaseString &s);
        MWString(size_t length, const char &ch);
        MWString(const char *str);
        MWString(const char *str, size_t length);
        MWString(const MWString &str, size_t index, size_t length);
        MWString(const size_t num);

        const MWString &operator =(size_t num);
        const MWString &operator =(double num);

        inline size_t ToSizeT() const;
        inline double ToDouble() const;
        MWString Replace(const MWString &wildcard,
                const std::vector<MWString> &replacement_vector) const;
    std::vector<MWString> Split(const char *str) const;
    std::vector<MWString> Split(const MWString &s) const;
    MWString Substr(size_t index, size_t num = BaseString::npos);
    MWString Encode(MWSTRING_TYPE_ENCODING encoding_from,
                    MWSTRING_TYPE_ENCODING encoding_to);

    virtual void AddSequence();
    virtual std::vector<char> Serialize();
    virtual size_t Deserialize(const std::vector<char> &char_vector, size_t off);
};

inline MWString MWString::ToString(size_t num) {
    MWString ret;
    return ret = num;
}

inline MWString MWString::ToString(double num) {
    MWString ret;
    return ret = num;
}

inline size_t MWString::ToSizeT() const {
    size_t ret(atol(this->c_str()));
    return ret < MWSTRING_SIZET_MINNOTACCEPTED ? ret : 0;
}

inline double MWString::ToDouble() const {
    return (double) atof(this->c_str());
}

#endif
