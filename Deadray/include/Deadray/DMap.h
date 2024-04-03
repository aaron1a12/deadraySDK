#pragma once

#include <vector>
template<typename T>
struct DMapPair {
	DMapPair(const char* _key, T t) {Key = _key; Value = t;}
	const char* Key;
	T Value;
};

template<typename Key, typename Value>
class DMap {
private:
	// elements are hashed into buckets
	// this how many buckets 
	unsigned int bucketCount;

	std::vector< std::vector<DMapPair<Value>> > buckets;

	// number of elements
	unsigned int count;

	char* GetStrVal(const char* key, bool& bShouldFree)
	{
		bShouldFree = false;
		return (char*)key;
	}

	char* GetStrVal(uint32 key,  bool& bShouldFree)
	{
		bShouldFree = true;

		// TODO: There is currently no management of this memory
		char* str = new char[16];
		sprintf(str, "%u", key);

		return str;
	}

	int HashKey(const char* key)
	{
		int sum = 0;

		while(key[0] != '\0')
		{
			sum += static_cast<int>(key[0]);
			++key;
		}

		return sum % bucketCount;
	}

public:

	DMap()
	{
		count = 0;

		// default buckets
		Reserve(10);
	}

	// Sets initial buckets
	void Reserve(unsigned int count)
	{
		// Set bucket count to be 125% of the element count plus 1
		//bucketCount = ceil(count * 1.25);
		bucketCount = 10;

		buckets.reserve(bucketCount);
		buckets.resize(bucketCount);

		//for(int i=0; i < buckets.size(); i++)
		{
			//buckets[i].reserve(1);
		}
	}

	unsigned int Size()
	{
		return count;
	}

	void Insert(Key key, Value value)
	{
		bool bShouldFree;


		// TODO: re-hash all if we're filled to 80%
		char* strVal = GetStrVal(key, bShouldFree);

		unsigned int index = HashKey(strVal);

		// TODO: First check if it's already there!
		buckets[index].push_back(DMapPair<Value>(strVal, value));
		count++;

		if (bShouldFree)
		{
			// Push back is only saving a pointer
			// Deleting here would cause the pointer to then point nowhere
			// delete[] strVal;
		}
	}

	Value* Find(Key key)
	{
		bool bShouldFree;
		const char* strVal = GetStrVal(key, bShouldFree);
		unsigned int index = HashKey(strVal);

		std::vector<DMapPair<Value>>& bucket = buckets[index];

		// Search the buckets
		for(unsigned int i=0; i < bucket.size(); i++)
		{
			// compare the keys
			if (strcmp(strVal, bucket[i].Key)==0)
			{
				if (bShouldFree)
					delete[] strVal;

				return &bucket[i].Value;
			}
		}

		return (Value*)0;
	}
};