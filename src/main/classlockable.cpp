#include <iostream>
#include <typeinfo>

#include "classlockable.h"

using namespace std;

pthread_mutex_t ClassLockable::_map_mutex = PTHREAD_MUTEX_INITIALIZER;
map<string, pthread_mutex_t*> ClassLockable::_mutex_map;

void ClassLockable::acquire()
{
	pthread_mutex_lock(& _map_mutex); // 对mutex_map的改动或取值保持互斥
	pthread_mutex_t* mutex = get_mutex();
	if (mutex == NULL)
	{
		mutex = add_mutex();
	}
	pthread_mutex_unlock(& _map_mutex); // 结束互斥

	pthread_mutex_lock(mutex);
}

void ClassLockable::release()
{
	pthread_mutex_lock(& _map_mutex); // 对mutex_map的改动或取值保持互斥
	pthread_mutex_t* mutex = get_mutex();
	pthread_mutex_unlock(& _map_mutex); // 结束互斥

	if (mutex != NULL)
	{
		pthread_mutex_unlock(mutex);
	}
}

pthread_mutex_t* ClassLockable::get_mutex()
{
	string class_name = typeid(* this).name();

	map<string, pthread_mutex_t* >::iterator finder = _mutex_map.find(class_name);
	return finder == _mutex_map.end() ? NULL : finder->second;
}

pthread_mutex_t* ClassLockable::add_mutex()
{
	 pthread_mutex_t* mutex = new pthread_mutex_t;
	 pthread_mutex_init(mutex, NULL);
	 _mutex_map.insert(pair<string, pthread_mutex_t*>(typeid(* this).name(), mutex));
	 return mutex;
}
