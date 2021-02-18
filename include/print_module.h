#ifndef PRINT_MODULE_H
#define PRINT_MODULE_H

#include<iostream>
#include<string>
#include<cstdarg>
#include<cstdlib>

#ifndef _WIN32
#include<unistd.h>
#else
#include<io.h>
#include<process.h>

#if !defined (getpid)
#define getpid() _getpid()
#endif

#endif

#define RED_FG "\033[1;31m"
#define GREEN_FG "\033[1;32m"
#define BLUE_FG "\033[1;34m"
#define YELLOW_FG "\033[1;33m"
#define MAGENTA_FG "\033[1;35m"
#define CYAN_FG "\033[1;36m"
#define CLOSE_COLOR "\033[0m"
#define BOLD "\33[1m"
#define CLOSE_BOLD "\33[0m"

//C-Like variadic functions
void _log(const char *format, ...);
void _warn(const char *format, ...);
void _error(const char *format, ...);
void _debug(const char *format, ...);

//C++-Like variadic functions

// Static Print Functions
#ifdef _WIN32

#pragma warning(push)
#pragma warning(disable : 4505) //remove unreferrenced function warning C4505

#endif

static void rec_print(){
    std::cout << std::endl;
}

#ifdef _WIN32
#pragma warning(pop)
#endif

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

#endif