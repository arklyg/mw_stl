#ifndef _MW_STL_TYPE_HELPERS_H_
#define _MW_STL_TYPE_HELPERS_H_

template <typename T, typename U>
struct SameType
{
	static const bool _is_same = false;
};

template <typename T>
struct SameType<T, T>
{
	static const bool _is_same = true;
};

template <typename T, typename U>
bool is_same_type(const T &t, const U &u)
{
	return SameType<T, U>::_is_same;
}

template <typename T>
bool is_basic_type(const T &t)
{
	return is_same_type(t, double(0)) || is_same_type(t, size_t(0));
}

#endif
