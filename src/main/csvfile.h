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


#ifndef _MW_STL_CSV_FILE_H_
#define _MW_STL_CSV_FILE_H_

#include <stdio.h>
#include <stdint.h>

#include "mwstring.h"
#include "mwlist.h"

#define CSVFILE_BUF_LEN 1024000

class CSVFile {
  private:
    int _fd;
    int64_t _off;
    int _end;
    bool _to_end;
    uint32_t _off_in_file;
    static MWString __big_str;
    static char __buf[CSVFILE_BUF_LEN * 2];
    MWString _csv_name;

  private:
    int64_t GetLine();
    int ParseCSV(const char *line_ch, uint32_t len, std::vector<MWString> &res,
                 uint32_t start = 0);
    int GetCSVLine(std::vector<MWString> &segs, std::string &ret, bool next_line = true);

  public:
    int Open(const MWString &csv_name, int mode);
    inline int Open(const MWString &csv_name, const char *mode);
    inline int OpenForRead(const MWString &csv_name);
    inline int OpenForWrite(const MWString &csv_name);
    int64_t NextLine(std::vector<MWString> &segs, MWString *line = NULL);
    inline int Close();
    int WriteLine(std::vector<MWString> &segs);
};

inline int CSVFile::Open(const MWString &csv_name, const char *mode) {
    int open_mode;
    if (mode[0] == 'r') {
        open_mode = O_RDONLY;
    } else {
        open_mode = O_WRONLY | O_CREAT | O_TRUNC;
    }

    return Open(csv_name, open_mode);
}

inline int CSVFile::OpenForRead(const MWString &csv_name) {
    return Open(csv_name, "r");
}

inline int CSVFile::OpenForWrite(const MWString &csv_name) {
    return Open(csv_name, "w");
}

inline int CSVFile::Close() {
    return ::close(_fd);
}

#endif
