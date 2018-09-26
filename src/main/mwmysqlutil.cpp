#include <mwglobal.h>

#include "mwmysqlutil.h"

MWMySQLUtil::MWMySQLUtil()
	: _result_set(NULL)
{
}

int MWMySQLUtil::init(const MWString& host, const MWString& user, const MWString& passwd, const MWString& database = "")
{
	_host = host;
	_user = user;
	_passwd = passwd;
	_database = database;

	return mysql_init(& _mysql);
}

MWMySQLUtil::MWMySQLUtil(const MWString& host, const MWString& user, const MWString& passwd, const MWString& database)
	: _host(host)
	, _user(user)
	, _passwd(passwd)
	, _database(database)
	, _result_set(NULL)
{
	mysql_init(& _mysql);
}

int MWMySQLUtil::connect()
{
	if (mysql_real_connect(&_mysql, _host.c_str(), _user.c_str(), _passwd.c_str(), _database == "" ? NULL : _database.c_str(), MW_MYSQLUTIL_DEFAULT_MYSQL_PORT, NULL, 0) == NULL)
	{
		LOG_ERROR(_logger, "mysql_real_connect to " << _user << ":" << _passwd << "@" << _host << ":" << MW_MYSQLUTIL_DEFAULT_MYSQL_PORT << "." << _database << " ERROR");
		return -1;
	}

	return 0;
}

int MWMySQLUtil::set_utf8()
{
	MWString sql = MW_MYSQLUTIL_SET_NAMES_UTF8;
	if (mysql_query(&_mysql, sql.c_str()) != 0)
	{
		LOG_ERROR(_logger, "mysql_query sql " << sql << " ERROR");
		return -1;
	}
	return 0;
}

int MWMySQLUtil::query(const MWString& sql)
{
	if (mysql_query(&_mysql, sql.c_str()) != 0)
	{
		LOG_ERROR(_logger, "mysql_query sql " << sql << " ERROR");
		return -1;
	}

	if ((_result_set = mysql_store_result(&_mysql)) == NULL)
	{
		LOG_ERROR(_logger, "mysql_store_result " << sql << " ERROR");
		return -2;
	}

	_num_rows = mysql_num_rows(_result_set);
	_num_fields = mysql_num_fields(_result_set);
	return _num_rows;
}

int MWMySQLUtil::next_row(MWVector<MWString>& row)
{
	// 还没有获取
	if (_result_set == NULL)
	{
		return 0;
	}

	if (row.size() != _num_fields)
	{
		row.resize(_num_fields);
	}

	MYSQL_ROW mysql_row = mysql_fetch_row(_result_set);
	if (mysql_row == NULL)
	{
		return 0;
	}

	for (size_t i = 0; i < _num_fields; ++i)
	{
		row[i] = mysql_row[i];
	}

	return 1;
}

size_t MWMySQLUtil::get_result(MWList<MWVector<MWString> >& result_list)
{
	MWVector<MWString> row;
	size_t len = 0;

	while (next_row(row) > 0)
	{
		result_list.push_back(row);
		++ len;
	}

	return len;
}

int MWMySQLUtil::free_result()
{
	if (_result_set != NULL)
	{
		mysql_free_result(_result_set);
	}
	return 0;
}

size_t MWMySQLUtil::get_num_rows()
{
	return _num_rows;
}

size_t MWMySQLUtil::get_num_fields()
{
	return _num_fields;
}

int MWMySQLUtil::close()
{
	mysql_close(&_mysql);
	return 0;
}
