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


#ifndef _MW_MYSQL_UTIL_H_
#define _MW_MYSQL_UTIL_H_

#include <vector>

#include <mysql/mysql.h>

#include "mwstring.h"
#include "mwlist.h"


#define MW_MYSQLUTIL_DEFAULT_MYSQL_PORT 3306
#define MW_MYSQLUTIL_SET_NAMES_UTF8 "SET NAMES 'utf8'"

class MMySQLUtil {
  private:
    const MWString _host;
    const MWString _user;
    const MWString _passwd;
    const MWString _database;

    MYSQL _mysql;
    MYSQL_RES *_result_set;
    size_t _num_rows;
    size_t _num_fields;

  public:
    MMySQLUtil(const MWString &host, const MWString &user, const MWString &passwd,
               const MWString &database = "");
    int Connect();
    int Query(const MWString &sql);
    int Set_UTF8();
    int NextRow(std::vector<MWString> &row);
    size_t GetResult(MWList<std::vector<MWString> > &result_list);
    int free_result();
    size_t GetNumRows();
    size_t GetNumFields();
    int Close();
};

#endif
