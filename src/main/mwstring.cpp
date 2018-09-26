#include <stdlib.h>
#include <sstream>
#include <string.h>
#include <iconv.h>
#include <limits>

#include "mwstring.h"

MWString MWString::to_string(size_t num)
{
	MWString ret;
	return ret = num;
}

MWString MWString::to_string(double num, size_t dec_place)
{
	ostringstream out;
	out.precision(numeric_limits<double>::digits10);//覆盖默认精度
	out << num;
	MWString ret = out.str(); //从流中取出字符串

	size_t n = ret.find(MWSTL_MWSTRING_DECIMALPOINT);
	if ((n != MWString::npos) && (ret.size() > n + dec_place)) {//有小数点吗？//后面至少还有dec_place位吗？
		ret[n + dec_place] = '\0';//覆盖第一个多余的数
	}

	return ret;
}

MWString::MWString()
	: BaseString()
{
}

MWString::~MWString()
{
}

MWString::MWString(const MWString &s)
	: BaseString(s)
{
}

MWString::MWString(const BaseString &s)
	: BaseString(s)
{
}

MWString::MWString(size_t length, const char &ch)
	: BaseString(length, ch)
{
}

MWString::MWString(const char* str)
	: BaseString(str == NULL ? "" : str)
{
}

MWString::MWString(const char* str, size_t length)
	: BaseString(str, length)
{
}

MWString::MWString(const MWString &str, size_t index, size_t length)
	: BaseString(str, index, length)
{
}

MWString::MWString(const size_t num)
	: BaseString()
{
	* this = num;
}

MWString::MWString(const double num, size_t dec_place)
	: BaseString()
{
	* this = MWString::to_string(num, dec_place);
}

const MWString & MWString::operator =(size_t num)
{
	std::stringstream s;
	s << num;
	s >> * this;
	return * this;
}

const MWString & MWString::operator =(double num)
{
	std::stringstream s;
	s << num;
	s >> * this;
	return * this;
}

size_t MWString::to_size_t() const
{
	size_t ret(atol(this->c_str()));
	return ret < LONG_MAX ? ret : 0;
}

double MWString::to_double() const
{
	return (double) atof(this->c_str());
}

MWString MWString::replace(const MWString &wildcard, const MWVector<MWString> &replacement_vector) const
{
	if (replacement_vector.empty())
	{
		return * this;
	}

	MWVector<MWString> segs = split(wildcard);
	if (segs.empty())
	{
		return * this;
	}

	MWString ret = segs[0];
	for (size_t i = 1; i < segs.size() && i - 1 < replacement_vector.size(); ++ i)
	{
		ret += (replacement_vector[i - 1] + segs[i]);
	}

	return ret;
}

MWString MWString::substr(size_t index, size_t num)
{
	return static_cast<MWString>(BaseString::substr(index, num));
}

MWString MWString::encode(MWSTRING_TYPE_ENCODING encoding_from, MWSTRING_TYPE_ENCODING encoding_to)
{
	iconv_t cd(iconv_open(encoding_to, encoding_from));
	if (cd == reinterpret_cast<iconv_t>(- 1))
	{
		return "";
	}

	size_t src_len(this->length()), dst_len(6 * this->length());
	char src[src_len + 1], * src_pt(reinterpret_cast<char*>(& src));
	strcpy(src, this->c_str());
	char dst[dst_len + 1], * dst_pt(reinterpret_cast<char*>(& dst));
	memset(dst, 0, dst_len + 1);

	iconv(cd, & src_pt, & src_len, & dst_pt, & dst_len);
	return MWString(dst);
}

MWVector<MWString> MWString::split(const char* str) const
{
	MWString s(str);
	return split(s);
}

MWVector<MWString> MWString::split(const MWString &s) const
{
	MWVector<MWString> ret;
	MWString str(* this);
	size_t start_pos(0), end_pos;
	while (true)
	{
		end_pos = this->find(s, start_pos);
		if (end_pos != BaseString::npos)
		{
			ret.push_back(str.substr(start_pos, end_pos - start_pos));
			start_pos = end_pos + s.length();
		}
		else
		{
			break;
		}
	}
	ret.push_back(str.substr(start_pos));

	return ret;
}

void MWString::add_serializing_sequence()
{
	return;
}

vector<char> MWString::get_serialized()
{
	MWVector<char> ret;
	const size_t size = this->size();
	const size_t size_t_size = sizeof(size_t);
	char char_arr[size_t_size + size];
	memcpy(char_arr, & size, size_t_size);

	size_t i;
	for (i = 0; i < size_t_size; ++ i)
	{
		ret.push_back(char_arr[i]);
	}

	do
	{
		ret.push_back(this->c_str()[i - size_t_size]);
	} while (++ i < size_t_size + size);

	return ret;
}

size_t MWString::get_instantiated(const vector<char> &char_vector, size_t off)
{
	const size_t size_t_size = sizeof(size_t);
	size_t size;
	char chs[size_t_size];
	for (size_t i = 0; i < size_t_size; ++ i)
	{
		chs[i] = char_vector[off + i];
	}
	memcpy(& size, chs, size_t_size);
	
	char char_arr[size + 1];
	for (size_t i = 0; i < size; ++ i)
	{
		char_arr[i] = char_vector[off + size_t_size + i];
	}
	char_arr[size] = '\0';

	* this = char_arr;
	return size_t_size + size;
}
