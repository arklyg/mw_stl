#include <string.h>

#include "mwcharhelpers.h"

size_t char_array_len(const char* char_array)
{
	size_t len(0);
	while (char_array[len] != '\0')
	{
		++ len;
	}

	return len;
}

char* char_array_copy(const char* char_array)
{
	size_t char_array_size(char_array_len(char_array) + 1);

	return (char*) memcpy(new char[char_array_size], char_array, char_array_size);
}
