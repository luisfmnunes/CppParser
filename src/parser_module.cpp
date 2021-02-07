#include "../include/parser_module.h"

// Constructors
// Blank Constructor
deCiPPher::deCiPPher() : debug{false} {}
// Descriptor Constructor
deCiPPher::deCiPPher(std::string description) : debug{false}{
    this->description = description;
}
// Descriptor and Usage Constructor
deCiPPher::deCiPPher(std::string description, std::string usage) : debug{false}{
    this->description = description;
    this->usage = usage;
}
// Full Constructor
deCiPPher::deCiPPher(std::string description, std::string usage, bool debug){
    this->description = description;
    this->usage = usage;
    this->debug = debug;
}
// Debug Constructor
deCiPPher::deCiPPher(bool debug) : debug{debug}{}

//Adds flag setting directives
dcppError deCiPPher::add_flag(std::string drt, std::string description, bool &ref, std::string name){
    std::vector<std::string> directives = split(drt,',');
    if (directives.empty()) directives.push_back(drt);
    for(auto dir : directives){
        if(debug) os_debug("Adding directive",drt,"to deCiPPher parser.", name.empty() ? "" : (std::string("Binding to variable " + name)));
        add_directive(dir,description,ref,0,false,name);
    }

    drt_description.emplace(drt,description);

    return dcppError::OK;
}

//Adds help printing directives
dcppError deCiPPher::add_help(std::string drt, std::string description){
    std::vector<std::string> directives_arg = split(drt,',');
    if (directives_arg.empty()) directives_arg.push_back(drt);
    for(auto d : directives_arg){
            if(directives.find(d)!=directives.end())
                return dcppError::ALREADY_SET;
            // if(var_names.find(name)!=var_names.end()) UNNECESSARY
            //     return dcpp_DOUBLE_BIND;  
            parsing_lambdas.emplace(d, [this,d] (std::string blank="") -> dcppError {
                print_help();
                return dcppError::OK;
            }); 

            directives.emplace(d);
            required.emplace(d,false);
            arg_count.emplace(d,0);
    }

    return dcppError::OK;
}

//Prints description, usage, and directives description;
void deCiPPher::print_help(){
    fprintf(stderr,"%sDESCRIPTION%s\n\t%s\n",BOLD,CLOSE_BOLD,description.c_str());
    fprintf(stderr,"%sDIRECTIVES%s\n",BOLD,CLOSE_BOLD);
    for(auto pair : drt_description){
        fprintf(stderr,"\t%s%s%s\n\t\t%s\n",BOLD,pair.first.c_str(),CLOSE_BOLD,pair.second.c_str());
    }
    fprintf(stderr,"%sUSAGE%s\n\t%s\n",BOLD,CLOSE_BOLD,usage.c_str());
}

dcppError deCiPPher::parse_arguments(int argc, char** argv){
    dcppError err;
    std::vector<std::string> arguments;
    for(int i = 0; i < argc; i++){
        arguments.push_back(argv[i]);
    }

    for(size_t i = 0; i < arguments.size(); i++){
        if(arguments[i].find('=')){ //If argument presents = statement
            std::vector<std::string> splitter = split(arguments[i]);
            if(directives.find(splitter.front())==directives.end()){
                os_warn("Directive",splitter.front(),"passed but not set. Checking if contains concatenated keys.");
                int _args = 0;
                if(splitter.front().front()=='-'){
                    for(char ch : splitter.front()){
                        std::string tmp_d = "-"+ch;
                        if(directives.find(tmp_d)!=directives.end()) _args+=arg_count[tmp_d];
                        else os_warn("Directive",tmp_d,"passed but not set in parser");
                    }
                }
                else{
                    for(char ch : splitter.front()){
                        std::string tmp_d (ch,1);
                        if(directives.find(tmp_d)!=directives.end()) _args+=arg_count[tmp_d];
                        else os_warn("Directive",ch,"passed but not set in parser");
                    }
                }
                if (_args!=1){
                    os_warn("Value",splitter.back(),"passed but multiple keys require at least 1 argument.");
                }
                continue;
            } else {
                err = parsing_lambdas[splitter.front()](splitter.back()); //For now, but later check what to do if splitter size > 2
                if(err != dcppError::OK){
                    os_warn(error_message(err,splitter.front()));
                    continue;
                }
            }
            //TODO concatenated keys with = - check args count, if > 1, pop error;
        } else{ //If argument doesn't present = statement
            if(directives.find(arguments[i])!=directives.end()){ //exists

            } else if(true) { //Do char by char checking

            } // else check cases started with '-'
        }
        //if(argument.front()=='-' && argument[1]!='-'){ //Treat cases of multiple directives (keys) stacked.
        //Check if contains =   
            //Check if whole word is a key
                //if TRUE see if it has spaced arguments and increment i (warn arguments equals to keys to warn user of possible input errors)
            //Else treat each char as key
                // Warn when key is not set
                // in case of = return error if argcount > 1 

//PARSER in case of 2+ arguments, joined string result will be passed, so the class operator >> MUST be implemented or result is undefined

        // }
    }

    return dcppError::OK;
}

//Returns an error message of a dcppError
std::string deCiPPher::error_message(dcppError error, std::string var){
    switch (error)
    {
    case dcppError::OK:
        return "OK Status";
        break;
    case dcppError::ALREADY_SET:
        return "Directive already set and binded to variable";
        break;
    case dcppError::WRONG_TYPE:
        return "Value passed to numeric variable " + var + (var.empty() ? "" : " ") + "is not numeric";
        break;
    case dcppError::NOT_EXIST:
        return "Directive passed isn't binded to a variable";
        break;
    case dcppError::DOUBLE_BIND:
        return "Variable " + var + (var.empty() ? "" : " ") + "is already binded to another directive";
        break;
    case dcppError::REQUIRED_NOT_SET:
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