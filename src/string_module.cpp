#include "../include/string_module.h"
#if !defined(S_ISDIR) && !defined(S_ISREG) && defined(S_IFMT) && defined(S_IFREG) && defined(S_IFDIR)
#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#endif

std::vector<std::string> split(std::string input, char delimiter){
    std::vector<std::string> output;
    std::string::size_type pos = input.find(delimiter);
    while(pos!=std::string::npos){
        output.push_back(input.substr(0,pos));
        input.erase(0,pos+1);
        pos = input.find(delimiter);
        if(pos==std::string::npos) output.push_back(input);
    }

    return output.empty() ? std::vector<std::string>() : output;
}

std::string join(std::vector<std::string> list, char delimiter){
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

    return input;
}
static std::string ltrim(std::string input){
    input.erase(input.begin(), std::find_if(input.begin(),input.end(),[](unsigned char ch){
        return !std::isspace(ch);
    }));

    return input;
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