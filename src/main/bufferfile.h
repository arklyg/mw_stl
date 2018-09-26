#ifndef _MWSTL_BUFFERFILE_H_
#define _MWSTL_BUFFERFILE_H_

#include <string>

class BufferFile {
public:
	std::string file_path_;
	int length_;
	char* buffer_;

public:
    explicit BufferFile(const std::string &file_path);
	int GetLength();
	char* GetBuffer();
	~BufferFile();
};

#endif
