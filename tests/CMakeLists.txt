cmake_minimum_required(VERSION 3.1.0)

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
set(args_t5 -d 3.14159265)
set(args_t6 -d=3.14159265)
set(args_t7 -d 3.1f)
set(args_t8 -d=4ff)
set(args_t9 -f)
set(args_t10 --flag)
set(args_t12 -ndf 10 3.14159265)

# Adding Executables
add_executable( int_test int_test.cc ${sources})
add_executable( int_fail int_fail.cc ${sources})
add_executable( double_test double_test.cc ${sources})
add_executable( double_fail double_fail.cc ${sources})
add_executable( flag_test flag_test.cc ${sources})
add_executable( multi_args multi_args.cc ${sources})

# Adding Tests
add_test( test_1 int_test ${args_t1})
add_test( test_2 int_test ${args_t2})
add_test( test_3 int_fail ${args_t3})
add_test( test_4 int_fail ${args_t4})
add_test( test_5 double_test ${args_t5})
add_test( test_6 double_test ${args_t6})
add_test( test_7 double_fail ${args_t7})
add_test( test_8 double_fail ${args_t8})
add_test( test_9 flag_test ${args_t9})
add_test( test_10 flag_test ${args_t10})
add_test( test_11 multi_args ${args_t1} ${args_t6} ${args_t10})
add_test( test_12 multi_args ${args_t12})