#ifndef _MW_STL_BIN_FILE_H_
#define _MW_STL_BIN_FILE_H_

#include <stdio.h>
#include <vector>

#include <mwstring.h>

#define MW_STL_BIN_FILE_ONE_TIME_READ_LEN 0x400 // = 1024

using namespace std;

class BinFile
{
private:
	FILE* _file_handle;

	MWString _file_name;

public:
	BinFile();
	BinFile(const MWString &file_name);
	~BinFile();

	void set_file_name(const MWString &file_name);
	const MWString &get_file_name() const;

	bool open(const MWString &mode);
	bool open_for_read();
	bool open_for_read(const MWString &file_name);
	bool open_for_write();
	bool open_for_write(const MWString &file_name);

	bool close();

	void set_off(size_t off) const;
	int read(size_t len, char *char_arr) const;
	vector<char> read(size_t len = 0) const;
	vector<char> read(const MWString &file_name, size_t len = 0);

	bool write(const vector<char> &vector_char) const;
	bool write(const MWString &file_name, const vector<char> &vector_char);
};


#endif

