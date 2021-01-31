#include "../include/string_module.h"

std::vector<std::string> split(std::string input, char delimiter){
    std::vector<std::string> output;
    while(input.find(delimiter)!=std::string::npos){
        std::string::size_type pos = input.find(delimiter);
        output.push_back(input.substr(0,pos));
        input.erase(pos);
    }
    return output.empty() ? std::vector<std::string>() : output;
}

std::string join(std::vector<std::string> list, char delimiter = ' '){
    std::string output;
    for(auto str : list){
        output += str;
        if(str != list.back()) output+=delimiter;
    }
    return output;
}

static std::string rtrim(std::string input){
    input.erase(std::find_if(input.rbegin(),input.rend(), [](unsigned char ch){
            return !std::isspace(ch);
        }).base(), input.end());
}
static std::string ltrim(std::string input){
    input.erase(input.begin(), std::find_if(input.begin(),input.end(),[](unsigned char ch){
        return !std::isspace(ch);
    }));
}

std::string trim(std::string input){
    input = rtrim(input);
    input = ltrim(input);
    return input;
}

//Check conditions
bool is_dir(std::string path){
    struct stat s;
    if(stat(path.c_str(),&s) == 0) return false;
    return (static_cast<bool>(S_ISDIR(s.st_mode)));
}

bool is_file(std::string path){
    struct stat s;
    if(stat(path.c_str(),&s)==0) return false;
    return (static_cast<bool>(S_ISREG(s.st_mode)));
}