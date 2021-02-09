#include "include/print_module.h"
#include "include/parser_module.h"

using namespace std;

void say_hello(){
    cout << "Hello, from CPParser!\n";
    os_log("this");
    os_warn("that");
    os_error("THIS", 3.1415);
    os_debug("THAT",42);
    _log("C this %s","GERONIMO");
    _warn("C that");
    _error("C THIS");
    _debug("C THAT %d",42);
}

int main(int argc, char** argv){
    int number = 0;
    bool flag = false;
    say_hello();

    deCiPPher parser(true);

    DCPP_ADD_OPTION(parser,"-n","aaa",number,1,false);
    DCPP_ADD_FLAG(parser,"-f","bbb",flag);
    DCPP_ADD_HELP(parser,"-h","A Debug example to test deCiPPher");
    DCPP_PARSE(parser,argc,argv);

    os_log("Value of number after parser =",number);
    os_log("Value of flag after parser =", flag ? "true" : "false");

    return EXIT_SUCCESS;
}