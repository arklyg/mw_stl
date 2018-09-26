#include <mwlogger.h>

#include "mwserializablesingleton.h"

vector<MWSerializableSingleton*> MWSerializableSingleton::_instance_pt_vector;

MWSerializableSingleton::~MWSerializableSingleton()
{
}

MWSerializableSingleton* MWSerializableSingleton::get_serializable_singleton_instance(size_t class_id)
{
	if (class_id < _instance_pt_vector.size())
	{
		return _instance_pt_vector[class_id];
	}

	LOG_ERROR(_logger, "class_id = " << class_id << ", _instance_pt_vector.size() = " << _instance_pt_vector.size() << ", no such class. NULL returned");
	return NULL;
}

void MWSerializableSingleton::set_class_id(size_t class_id)
{
	_class_id = class_id;
	if (_instance_pt_vector.size() < _class_id + 1)
	{
		_instance_pt_vector.resize(_class_id + 1, NULL);
	}
	if (_instance_pt_vector[_class_id] != NULL)
	{
		LOG_ERROR(_logger, "_instance_pt_vector[" << _class_id << "] is already taken, but this set to that eventually.");
	}

	_instance_pt_vector[_class_id] = this;
}

size_t MWSerializableSingleton::get_class_id() const
{
	return _class_id;
}
