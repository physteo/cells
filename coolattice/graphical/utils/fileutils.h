#pragma once

#include <cstring>

namespace sparky {

    // function to read files
    class FileUtils{
    public:
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

}
