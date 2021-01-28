#include "include/print_module.h"

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

int main(){
    say_hello();
}