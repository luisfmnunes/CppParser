#include "testing.h"
#include "../deCiPPher.h"

bool test(int argc, char** argv){

    deCiPPher parser(true);
    std::string required;
    std::string not_req;

    DCPP_ADD_OPTION(parser,"-r,--required","A required argument",required,1,true);
    DCPP_ADD_OPTION(parser,"-n,--not","A not required argument",not_req,1,false);
    if(DCPP_PARSE(parser,argc,argv)!=dcppError::OK) return false;

    return true;
}

int main(int argc, char** argv){

    if(!test(argc,argv)) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}