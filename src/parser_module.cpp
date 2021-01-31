#include "../include/parser_module.h"

//Adds flag setting directives
dcppError deCiPPher::add_flag(std::string drt, std::string description, bool &ref, std::string name = ""){
    std::vector<std::string> directives = split(drt,',');
    if (directives.empty()) directives.push_back(drt);
    for(auto dir : directives){
        if(debug) os_debug("Adding directive",drt,"to deCiPPher parser.", name.empty() ? "" : (std::string("Binding to variable " + name)));
        add_directive(dir,description,ref,0,false,name);
    }

    drt_description.emplace(drt,description);
}

//Adds help printing directives
dcppError deCiPPher::add_help(std::string drt, std::string description){
    std::vector<std::string> directives_arg = split(drt,',');
    if (directives_arg.empty()) directives_arg.push_back(drt);
    for(auto d : directives_arg){
            if(directives.find(d)!=directives.end())
                return dcppError::dcpp_ALREADY_SET;
            // if(var_names.find(name)!=var_names.end()) UNNECESSARY
            //     return dcpp_DOUBLE_BIND;  
            parsing_lambdas.emplace([this,d] (std::string blank=""){
                print_help();
                return dcppError::dcpp_OK;
            }); 

            directives.emplace(d);
            required.emplace(d,false);
            arg_count.emplace(d,0);
    }
}

//Prints description, usage, and directives description;
void deCiPPher::print_help(){
    fprintf(stderr,"%sDESCRIPTION%s\n\t%s\n",BOLD,CLOSE_BOLD,description);
    fprintf(stderr,"%sDIRECTIVES%s\n",BOLD,CLOSE_BOLD);
    for(auto pair : drt_description){
        fprintf(stderr,"\t%s%s%s\n\t\t%s\n",BOLD,pair.first,CLOSE_BOLD,pair.second);
    }
    fprintf(stderr,"%sUSAGE%s\n\t%s\n",BOLD,CLOSE_BOLD,usage);
}

//Returns an error message of a dcppError
std::string deCiPPher::error_message (dcppError error, std::string var=""){
    switch (error)
    {
    case dcppError::dcpp_OK:
        return "OK Status";
        break;
    case dcppError::dcpp_ALREADY_SET:
        return "Directive already set and binded to variable";
        break;
    case dcppError::dcpp_WRONG_TYPE:
        return "Value passed to numeric variable " + var + (var.empty() ? "" : " ") + "is not numeric";
        break;
    case dcppError::dcpp_NOT_EXIST:
        return "Directive passed isn't binded to a variable";
        break;
    case dcppError::dcpp_DOUBLE_BIND:
        return "Variable " + var + (var.empty() ? "" : " ") + "is already binded to another directive";
        break;
    case dcppError::dcpp_REQUIRED_NOT_SET:
        return "Variable " + var + (var.empty() ? "" : " ") + "set as required isn't initialized";
        break;

    default:
        return "Unkown Parsing Error";
        break;
    }
}

bool deCiPPher::get_debug(){
    return this->debug;
}
std::string deCiPPher::get_description(){
    return this->description;
}
std::string deCiPPher::get_usage(){
    return this->usage;
}
std::set<std::string> deCiPPher::get_directives(){
    return this->directives;
}

void deCiPPher::set_debug(bool value){
    this->debug = value;
}
void deCiPPher::set_description(std::string input){
    this->description = input;
}
void deCiPPher::set_usage(std::string input){
    this->usage = input;
}