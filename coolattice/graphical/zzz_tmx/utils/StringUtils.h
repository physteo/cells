#pragma once

#include <vector>

namespace tmx {
	namespace utils {


		//! Returns a vector of strings, obtained splitting the input word using the input delimiter
		static std::vector<std::string> splitString(const std::string& word, const std::string& delimiter)
		{
			std::vector<std::string> result;

			size_t last = 0;
			size_t next = 0;
			while ((next = word.find(delimiter, last)) != std::string::npos) {
				result.push_back(word.substr(last, next - last));
				last = next + 1;
			}
			result.push_back(word.substr(last));
			return result;
		}


} }