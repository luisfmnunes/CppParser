cmake_minimum_required(VERSION 3.0.0)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

file(GLOB sources ../src/*.cpp)

# Command-Line Arguments
set(args_t1 -n 10)
set(args_t2 -n=10)
set(args_t3 -n=1.0)
set(args_t4 -n fail) 

# Adding Executables
add_executable( int_test int_test.cc ${sources})
add_executable( int_fail int_fail.cc ${sources})

# Adding Tests
add_test( test_1 int_test ${args_t1})
add_test( test_2 int_test ${args_t2})
add_test( test_3 int_fail ${args_t3})
add_test( test_4 int_fail ${args_t4})