#pragma once

inline unsigned long StringHash(const unsigned char* str)
{
	// TODO: MessageBox(0, str, 0, 0);
	unsigned long hash = 5381;
	int c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + tolower(c); /* hash * 33 + c */

	return hash;
}

inline unsigned long StringHash(const char* str)
{
	return StringHash((const unsigned char*)str);
}

struct StringHashFunctor
{
	unsigned long operator()(const char* str) const
	{
		// use case-insensitive hash because user can enter the names in lower case from the script
		return StringHash(str);
	}
};

struct StringComparator
{
	bool operator()(const char* str1, const char* str2) const
	{
		// use case-insensitive compare because user can enter the names in lower case from the script
		return strcasecmp(str1, str2) == 0;
	}
};