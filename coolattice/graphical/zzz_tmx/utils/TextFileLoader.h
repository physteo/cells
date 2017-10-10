#pragma once

#include <vector>
#include <string>
#include <fstream> 
#include <sstream>
#include <iterator>

namespace tmx { namespace utils {


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
		std::cout << "size of file: " << result.size() << std::endl;
		delete[] data;
		return result;
	}


	//! Loads file into a vector, where the elements are the lines of the file
	//! and each line is a vector of strings, each string is a word, obtained
	//! by splitting the line at spaces. Excludes commented (#) lines.
	static std::vector<std::vector<std::string> > load_text_file(const std::string& path)
	{
		std::vector<std::vector<std::string> > lines;

		std::ifstream ifs(path);
		if (!ifs)
		{
			throw std::runtime_error("Error opening file.");
		}
		// vector of lines, each line is a vector of strings
		std::string line;
		while (std::getline(ifs, line)) {
			// stop if the file ends
			if (!ifs)
				break;
			// ignore commented lines
			if (line[0] == '#')
				continue;
			std::vector<std::string> row;
			std::istringstream iss(line);
			std::copy(std::istream_iterator<std::string>(iss),
				std::istream_iterator<std::string>(),
				std::back_inserter(row));
			lines.push_back(row);
		}

		return lines;

	}


} }