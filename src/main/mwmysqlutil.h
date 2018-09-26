#ifndef _MW_MYSQL_UTIL_H_
#define _MW_MYSQL_UTIL_H_

#include <mysql/mysql.h>

#include "mwstring.h"
#include "mwvector.h"
#include "mwlist.h"

using namespace std;

#define MW_MYSQLUTIL_DEFAULT_MYSQL_PORT 3306
#define MW_MYSQLUTIL_SET_NAMES_UTF8 "SET NAMES 'utf8'"

class MWMySQLUtil
{
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
	MWMySQLUtil();
	MWMySQLUtil(const MWString& host, const MWString& user, const MWString& passwd, const MWString& database = "");

	int init(const MWString& host, const MWString& user, const MWString& passwd, const MWString& database = "");
	int connect();
	int query(const MWString& sql);
	int set_utf8();
	int next_row(MWVector<MWString>& row);
	size_t get_result(MWList<MWVector<MWString> >& result_list);
	int free_result();
	size_t get_num_rows();
	size_t get_num_fields();
	int close();
};

#endif
