#ifndef _MW_STL_MW_SERIALIZABLE_H_
#define _MW_STL_MW_SERIALIZABLE_H_

#include <vector>

#include <mwglobal.h>
#include <mwmathglobal.h>

using namespace std;

class MWSerializableSingleton;
class MWSerializable;

typedef vector<char> (MWSerializable::* MWSerializableFunctionGetVectorCharWithVoidPt)(const void*) const;
typedef size_t (MWSerializable::* MWSerializableFunctionGetSizeTAssignInstanceWithCharPtSizeT)(void*, const vector<char> &, size_t);


class MWSerializable
{
protected:
	vector<void*> _serializing_sequence_void_pt_vector;
	vector<MWSerializableFunctionGetVectorCharWithVoidPt> _get_serialized_function_vector;
	vector<MWSerializableFunctionGetSizeTAssignInstanceWithCharPtSizeT> _get_instantiated_function_vector;

private:
	vector<char> get_serializable_serialized(const void* instance_pt) const;
	vector<char> get_singleton_pt_serialized(const void* instance_pt) const;
	vector<char> get_mwdata_serialized(const void* instance_pt) const;
	vector<char> get_size_t_serialized(const void* instance_pt) const;
	vector<char> get_char_serialized(const void* instance_pt) const;

	size_t get_serializable_instantiated(void* instance_pt, const vector<char> &char_vector, size_t off);
	size_t get_singleton_pt_instantiated(void* instance_pt, const vector<char> &char_vector, size_t off);
	size_t get_mwdata_instantiated(void* instance_pt, const vector<char> &char_vector, size_t off);
	size_t get_size_t_instantiated(void* instance_pt, const vector<char> &char_vector, size_t off);
	size_t get_char_instantiated(void* instance_pt, const vector<char> &char_vector, size_t off);

protected:
	virtual void add_serializing_sequence() = 0;
	virtual void add_serializing_sequence_for_instantiating();

	virtual void set_serializing_sequence_for_serializing();
	virtual void set_serializing_sequence_for_instantiating();

	void add_serializing_sequence_data(MWSerializable* pt);
	void add_serializing_sequence_data(MWSerializableSingleton** pt);
	void add_serializing_sequence_data(MWData* pt);
	void add_serializing_sequence_data(size_t* pt);
	void add_serializing_sequence_data(char* pt);

public:
	virtual ~MWSerializable();

	virtual vector<char> get_serialized();
	virtual size_t get_instantiated(const vector<char> &char_vector, size_t off);
};

template <typename T>
vector<char> get_basic_serialized(const T* instance_pt)
{
	size_t len = sizeof(T);
	char chs[len];

	memcpy(chs, instance_pt, len);

	vector<char> ret(len);
	for (size_t i = 0; i < len; ++ i)
	{
		ret[i] = chs[i];
	}

	return ret;
}

template <typename T>
size_t get_basic_instantiated(T* instance_pt, const vector<char> &char_vector, size_t off)
{
	size_t len = sizeof(T);
	char chs[len];
	for (size_t i = 0; i < len; ++ i)
	{
		chs[i] = char_vector[off + i];
	}

	memcpy(instance_pt, chs, len);

	return len;
}

vector<char> get_singleton_pt_serialized(const MWSerializableSingleton** instance_pt_pt);

size_t get_singleton_pt_instantiated(MWSerializableSingleton** instance_pt_pt, const vector<char> &char_vector, size_t off);

#endif
