/*
// Copyright (C) 2016 Entropy Technologies LLC. All rights reserved.
*/

#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

#include <mwlogger.h>
#include <bufferfile.h>

BufferFile::BufferFile(const std::string &file_path)
	:file_path_(file_path) {

	std::ifstream ifs(file_path.c_str(), std::ios::in | std::ios::binary);
	if (!ifs) {
		LOGERROR("Can't open the file. Please check " << file_path);
		return;
	}

	ifs.seekg(0, std::ios::end);
	length_ = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	LOGINFO(file_path << " ... "<< length_ << " bytes");

	buffer_ = new char[sizeof(char) * length_];
	ifs.read(buffer_, length_);
	ifs.close();
}

int BufferFile::GetLength() {
	return length_;
}

char* BufferFile::GetBuffer() {
	return buffer_;
}

BufferFile::~BufferFile() {
	delete[] buffer_;
	buffer_ = NULL;
}
