#ifndef NAMEDICTIONARY_h
#define NAMEDICTIONARY_h

#include <Arduino.h>

/** \file NameDictionary.h
    \brief Block auto naming functionality.

    This file includes an of esp8266 chip IDs paired with 3 letter human
		readable names. Blocks not on the list should print their chip ID when
		booting up any of the example sketches so you can add them to the list.
*/

/** \struct keyValuePair
    \brief A structure to hold key:value paired data, similar to a python dictionary.
*/
typedef struct
{
	String key;
	String value;
} keyValuePair;

/** \class NameDictionary
    \brief Storage and functionality for block naming.

    This class contains an array of chip ID to name mappings and a number of
		functions to help blocks auto asign themselves a name.
*/
class NameDictionary
{
	keyValuePair data[50] = {
		{"c2c373", "abe"},
		{"c31d9d", "aja"},
		{"c2b603", "ace"},
		{"da58f5", "ali"},
		{"da6195", "alf"},
		{"da50d8", "amy"},
		{"da5649", "ann"},
		{"c2b2d6", "art"},
		{"da516d", "aya"},
		{"c2a597", "bea"},
		{"da5331", "bev"},
		{"c2c41d" ,"bob"},
		{"c318a1", "bud"},
		{"da5a56", "deb"},
		{"da4a70", "cal"},
		{"c2c5c2", "cam"},
		{"c2a5e6", "che"},
		{"c2a23f", "dot"},
		{"c2c415", "dan"},
		{"c2bf2a", "dax"},
		{"da4ee4", "fin"},
		{"c2af18", "jim"},
		{"1a6b16", "joe"},
		{"8810e9", "kat"},
		{"31054b", "kev"},
		{"30ce48", "sam"},
		{"8e1ac" , "eve"},
		{"c2a773", "ben"},
		{"da613c", "lou"},
		{"c2c047", "tim"},
		{"30ce96", "joy"},
	};

	int _arraySize;

public:
	/**
		 @brief Class constructor.
	 */
	NameDictionary();

	/**
	   @brief Returns the value associated to a key. If no key value pair is found, the search term is returned.
	 */
	String get(String key);

	/**
	   @brief Prints all the key value pairs to the serial.
	 */
	void list();

	/**
	   @brief Prints all the values to the serial.
	 */
	void values();

	/**
	   @brief Prints all the keys to the serial.
	 */
	void keys();

	/**
	   @brief Returns the number of key value pairs in the dictionary.
	 */
	int length();
};

#endif
