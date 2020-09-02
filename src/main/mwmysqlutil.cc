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


#include <mwglobal.h>
#include <mwlogger.h>

#include "mwmysqlutil.h"

MMySQLUtil::MMySQLUtil(const MWString &host, const MWString &user,
                       const MWString &passwd, const MWString &database)
    : _host(host)
    , _user(user)
    , _passwd(passwd)
    , _database(database)
    , _result_set(NULL) {
    mysql_init(&_mysql);
}

int MMySQLUtil::Connect() {
    if (mysql_real_connect(&_mysql, _host.c_str(), _user.c_str(), _passwd.c_str(),
                           _database == "" ? NULL : _database.c_str(), MW_MYSQLUTIL_DEFAULT_MYSQL_PORT,
                           NULL, 0) == NULL) {
        LOGERROR("mysql_real_connect to " << _user << 
                 ":" << _passwd << 
                 "@" << _host << 
                 ":" << MW_MYSQLUTIL_DEFAULT_MYSQL_PORT << 
                 "." << _database <<
                 " ERROR");
        return -1;
    }

    return 0;
}

int MMySQLUtil::Set_UTF8() {
    MWString sql = MW_MYSQLUTIL_SET_NAMES_UTF8;
    if (mysql_query(&_mysql, sql.c_str()) != 0) {
        LOGERROR("mysql_query sql " << sql << " ERROR");
        return -1;
    }
    return 0;
}

int MMySQLUtil::Query(const MWString &sql) {
    if (mysql_query(&_mysql, sql.c_str()) != 0) {
        LOGERROR("mysql_query sql " << sql << " ERROR");
        return -1;
    }

    if ((_result_set = mysql_store_result(&_mysql)) == NULL) {
        LOGERROR("mysql_store_result " << sql << " ERROR");
        return -2;
    }

    _num_rows = mysql_num_rows(_result_set);
    _num_fields = mysql_num_fields(_result_set);
    return _num_rows;
}

int MMySQLUtil::NextRow(std::vector<MWString> &row) {
    // 还没有获取
    if (_result_set == NULL) {
        return 0;
    }

    if (row.size() != _num_fields) {
        row.resize(_num_fields);
    }

    MYSQL_ROW mysql_row = mysql_fetch_row(_result_set);
    if (mysql_row == NULL) {
        return 0;
    }

    for (size_t i = 0; i < _num_fields; ++i) {
        row[i] = mysql_row[i];
    }

    return 1;
}

size_t MMySQLUtil::GetResult(MWList<std::vector<MWString> > &result_list) {
    std::vector<MWString> row;
    size_t len = 0;

    while (NextRow(row) > 0) {
        result_list.push_back(row);
        ++ len;
    }

    return len;
}

int MMySQLUtil::free_result() {
    if (_result_set != NULL) {
        mysql_free_result(_result_set);
    }
    return 0;
}

size_t MMySQLUtil::GetNumRows() {
    return _num_rows;
}

size_t MMySQLUtil::GetNumFields() {
    return _num_fields;
}

int MMySQLUtil::Close() {
    mysql_close(&_mysql);
    return 0;
}
