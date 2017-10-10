#pragma once
#include <string>
#include <iostream>

// function to read files
class Miscellaneous {
public:
	// convert from a string into an unsigned int (which is a size_t). Taken from https://stackoverflow.com/a/8715855/4746978
	static unsigned stou(std::string const & str, size_t * idx = 0, int base = 10) {
		unsigned long result;
		try {
			result = std::stoul(str, idx, base);
			if (result > std::numeric_limits<unsigned>::max()) {
				throw std::out_of_range("stou");
			}
		}
		catch (std::invalid_argument& err)
		{
			throw err;
		}
		return result;
	}

	static std::string read_file(const char* filepath)
	{
		FILE* file = fopen(filepath, "rt");
		//how long is the file? use fseek and ftell
		fseek(file, 0, SEEK_END);
		// ftell gives the number of bytes (which is the same as the number of chars, since 1char = 1byte)
		unsigned long length = ftell(file);
		// create the string, adding the terminating character
		char* data = new char[length + 1];
		// this is needed to not have extra rubbish bytes
		memset(data, 0, length + 1);
		// go back to the beginning of file
		fseek(file, 0, SEEK_SET);
		// read stuff and put it inside data
		fread(data, 1, length, file);
		// construct the string from the array of chars
		std::string result(data);
		delete[] data;
		return result;
	}



};
