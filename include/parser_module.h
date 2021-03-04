#ifndef PARSER_MODULE_H
#define PARSER_MODULE_H

#include "print_module.h"
#include "string_module.h"
#include <queue>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <functional>

#if !defined(uint)
    typedef unsigned int uint;
#endif

#define DCPP_ADD_OPTION(DCPP,DRT,DESC,REF,ARGS,REQ) DCPP.add_option(DRT,DESC,REF,ARGS,REQ,#REF);
#define DCPP_ADD_FLAG(DCPP,DRT,DESC,REF) DCPP.add_flag(DRT,DESC,REF,#REF);
#define DCPP_ADD_HELP(DCPP,DRT,DESC) DCPP.add_help(DRT,DESC);
#define DCPP_PARSE(DCPP,ARGC,ARGV) DCPP.parse_arguments(ARGC,ARGV)

enum class dcppError{
    OK = 0,
    ALREADY_SET = 1,
    WRONG_TYPE = 2,
    NOT_EXIST = 3,
    DOUBLE_BIND = 4,
    REQUIRED_NOT_SET = 5,
};

enum class dcppType{
    OPTION = 0,
    FLAG = 1,
    HELP = 2
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
            std::vector<std::string> drts = split(drt,',');
            if (drts.empty()) drts.push_back(drt);
            for(auto dir : drts){
                if(debug) os_debug("Adding directive",dir,"to deCiPPher parser.", name.empty() ? "" : (std::string("Binding to variable " + name)));
                add_directive(dir,description,ref,args,name);
                types.emplace(dir,dcppType::OPTION);
            }
            if(req) required.emplace(drt,true);
            drt_description.emplace(drt,description);
            return dcppError::OK;
        }
        
        //Methods
        dcppError add_flag(std::string drt, std::string description, bool &ref, std::string name = "");
        dcppError add_help(std::string drt, std::string description);
        void print_help();
        dcppError parse_arguments(int argc, char** argv);
        std::string error_message(dcppError error, std::string var="");
    private:
        //Local Types
        typedef std::unordered_map<std::string,bool> umap_b;
        typedef std::unordered_map<std::string,int> umap_i;
        typedef std::unordered_map<std::string,std::string> umap_s;
        typedef std::unordered_map<std::string,dcppType> umap_t;

        //Attributes
        bool debug;
        std::string description;
        std::string usage;
        std::unordered_map<std::string,std::function<dcppError(std::string)> > parsing_lambdas;
        std::set<std::string> directives;
        std::string help_drt;
        umap_t types;
        umap_b required;
        umap_i arg_count;
        umap_s var_names;
        umap_s drt_description;

        //Inline Methods
        template<class T> inline dcppError add_directive(std::string drt, std::string description,T& ref,uint args,std::string name=""){
            if(directives.find(drt)!=directives.end())
                return dcppError::ALREADY_SET;
            // if(var_names.find(name)!=var_names.end()) UNNECESSARY
            //     return dcpp_DOUBLE_BIND;  
            parsing_lambdas.emplace(drt,[this,&ref,drt,name](std::string value) -> dcppError {
                if(is_type_numeric(ref) && !is_numeric<typename std::remove_reference<decltype(ref)>::type>(value)) {os_debug("Value is",value); return dcppError::WRONG_TYPE;}
                if(!name.empty()) os_debug("Setting variable",name,"with value:",value);
                std::stringstream string_parser;
                string_parser << value;
                string_parser >> ref;
                return dcppError::OK;
            }); 

            directives.emplace(drt);
            arg_count.emplace(drt,args);
            if(!name.empty()) var_names.emplace(drt,name);
            if(!description.empty()){}
            return dcppError::OK;
        }

        // Methods
        
        //Getters and Setters
        bool get_debug();
        std::string get_description();
        std::string get_usage();
        std::set<std::string> get_directives();

        void set_debug(bool value);
        void set_description(std::string input);
        void set_usage(std::string input);

};

#endif