#ifndef DECIPPHER_TEST_H
#define DECIPPHER_TEST_H

#include<iostream>
#include<stdexcept>
#include<string>

#ifndef _WIN32
#define __FUNC_MACRO__ __PRETTY_FUNCTION__
#else
#define __FUNC_MACRO__ __FUNCSIG__
#endif


#define DECIPPHER_ASSERT_THROW(condition) {\
    if(!(condition)){ \
        std::cerr << std::string(__FILE__) \
                                  + std::string(":") \
                                  + std::to_string(__LINE__) \
                                  + std::string(" in ") \
                                  + std::string( __FUNC_MACRO__ ) \
        std::endl;\
        return false;\
    }\
}

#define DECIPPHER_ASSERT_EQUAL( x, y ) { \
    if( ( x ) != ( y ) ){\
        std::cerr << std::string( __FILE__ ) \
                                  + std::string(":") \
                                  + std::to_string(__LINE__) \
                                  + std::string(" in ") \
                                  + std::string( __FUNC_MACRO__ ) \
                                  + std::string(": ") \
                                  + std::string((#x)) \
                                  + std::string(" = ")\
                                  + std::to_string( ( x ) ) \
                                  + std::string(" != ") \
                                  + std::to_string( ( y ) ) \
        << std::endl;\
        return false;\
    }\
}

#define DECIPPHER_TEST_BEGIN( name ) \
    {\
        std::cerr << "--Beginning Test \"" << name << "\"..." << std::endl;\
    }

#define DECIPPHER_TEST_END() \
{\
    std::cerr << "finished" << std::endl;\
}

#endif