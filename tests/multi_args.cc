#include "testing.h"
#include "../deCiPPher.h"


bool test(int argc, char** argv){
    // variable to parse
    int num = 0;
    double fp = 0.0;
    bool flag = false;

    // parser object instance
    deCiPPher parser(true);

    // Adds option to parser
    DCPP_ADD_OPTION(parser,"-n","An int number",num,1,false);
    DCPP_ADD_OPTION(parser,"-d","A double number",fp,1,false);
    DCPP_ADD_FLAG(parser,"-f","A flag",flag);
    
    // Parse command line
    DCPP_PARSE(parser,argc,argv);

    DECIPPHER_ASSERT_EQUAL(fp, 3.14159265);
    DECIPPHER_ASSERT_EQUAL(num, 10);
    DECIPPHER_ASSERT_THROW(flag);

    DECIPPHER_TEST_END();

    return true;
}

int main (int argc, char** argv){

    DECIPPHER_TEST_BEGIN("Single Argument Parsing");

    if(!test(argc,argv)) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}