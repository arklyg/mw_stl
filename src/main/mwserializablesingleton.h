#ifndef _MW_STL_MW_POINTER_SERIALIZABLE_H_
#define _MW_STL_MW_POINTER_SERIALIZABLE_H_

#include <vector>

using namespace std;

class MWSerializableSingleton;

class MWSerializableSingleton
{
protected:
	static vector<MWSerializableSingleton*> _instance_pt_vector;

	size_t _class_id;

public:
	static MWSerializableSingleton* get_serializable_singleton_instance(size_t class_id);

	virtual ~MWSerializableSingleton();

	virtual void set_class_id(size_t class_id);
	virtual size_t get_class_id() const;
};

#endif
