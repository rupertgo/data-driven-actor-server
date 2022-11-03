#include "NameDictionary.h"


NameDictionary::NameDictionary()
{
	_arraySize = sizeof(data) / sizeof(keyValuePair);
};

String NameDictionary::get(String key)
{
	for (int i = 0; i < _arraySize; i++)
	{
		if (key == data[i].key)
		{
			return data[i].value;
		}
	}

	return key;
};

int NameDictionary::length()
{
	return _arraySize;
};

void NameDictionary::values()
{
	for (int i = 0; i < _arraySize; i++)
	{
		Serial.println(data[i].value);
	}
};

void NameDictionary::list()
{
	for (int i = 0; i < _arraySize; i++)
	{
		Serial.println(data[i].key + " : " + data[i].value);
	}
};

void NameDictionary::keys()
{
	for (int i = 0; i < _arraySize; i++)
	{
		Serial.println(data[i].key);
	}
};
