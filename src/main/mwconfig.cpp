#include <iostream>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include <mwglobal.h>
#include <mwlogger.h>

#include "mwconfig.h"

int MWConfig::parse(char *line)
{
	int len = strlen(line);
	while (len != 0 && line[len - 1] == '\n')
	{
		line[len - 1] = '\0';
		len = strlen(line);
	}
	if (line[0] == '\0' || line[0] == '\n' || line[0] == '#' || line[0] == EOF)
	{
		return 0;
	}

	char *pos = strstr(line, CONF_KEY_VAL_SEPER);
	if (pos == NULL)
	{
		LOG_ERROR(_logger, "sepper not found in conf");
		return -1;
	}

	char key[LINE_LEN], val[LINE_LEN];
	strncpy(key, line, pos - line);
	key[pos - line] = '\0';

	strncpy(val, pos + 1, line + len - pos - 1);
	val[line + len - pos - 1] = '\0';

	return insert(key, val);
}

int MWConfig::insert(const string& key, const string& val)
{
	map<string, ConfVal>::iterator iter = _key_vals.find(key);
	if (iter != _key_vals.end())
	{
		LOG_WARN(_logger, "key:" << key << " has two or more values, latter val ignored");
		return 0;
	}

	_key_vals.insert(pair<string, ConfVal>(key, ConfVal(val, _num++)));

	return 0;
}

void MWConfig::clear()
{
	_num = 0;
	_key_vals.clear();
}

MWConfig::MWConfig(const string& file_name)
	: _file_name(file_name)
	, _num(0)
{
}

int MWConfig::load()
{
	FILE *conf = fopen(_file_name.c_str(), "r");
	if (conf == NULL)
	{
		cout << "open " << _file_name << " error, " << strerror(errno) << endl;
		return -1;
	}
	int ret = parse_lines(conf);
	if (fclose(conf) != 0 || ret < 0)
	{
		return -2;
	}
	return 0;
}

int MWConfig::parse_lines(FILE *conf)
{
	char buf[LINE_LEN + 1], *ret;
	while (true)
	{
		errno = 0;
		ret = fgets(buf, LINE_LEN, conf);
		if (errno != 0)
		{
			cout << "fgets " << _file_name << " error, " << strerror(errno) << endl;
			return -2;
		}
		if (ret == NULL)
		{
			break;
		}
		if (parse(ret) < 0)
		{
			cout << "parse line: " << ret << " error" << endl;
			return -3;
		}
	}
	return 0;
}

set<string> MWConfig::get_keys() const
{
	set<string> keys;
	for (map<string, ConfVal>::const_iterator iter = _key_vals.begin(); iter != _key_vals.end(); ++iter)
	{
		keys.insert(iter->first);
	}
	return keys;
}

int MWConfig::get_keys_num() const
{
	return _key_vals.size();
}

int MWConfig::get_int_val(string key) const
{
	string val = get_string_val(key);
	if (val == "")
	{
		return 0;
	}
	return atoi(val.c_str());
}

string MWConfig::get_string_val(const string& key) const
{
	map<string, ConfVal>::const_iterator iter = _key_vals.find(key);
	if (iter == _key_vals.end())
	{
		return "";
	}
	return iter->second._val;
}
