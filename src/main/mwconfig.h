#ifndef _MW_CONFIGURATOR_H_
#define _MW_CONFIGURATOR_H_

#include <map>
#include <string.h>
#include <set>

#define LINE_LEN 1024
#define CONF_KEY_VAL_SEPER "="
#define CONF_VAL_SEPER ","

using namespace std;

struct ConfVal
{
	string _val;
	int _num;

public:
	ConfVal(const string& val, int num)
		: _val(val)
		, _num(num)
	{
	}
};

class MWConfig
{
private:
	string _file_name;
	map<string, ConfVal> _key_vals;
	int _num;

private:
	int parse(char *line);
	void clear();
	int parse_lines(FILE *conf);
	int insert(const string& key, const string& val);

public:
	MWConfig(const string& file_name);

	int load();
	set<string> get_keys() const;
	int get_keys_num() const;
	int get_int_val(string key) const;
	string get_string_val(const string& key) const;
};

#endif
