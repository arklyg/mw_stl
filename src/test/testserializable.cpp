#include <string.h>

#include <mwlogger.h>
#include <mwmathglobal.h>
#include <mwvector.h>
#include <mwmatrix.h>

#include <typehelpers.h>

#define PROGRAMM_NAME "testserializable"

int main(int argc, const char* argv[])
{
	init_logger(PROGRAMM_NAME);

	MWVector<MWData> v1(3);
	v1[0] = 1;
	v1[1] = 2;
	v1[2] = 3;
	LOG_INFO(_logger, "v1 = " << v1);

	vector<char> char_vector = v1.get_serialized();

	char len_data[sizeof(size_t)];
	for (size_t i = 0; i < sizeof(size_t); ++ i)
	{
		len_data[i] = char_vector[i];
	}

	size_t len;
	memcpy(& len, len_data, sizeof(size_t));

	LOG_INFO(_logger, "len = " << len);

	char all_data[char_vector.size()];
	for (size_t i = 0; i < char_vector.size(); ++ i)
	{
		all_data[i] = char_vector[i];
	}
	
	MWData* mwdata;
	for (size_t i = 0; i < 3; ++ i)
	{
		mwdata = reinterpret_cast<MWData*>(all_data + sizeof(size_t) + i * sizeof(MWData));
		LOG_INFO(_logger, "data = " << *mwdata);
	}

	MWVector<MWData> v2;
	v2.get_instantiated(char_vector, 0);
	LOG_INFO(_logger, "v2 = " << v2);

	MWVector<MWVector<MWData> > v3;
	v3.push_back(v2);
	v3.push_back(v2);
	v3.push_back(v2);
	
	char_vector = v3.get_serialized();
	char all_data_3[char_vector.size()];
	LOG_INFO(_logger, "v3 = " << v3);

	for (size_t i = 0; i < char_vector.size(); ++ i)
	{
		all_data_3[i] = char_vector[i];
	}

	MWVector<MWVector<MWData> > v4;
	v4.get_instantiated(char_vector, 0);
	LOG_INFO(_logger, "v4 = " << v4);

	MWMatrix<MWData> m1(v1);
	LOG_INFO(_logger, "m1 = " << m1);

	char_vector = m1.get_serialized();
	char all_data_m1[char_vector.size()];
	for (size_t i = 0; i < char_vector.size(); ++ i)
	{
		all_data_m1[i] = char_vector[i];
	}

	MWMatrix<MWData> m2;
	m2.get_instantiated(char_vector, 0);
	LOG_INFO(_logger, "m2 = " << m2);

	m2._data[1][2] = 100;
	MWVector<MWMatrix<MWData> > v_m;
	v_m.push_back(m1);
	v_m.push_back(m2);
	v_m.push_back(m1);
	v_m.push_back(m2);
	LOG_INFO(_logger, "v_m = " << v_m);
	
	char_vector = v_m.get_serialized();
	char all_data_v_m[char_vector.size()];
	
	for (size_t i = 0; i < char_vector.size(); ++ i)
	{
		all_data_v_m[i] = char_vector[i];
	}

	MWVector<MWMatrix<MWData> > v_m_2;
	v_m_2.get_instantiated(char_vector, 0);
	
	LOG_INFO(_logger, "v_m_2 = " << v_m_2);

	return EXIT_SUCCESS;
}
