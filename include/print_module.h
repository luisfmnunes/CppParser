#include<iostream>
#include<string>
#include<cstdarg>
#include<cstdlib>
#include<unistd.h>

#define RED_FG "\033[1;31m"
#define GREEN_FG "\033[1;32m"
#define BLUE_FG "\033[1;34m"
#define YELLOW_FG "\033[1;33m"
#define MAGENTA_FG "\033[1;35m"
#define CYAN_FG "\033[1;36m"
#define CLOSE_COLOR "\033[0m"
#define BOLD "\e[1m"
#define CLOSE_BOLD "\e[0m"

//C-Like variadic functions
void _log(const char *format, ...);
void _warn(const char *format, ...);
void _error(const char *format, ...);
void _debug(const char *format, ...);

//C++-Like variadic functions

// Static Print Functions
static void rec_print(){
    std::cout << std::endl;
}
template<class T, class... Args> static inline void rec_print(const T& First, Args... args){
    std::cout << First << " ";
    rec_print(args...);
};

template<class... Args> inline void os_log(Args... args){
    std::cout << '(' << getpid() << ") [" << CYAN_FG << "LOG" << CLOSE_COLOR << "]: ";
    rec_print(args...);
};
template<class... Args> inline void os_warn(Args... args){
    std::cout << '(' << getpid() << ") [" << YELLOW_FG << "WARNING" << CLOSE_COLOR << "]: ";
    rec_print(args...);
};
template<class... Args> inline void os_error(Args... args){
    std::cout << '(' << getpid() << ") [" << RED_FG << "ERROR" << CLOSE_COLOR << "]: ";
    rec_print(args...);
};
template<class... Args> inline void os_debug(Args... args){
    std::cout << '(' << getpid() << ") [" << MAGENTA_FG << "DEBUG" << CLOSE_COLOR << "]: ";
    rec_print(args...);
};