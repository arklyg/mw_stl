#ifndef _MW_STL_MW_STRING_
#define _MW_STL_MW_STRING_

#include <string>
#include <limits.h>

#include <mwglobal.h>
#include <mwvector.h>

#include "mwserializable.h"

typedef const char* MWSTRING_TYPE_ENCODING;

#define BaseString std::string
#define MWSTRING_ENCODING_UTF8 "UTF-8"
#define MWSTRING_ENCODING_GB18030 "GB18030"

#define MWSTL_MWSTRING_DECIMALPOINT ','

class MWString : public BaseString, public MWSerializable
{
public:
	static MWString to_string(size_t num);
	static MWString to_string(double num, size_t dec_place);

	MWString();
	~MWString();
	MWString(const MWString &s);
	MWString(const BaseString &s);
	MWString(size_t length, const char &ch);
	MWString(const char* str);
	MWString(const char* str, size_t length);
	MWString(const MWString &str, size_t index, size_t length);
	MWString(const size_t num);
	MWString(const double num, size_t dec_place);

	const MWString & operator =(size_t num);
	const MWString & operator =(double num);

	size_t to_size_t() const;
	double to_double() const;
	MWString replace(const MWString &wildcard, const MWVector<MWString> &replacement_vector) const;
	MWVector<MWString> split(const char* str) const;
	MWVector<MWString> split(const MWString &s) const;
	MWString substr(size_t index, size_t num = BaseString::npos);
	MWString encode(MWSTRING_TYPE_ENCODING encoding_from, MWSTRING_TYPE_ENCODING encoding_to);

	virtual void add_serializing_sequence();
	virtual vector<char> get_serialized();
	virtual size_t get_instantiated(const vector<char> &char_vector, size_t off);
};

#endif
