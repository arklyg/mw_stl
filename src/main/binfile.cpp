#include <errno.h>
#include <string.h>

#include <mwlogger.h>

#include "binfile.h"

BinFile::BinFile()
	: _file_handle(NULL)
{
}

BinFile::BinFile(const MWString &file_name)
	: _file_handle(NULL)
	, _file_name(file_name)
{
}

BinFile::~BinFile()
{
	if (_file_handle != NULL)
	{
		close();
	}
}

void BinFile::set_file_name(const MWString &file_name)
{
	_file_name = file_name;
}

const MWString &BinFile::get_file_name() const
{
	return _file_name;
}

bool BinFile::open(const MWString &mode)
{
	if (_file_handle != NULL)
	{
		close();
		LOG_WARN(_logger, "_file_handle != NULL, closed eventually");
	}

	_file_handle = fopen(_file_name.c_str(), mode.c_str());
	if (_file_handle == NULL)
	{
		LOG_ERROR(_logger, "fopen(" << _file_name << ", \"" << mode << "\") error, " << strerror(errno) << ", false returned");
		return false;
	}

	return true;
}

bool BinFile::open_for_read()
{
	return open("r");
}

bool BinFile::open_for_read(const MWString &file_name)
{
	_file_name = file_name;
	return open_for_read();
}

bool BinFile::open_for_write()
{
	return open("w");
}

bool BinFile::open_for_write(const MWString &file_name)
{
	_file_name = file_name;
	return open_for_write();
}

bool BinFile::close()
{
	if (_file_handle != NULL)
	{
		if (fclose(_file_handle) != 0)
		{
			LOG_ERROR(_logger, "fclose(_file_handle) error " << strerror(errno));
			return false;
		}

		_file_handle = NULL;
	}
	else
	{
		LOG_WARN(_logger, "fclose(NULL) means nothing, but true returned");
	}

	return true;
}

void BinFile::set_off(size_t off) const
{
	fseek(_file_handle, off, SEEK_SET);
}

int BinFile::read(size_t char_arr_len, char *char_arr) const
{
	if (_file_handle == NULL)
	{
		LOG_ERROR(_logger, "_file_handle == NULL, can not read, file_name = " << _file_name << ", empty vector returned " << strerror(errno));
		return -1;
	}

	return fread(char_arr, sizeof(char), char_arr_len, _file_handle);
}

vector<char> BinFile::read(size_t len) const
{
	vector<char> ret;
	if (_file_handle == NULL)
	{
		LOG_ERROR(_logger, "_file_handle == NULL, can not read, file_name = " << _file_name << ", empty vector returned " << strerror(errno));
		return ret;
	}

	size_t char_arr_len(len == 0 ? MW_STL_BIN_FILE_ONE_TIME_READ_LEN : len), content_len, i;
	char char_arr[char_arr_len];

	do 
	{
		content_len = fread(char_arr, sizeof(char), char_arr_len, _file_handle);

		for (i = 0; i < content_len; ++ i)
		{
			ret.push_back(char_arr[i]);
		}
	} while (content_len != 0 && len == 0);

	return ret;
}

vector<char> BinFile::read(const MWString &file_name, size_t len)
{
	_file_name = file_name;
	return read(len);
}

bool BinFile::write(const vector<char> &vector_char) const
{
	if (_file_handle == NULL)
	{
		LOG_ERROR(_logger, "_file_handle == NULL, can not write, file_name = " << _file_name << ", " << strerror(errno) << ", false returned");
		return false;
	}

	size_t write_len;
	for (size_t i = 0; i < vector_char.size(); ++ i)
	{
		write_len = fwrite(& (vector_char[i]), sizeof(char), 1, _file_handle);
		if (write_len < 1)
		{
			LOG_ERROR(_logger, "write file " << _file_name << " error, " << strerror(errno) << ", return false");
			return false;
		}
	}

	return true;
}

bool BinFile::write(const MWString &file_name, const vector<char> &vector_char)
{
	_file_name = file_name;
	return write(vector_char);
}
