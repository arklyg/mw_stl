#ifndef _MW_STL_CSV_FILE_H_
#define _MW_STL_CSV_FILE_H_

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include <mwvector.h>

#include "mwstring.h"
#include "mwlist.h"

using namespace std;

#define CSVFILE_BUF_LEN 1024000

class CSVFile
{
private:
	int _fd;
	int64_t _off;
	int _end;
	bool _to_end;
	uint32_t _off_in_file;
	static MWString __big_str;
	static char __buf[CSVFILE_BUF_LEN * 2];
	MWString _csv_name;
	int _len;

private:
	int64_t get_line();
	int parse_csv(const char *line_ch, uint32_t len, vector<MWString>& res, uint32_t start = 0);
	int get_csv_line(vector<MWString>& segs, string& ret, bool next_line = true);

public:
	int open(const MWString &csv_name, int mode);
	int open(const MWString &csv_name, const char *mode);
	int open_for_read(const MWString &csv_name);
	int open_for_write(const MWString &csv_name);
	int64_t next_line(vector<MWString> &segs, MWString* line = NULL);
	int close();
	int write_line(vector<MWString> &segs);
};

#endif
