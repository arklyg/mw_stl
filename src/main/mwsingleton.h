#ifndef _MW_SINGLETON_H_
#define _MW_SINGLETON_H_

template<typename T>
class MWSingleton
{
public:
	static T *get_instance();

protected:
	MWSingleton();
};

template<typename T>
T* MWSingleton<T>::get_instance()
{
	static T instance;
	return & instance;
}

template<typename T>
MWSingleton<T>::MWSingleton()
{
}

#endif
