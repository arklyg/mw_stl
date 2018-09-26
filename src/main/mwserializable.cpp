#include <string.h>

#include "mwserializablesingleton.h"

#include "mwserializable.h"

MWSerializable::~MWSerializable()
{
}

void MWSerializable::set_serializing_sequence_for_serializing()
{
	_serializing_sequence_void_pt_vector.clear();
	_get_serialized_function_vector.clear();
	_get_instantiated_function_vector.clear();
	add_serializing_sequence();
}

void MWSerializable::set_serializing_sequence_for_instantiating()
{
	_serializing_sequence_void_pt_vector.clear();
	_get_serialized_function_vector.clear();
	_get_instantiated_function_vector.clear();
	add_serializing_sequence_for_instantiating();
}

void MWSerializable::add_serializing_sequence_for_instantiating()
{
	add_serializing_sequence();
}

void MWSerializable::add_serializing_sequence_data(MWSerializable* pt)
{
	_serializing_sequence_void_pt_vector.push_back(static_cast<void*>(pt));
	_get_serialized_function_vector.push_back(static_cast<MWSerializableFunctionGetVectorCharWithVoidPt>(& MWSerializable::get_serializable_serialized));
	_get_instantiated_function_vector.push_back(static_cast<MWSerializableFunctionGetSizeTAssignInstanceWithCharPtSizeT>(& MWSerializable::get_serializable_instantiated));
}

void MWSerializable::add_serializing_sequence_data(MWSerializableSingleton** pt)
{
	_serializing_sequence_void_pt_vector.push_back(static_cast<void*>(pt));
	_get_serialized_function_vector.push_back(static_cast<MWSerializableFunctionGetVectorCharWithVoidPt>(& MWSerializable::get_singleton_pt_serialized));
	_get_instantiated_function_vector.push_back(static_cast<MWSerializableFunctionGetSizeTAssignInstanceWithCharPtSizeT>(& MWSerializable::get_singleton_pt_instantiated));
}

void MWSerializable::add_serializing_sequence_data(MWData* pt)
{
	_serializing_sequence_void_pt_vector.push_back(static_cast<void*>(pt));
	_get_serialized_function_vector.push_back(static_cast<MWSerializableFunctionGetVectorCharWithVoidPt>(& MWSerializable::get_mwdata_serialized));
	_get_instantiated_function_vector.push_back(static_cast<MWSerializableFunctionGetSizeTAssignInstanceWithCharPtSizeT>(& MWSerializable::get_mwdata_instantiated));
}

void MWSerializable::add_serializing_sequence_data(size_t* pt)
{
	_serializing_sequence_void_pt_vector.push_back(static_cast<void*>(pt));
	_get_serialized_function_vector.push_back(static_cast<MWSerializableFunctionGetVectorCharWithVoidPt>(& MWSerializable::get_size_t_serialized));
	_get_instantiated_function_vector.push_back(static_cast<MWSerializableFunctionGetSizeTAssignInstanceWithCharPtSizeT>(& MWSerializable::get_size_t_instantiated));
}

void MWSerializable::add_serializing_sequence_data(char* pt)
{
	_serializing_sequence_void_pt_vector.push_back(static_cast<void*>(pt));
	_get_serialized_function_vector.push_back(static_cast<MWSerializableFunctionGetVectorCharWithVoidPt>(& MWSerializable::get_char_serialized));
	_get_instantiated_function_vector.push_back(static_cast<MWSerializableFunctionGetSizeTAssignInstanceWithCharPtSizeT>(& MWSerializable::get_char_instantiated));
}

vector<char> MWSerializable::get_serialized()
{
	set_serializing_sequence_for_serializing();

	size_t i, j;
	vector<char> ret, a_ret;
	for (i = 0; i < _serializing_sequence_void_pt_vector.size(); ++ i)
	{
		a_ret = (this->*(_get_serialized_function_vector[i]))(_serializing_sequence_void_pt_vector[i]);
		for (j = 0; j < a_ret.size(); ++ j)
		{
			ret.push_back(a_ret[j]);
		}
	}

	return ret;
}

vector<char> MWSerializable::get_serializable_serialized(const void* instance_pt) const
{
	return ((MWSerializable*) instance_pt)->get_serialized();
}

vector<char> MWSerializable::get_singleton_pt_serialized(const void* instance_pt) const
{
	return get_singleton_pt_serialized((MWSerializableSingleton**) instance_pt);
}

vector<char> MWSerializable::get_mwdata_serialized(const void* instance_pt) const
{
	return get_basic_serialized((MWData*) instance_pt);
}

vector<char> MWSerializable::get_size_t_serialized(const void* instance_pt) const
{
	return get_basic_serialized((size_t*) instance_pt);
}

vector<char> MWSerializable::get_char_serialized(const void* instance_pt) const
{
	return get_basic_serialized((char*) instance_pt);
}

size_t MWSerializable::get_instantiated(const vector<char> &char_vector, size_t off)
{
	set_serializing_sequence_for_instantiating();

	size_t i;
	const size_t original_off = off;
	for (i = 0; i < _serializing_sequence_void_pt_vector.size(); ++ i)
	{
		off += (this->*(_get_instantiated_function_vector[i]))(_serializing_sequence_void_pt_vector[i], char_vector, off);
	}

	return off - original_off;
}

size_t MWSerializable::get_serializable_instantiated(void* instance_pt, const vector<char> &char_vector, size_t off)
{
	return ((MWSerializable*) instance_pt)->get_instantiated(char_vector, off);
}

size_t MWSerializable::get_singleton_pt_instantiated(void* instance_pt, const vector<char> &char_vector, size_t off)
{
	return get_singleton_pt_instantiated((MWSerializableSingleton**) instance_pt, char_vector, off);
}

size_t MWSerializable::get_mwdata_instantiated(void* instance_pt, const vector<char> &char_vector, size_t off)
{
	return get_basic_instantiated((MWData*) instance_pt, char_vector, off);
}

size_t MWSerializable::get_size_t_instantiated(void* instance_pt, const vector<char> &char_vector, size_t off)
{
	return get_basic_instantiated((size_t*) instance_pt, char_vector, off);
}

size_t MWSerializable::get_char_instantiated(void* instance_pt, const vector<char> &char_vector, size_t off)
{
	return get_basic_instantiated((char*) instance_pt, char_vector, off);
}

vector<char> get_singleton_pt_serialized(const MWSerializableSingleton** instance_pt_pt)
{
	size_t len = sizeof(size_t), class_id = (* instance_pt_pt)->get_class_id();
	char chs[len];

	memcpy(chs, & class_id, len);

	vector<char> ret(len);
	for (size_t i = 0; i < len; ++ i)
	{
		ret[i] = chs[i];
	}

	return ret;
}

size_t get_singleton_pt_instantiated(MWSerializableSingleton** instance_pt_pt, const vector<char> &char_vector, size_t off)
{
	size_t len = sizeof(size_t), class_id;
	char chs[len];
	for (size_t i = 0; i < len; ++ i)
	{
		chs[i] = char_vector[off + i];
	}

	memcpy(& class_id, chs, len);

	* instance_pt_pt = MWSerializableSingleton::get_serializable_singleton_instance(class_id);

	return len;
}
