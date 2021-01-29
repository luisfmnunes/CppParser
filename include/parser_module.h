#include "print_module.h"
#include "string_module.h"
#include <queue>
#include <unordered_map>
#include <set> 

#define DCPP_ADD_OPTION()
#define DCPP_ADD_FLAG()
#define DCPP_ADD_HELP()
#define DCPP_PARSE()

enum class dcppError{
    dcpp_OK = 0,
    dcpp_ALREADY_SET = 1,
    dcpp_WRONG_TYPE = 2,
    dcpp_NOT_EXIST = 3,
    dcpp_DOUBLE_BIND = 4,
    dcpp_REQUIRED_NOT_SET = 5,
};

class deCiPPher {
    public:

    private:
        //Local Types
        typedef std::unordered_map<std::string,bool> umapb;
        typedef std::unordered_map<std::string,int> umapi;
        typedef std::unordered_map<std::string,std::string> umaps;
        
        //Attributes
        std::queue<std::function<dcppError(std::string)> > parsing_lambdas;
        std::set<std::string> directives;
        umapb required;
        umapi arg_count;
        umaps var_names;

        template<class T> inline dcppError add_directive(std::string drt,T& ref,uint args,bool required,std::string name=""){
            if(directives.find(drt)!=directives.end())
                return dcpp_ALREADY_SET;
            if(var_names.find(name)!=var_names.end())
                return dcpp_DOUBLE_BIND;  
            parsing_lambdas.emplace([this,&ref,drt] -> dcppError (std::string value){
                if(is_type_numeric(ref) && !is_numeric(value)) return dcpp_WRONG_TYPE;
                std::stringstream string_parser;
                string_parser << value;
                string_parser >> ref;
                return dcpp_OK;
            }); 

            directives.emplace(drt);
            if(!name.empty()) var_names.emplace(drt,name);
        };

        std::string error_message (dcppError error, std::string var="");
};