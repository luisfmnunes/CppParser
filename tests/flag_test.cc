#include "testing.h"
#include "../deCiPPher.h"


bool test(int argc, char** argv){
    // variable to parse
    bool flag = false;

    // parser object instance
    deCiPPher parser(true);

    // Adds option to parser
    DCPP_ADD_FLAG(parser,"-f,--flag","A flag",flag);
    
    // Parse command line
    DCPP_PARSE(parser,argc,argv);

    DECIPPHER_ASSERT_EQUAL(flag, true);

    DECIPPHER_TEST_END();

    return true;
}

int main (int argc, char** argv){

    DECIPPHER_TEST_BEGIN("Single Argument Parsing");

    if(!test(argc,argv)) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}