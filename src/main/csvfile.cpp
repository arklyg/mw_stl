#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "csvfile.h"


char CSVFile::__buf[CSVFILE_BUF_LEN*  2];

MWString CSVFile::__big_str;

int CSVFile::open_for_read(const MWString &csv_name)
{
	return open(csv_name, "r");
}

int CSVFile::open_for_write(const MWString &csv_name)
{
	return open(csv_name, "w");
}

int CSVFile::open(const MWString &csv_name, int mode)
{
	_csv_name = csv_name;
	if (mode & O_CREAT)
	{
		_fd = ::open(csv_name.c_str(), mode, S_IRUSR | S_IWUSR);
	}
	else
	{
		_fd = ::open(csv_name.c_str(), mode);
	}

	if (_fd < 0)
	{
		LOG_ERROR(_logger, "fopen file: " << csv_name << " error, " << strerror(errno));
		return -1;
	}

	_off = 0;
	_end = 0;
	_off_in_file = 0;
	_to_end = false;

	return 0;
}

int CSVFile::open(const MWString &csv_name, const char* mode)
{
	int open_mode;
	if (mode[0] == 'r')
	{
		open_mode = O_RDONLY;
	}
	else
	{
		open_mode = O_WRONLY | O_CREAT | O_TRUNC;
	}

	return open(csv_name, open_mode);
}

int CSVFile::close()
{
	return ::close(_fd);
}

int64_t CSVFile::get_line()
{
	int i;
	// 从内存中扫一行
	for (i = _off; i < _end; ++i)
	{
		// 如果扫到了，返回位置
		if (__buf[i] == '\n')
		{
			return ++i;
		}
	}

	if (_to_end)
	{
		return -1;
	}

	int ret = read(_fd, __buf + _end, CSVFILE_BUF_LEN - _end);
	if (ret < 0)
	{
		LOG_ERROR(_logger, "read file " << _csv_name << " error, " << strerror(errno));
		return -2;
	}
	_to_end = ret < CSVFILE_BUF_LEN - _end;

	_end += ret;
	
	for ( ; i < _end; ++i)
	{
		if (__buf[i] == '\n')
		{
			return ++i;
		}
	}

	if (_to_end)
	{
		return -1;
	}

	ret = read(_fd, __buf, _off);
	if (ret < 0)
	{
		LOG_ERROR(_logger, "read file " << _csv_name << " error, " << strerror(errno));
		return -2;
	}
	_to_end = ret < _off;

	_end = ret;

	for (i = 0; i < _end; ++i)
	{
		if (__buf[i] == '\n')
		{
			return ++i;
		}
	}

	if (_to_end)
	{
		return -1;
	}

	LOG_ERROR(_logger, CSVFILE_BUF_LEN << " too short for csv file: " << _csv_name);
	return -2;
}

// -1, to the end; -2 error; >= 0 normal read
int64_t CSVFile::next_line(vector<MWString> &segs, MWString* line)
{
	int ret = get_line();
	if (ret < 0)
	{
		return ret;
	}

	uint32_t len;
	if (ret <= _off)
	{
		memcpy(__buf + CSVFILE_BUF_LEN, __buf, ret);
		len = ret + CSVFILE_BUF_LEN - _off;
	}
	else
	{
		len = ret - _off;
	}

	parse_csv(__buf + _off, len, segs);
	if (line != NULL)
	{
		char line_ch[len + 1];
		memcpy(line_ch, __buf + _off, len);
		for (int i = len; i >= 0; -- i) {
			if (line_ch[i] == '\n') {
				line_ch[i] = '\0';
			}
		}
		* line = line_ch;
	}

	_off = (_off + len) % CSVFILE_BUF_LEN;
	if (_off == 0)
	{
		_end = 0;
	}
	_off_in_file += len;

	return _off_in_file;
}

int CSVFile::write_line(vector<MWString> &segs)
{
	__big_str = "";
	get_csv_line(segs, __big_str);
	ssize_t ret = write(_fd, __big_str.c_str(), __big_str.length());
	if (ret < (ssize_t) __big_str.length())
	{
		LOG_ERROR(_logger, "write to csv file error, " << strerror(errno));
		return -1;
	}

	return ret;
}

int CSVFile::parse_csv(const char* line_ch, uint32_t len, vector<MWString> &res, uint32_t start)
{
	int state = 0;
	char ch;
	MWString cur = "";
	size_t res_original_size = res.size(), res_size_cnt = 0;

	len += start;
	for (uint32_t i = start; i < len; ++i)
	{
		ch = line_ch[i];
		if (ch == '\n')
		{
			break;
		}
		switch (state)
		{
		case 0:
			if (ch == '"')
			{
				state = 1;
			}
			else if (ch == ',')
			{
				if (res_size_cnt < res_original_size)
				{
					res[res_size_cnt] = cur;
					++res_size_cnt;
				}
				else
				{
					res.push_back(cur.c_str());
				}
				cur = "";
				state = 0;
			}
			else
			{
				cur += ch;
				state = 2;
			}
			break;
		case 1:
			if (ch == '"')
			{
				state = 3;
			}
			else if (ch == ',')
			{
				state = 1;
				cur += ch;
			}
			else
			{
				state = 1;
				cur += ch;
			}
			break;
		case 2:
			if (ch == '"')
			{
				state = 2;
				cur += ch;
			}
			else if (ch == ',')
			{
				state = 0;
				if (res_size_cnt < res_original_size)
				{
					res[res_size_cnt] = cur;
					++res_size_cnt;
				}
				else
				{
					res.push_back(cur);
				}
				cur = "";
			}
			else
			{
				state = 2;
				cur += ch;
			}
			break;
		case 3:
			if (ch == '"')
			{
				state = 1;
				cur += ch;
			}
			else if (ch == ',')
			{
				state = 0;
				if (res_size_cnt < res_original_size)
				{
					res[res_size_cnt] = cur;
					++res_size_cnt;
				}
				else
				{
					res.push_back(cur);
				}
				cur = "";
			}
			else
			{
				return -1;
			}
			break;
		default:
			return -1;
		}
	}

	if (res_size_cnt < res_original_size)
	{
		res[res_size_cnt] = cur;
		++res_size_cnt;
		for (size_t j = 0; j < res_original_size - res_size_cnt; ++j)
		{
			res.pop_back();
		}
	}
	else
	{
		res.push_back(cur);
	}

	return res_size_cnt;
}

int CSVFile::get_csv_line(vector<MWString>& segs, string& ret, bool next_line)
{
	uint32_t seg_len;
	size_t count = 0, len = segs.size();
	for (vector<MWString>::iterator iter = segs.begin(); iter != segs.end(); ++iter)
	{
		const char *seg = (*iter).c_str();
		seg_len = (*iter).length();
		ret += "\"";
		for (uint32_t i = 0; i < seg_len; ++i)
		{
			//if (seg[i] == ',' || seg[i] == '"')
			if (seg[i] == '"')
			{
				ret += "\"";
			}
			ret += seg[i];
		}
		ret += "\"";
		if (++count < len)
		{
			ret += ",";
		}
	}

	if (next_line)
	{
		ret += "\n";
	}

	return 0;
}
