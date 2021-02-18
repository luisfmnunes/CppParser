#include "testing.h"
#include "../deCiPPher.h"


bool test(int argc, char** argv){
    // variable to parse
    double fp = 0.0;

    // parser object instance
    deCiPPher parser(true);

    // Adds option to parser
    DCPP_ADD_OPTION(parser,"-d","A double number",fp,1,false);
    
    // Parse command line
    DCPP_PARSE(parser,argc,argv);

    DECIPPHER_ASSERT_EQUAL(fp, 0.0);

    DECIPPHER_TEST_END();

    return true;
}

int main (int argc, char** argv){

    DECIPPHER_TEST_BEGIN("Single Argument Parsing");

    if(!test(argc,argv)) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}