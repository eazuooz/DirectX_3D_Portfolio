#pragma once
#include <map>

class Template
{
public:

	template<typename T>
	static void MemZero(T& _Value) 
	{
		memset(&_Value, 0, sizeof(T));
	}

/*
	template<typename Key, typename Return>
	static Return MapFindToPointer(Key& _Key, std::map<Key, Return>& _Map)
	{
		std::map<Key, Return>::iterator FindIter
			= _Map.find(_Key);

		if (FindIter == _Map.end())
		{
			return nullptr;
		}

		return FindIter->second;
	}*/
/*
	template<typename Return, typename Key, typename Map>
	static Return* MapFindToPointer(Key& _Key, Map& _Map) 
	{
		Map::iterator FindIter
			= _Map.find(_Key);

		if (FindIter == _Map.end())
		{
			return nullptr;
		}

		return FindIter->second;
	}*/

private:
	Template();
	~Template();
};

