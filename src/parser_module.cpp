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
        types.emplace(dir,dcppType::FLAG);
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
            if(debug) os_debug("Adding help directive",drt,"to deCiPPher parser. Use to print help description.");  
            parsing_lambdas.emplace(d, [this,d] (std::string blank) -> dcppError {
                print_help();
                return dcppError::OK;
            }); 

            directives.emplace(d);
            required.emplace(d,false);
            arg_count.emplace(d,0);
            types.emplace(d,dcppType::HELP);
        
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

//TODO Treat required arguments (return if required not set), in Options, check if next argumment is another directive and warn possible error
dcppError deCiPPher::parse_arguments(int argc, char** argv){
    dcppError err;
    std::vector<std::string> arguments;
    for(int i = 1; i < argc; i++){
        arguments.push_back(argv[i]);
    }

    for(size_t i = 0; i < arguments.size(); i++){
        if(arguments[i].find('=')!=std::string::npos){ //If argument presents = statement
            std::vector<std::string> splitter = split(arguments[i],'=');
            if(directives.find(splitter.front())==directives.end()){
                os_warn("Directive",splitter.front(),"passed but not set. Checking if contains concatenated keys.");
                int _args = 0;
                for(char ch : splitter.front()){
                    if(ch=='-') continue;
                    std::string tmp_d = splitter.front().front()=='-' ? "-"+ch : ""+ch;
                    if(directives.find(tmp_d)!=directives.end()) _args+=arg_count[tmp_d];
                    else os_warn("Directive",tmp_d,"passed but not set in parser");
                }               
                if (_args!=1){
                    os_warn("Value '",splitter.back(),"' passed but a single key requires multiple arguments or multiple keys require at least 1 argument.");
                } else { //solves arguments
                    for(char ch : splitter.front()){
                        if(ch == '-') continue;
                        std::string tmp_d = splitter.front().front()=='-' ? "-"+ch : ""+ch;
                        if(directives.find(tmp_d)!=directives.end()){
                            switch (types[tmp_d]){
                            case dcppType::OPTION:
                                err = parsing_lambdas[tmp_d](splitter.back());
                                break;
                            case dcppType::FLAG:
                                err = parsing_lambdas[tmp_d]("1");
                                break;
                            case dcppType::HELP:
                                err = parsing_lambdas[tmp_d]("");
                                break;
                            }
                        }
                        if (err != dcppError::OK) os_warn(error_message(err,tmp_d));
                    }
                }
                continue;
            } else {
                if(types[splitter.front()]==dcppType::OPTION)
                    err = parsing_lambdas[splitter.front()](splitter.back()); //For now, but later check what to do if splitter size > 2
                else{
                    err = parsing_lambdas[splitter.front()]("1");
                    os_warn("Unnecessary use of = to set argument of either flag or help directive.");
                }
                if(err != dcppError::OK){
                    os_warn(error_message(err,splitter.front()));
                    continue;
                }
                continue;
            }
            
        } else{ //If argument doesn't present = statement
            std::string drt = arguments[i];
            if(directives.find(drt)!=directives.end()){ //exists
                switch (types[drt]){
                    case dcppType::OPTION:
                    {
                        uint arg_c = arg_count[drt];
                        if(i+arg_c >= argc){
                            os_error("Directive",drt,"expected",arg_c,"arguments, but only", argc-(i+arg_c),"arguments are available");
                            continue;
                        }
                    }
                    err = parsing_lambdas[drt](arguments[++i]); //for now only 1 argument is accepted
                    break;

                    case dcppType::FLAG:
                       err = parsing_lambdas[drt]("1");
                    break;

                    case dcppType::HELP:
                        err = parsing_lambdas[drt]("");
                    break;
                }

                if(err != dcppError::OK){
                    os_warn(error_message(err,drt));
                    continue;
                }
            } else{ //Doens't exist checking for chained keys
                os_warn("Directive",drt,"passed but not set. Checking if contains concatenated keys");
                int _args = 0; //number of arguments already consumed by options
                for(char ch : drt){
                    if(ch == '-') continue;
                    std::string tmp_d = drt.front()=='-' ? "-"+ch : ""+ch;
                    if(directives.find(tmp_d)!=directives.end()){
                            switch (types[tmp_d]){
                            case dcppType::OPTION:
                                if(_args+i+arg_count[tmp_d] >= argc){
                                    os_error("Directive",drt,"expected",arg_count[tmp_d],"arguments, but only", argc-(i+arg_count[tmp_d]+_args),"arguments are available");
                                    continue;
                                }
                                err = parsing_lambdas[tmp_d](arguments[++i]); //allow only 1 argument for now
                                _args++; // assume only 1 argument for now
                                break;
                            case dcppType::FLAG:
                                err = parsing_lambdas[tmp_d]("1");
                                break;
                            case dcppType::HELP:
                                err = parsing_lambdas[tmp_d]("");
                                break;
                        }

                        if(err != dcppError::OK){
                            os_warn(error_message(err,drt));
                            continue;
                        }
                    } else os_warn("Directive",tmp_d,"passed but not set in parser");
                }
            } 
        }

//PARSER in case of 2+ arguments, joined string result will be passed, so the class operator >> MUST be implemented or result is undefined

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