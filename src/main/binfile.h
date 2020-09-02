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


#ifndef _MW_STL_BIN_FILE_H_
#define _MW_STL_BIN_FILE_H_

#include <stdio.h>
#include <vector>

#include "mwstring.h"

#define MW_STL_BIN_FILE_ONE_TIME_READ_LEN 0x400 // = 1024

class BinFile {
  private:
    FILE *_file_handle;

    MWString _file_name;

  public:
    BinFile();
    BinFile(const MWString &file_name);
    ~BinFile();

    void SetFileName(const MWString &file_name);
    const MWString &GetFileName() const;

    bool Open(const MWString &mode);
    bool OpenForRead();
    bool OpenForRead(const MWString &file_name);
    bool OpenForWrite();
    bool OpenForWrite(const MWString &file_name);

    bool Close();

    void SetOff(size_t off) const;
    int Read(size_t len, char *char_arr) const;
    std::vector<char> Read(size_t len = 0) const;
    std::vector<char> Read(const MWString &file_name, size_t len);

    bool Write(const std::vector<char> &vector_char) const;
    bool Write(const MWString &file_name, const std::vector<char> &vector_char);
};


#endif

