#include "print_module.h"
#include "string_module.h"
#include <queue>
#include <unordered_map>
#include <set> 

#define DCPP_ADD_OPTION(DCPP,DRT,DESC,REF,ARGS,REQ,NAME) DCPP.add_option(DRT,DESC,REF,ARGS,REQ,#NAME);
#define DCPP_ADD_FLAG(DCPP,DRT,DESC,REF,NAME) DCPP.add_flag(DRT,DESC,REF,#NAME);
#define DCPP_ADD_HELP(DCPP,DRT,DESC) DCPP.add_help(DRT,DESC);
#define DCPP_PARSE(DCPP,ARGC,ARGV);

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
        //Constructors
        deCiPPher();
        deCiPPher(std::string description);
        deCiPPher(std::string description, std::string usage);
        deCiPPher(std::string description, std::string usage, bool debug);
        deCiPPher(bool debug);

        //Inline Methods
        template<class T> inline dcppError add_option(std::string drt, std::string description, T& ref, uint args, bool req, std::string name = ""){
            std::vector<std::string> directives = split(drt,',');
            if (directives.empty()) directives.push_back(drt);
            for(auto dir : directives){
                if(debug) os_debug("Adding directive",drt,"to deCiPPher parser.", name.empty() ? "" : (std::string("Binding to variable " + name)));
                add_directive(dir,description,ref,args,req,name);
            }

            drt_description.emplace(drt,description);
        }
        
        //Methods
        dcppError add_flag(std::string drt, std::string description, bool &ref, std::string name = "");
        dcppError add_help(std::string drt, std::string description);
        void print_help();
    private:
        //Local Types
        typedef std::unordered_map<std::string,bool> umapb;
        typedef std::unordered_map<std::string,int> umapi;
        typedef std::unordered_map<std::string,std::string> umaps;
        
        //Attributes
        bool debug;
        std::string description;
        std::string usage;
        std::queue<std::function<dcppError(std::string)> > parsing_lambdas;
        std::set<std::string> directives;
        std::string help_drt;
        umapb required;
        umapi arg_count;
        umaps var_names;
        umaps drt_description;

        //Inline Methods
        template<class T> inline dcppError add_directive(std::string drt, std::string description,T& ref,uint args,bool req,std::string name=""){
            if(directives.find(drt)!=directives.end())
                return dcpp_ALREADY_SET;
            // if(var_names.find(name)!=var_names.end()) UNNECESSARY
            //     return dcpp_DOUBLE_BIND;  
            parsing_lambdas.emplace([this,&ref,drt,name] -> dcppError (std::string value){
                if(is_type_numeric(ref) && !is_numeric(value)) return dcpp_WRONG_TYPE;
                std::stringstream string_parser;
                string_parser << value;
                string_parser >> ref;
                return dcpp_OK;
            }); 

            directives.emplace(drt);
            required.emplace(drt,req);
            arg_count.emplace(drt,args);
            if(!name.empty()) var_names.emplace(drt,name);
        };

        // Methods
        std::string error_message (dcppError error, std::string var="");

        //Getters and Setters
        bool get_debug();
        std::string get_description();
        std::string get_usage();
        std::set<std::string> get_directives();

        void set_debug(bool value);
        void set_description(std::string input);
        void set_usage(std::string input);

};