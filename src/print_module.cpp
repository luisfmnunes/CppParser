#include"../include/print_module.h"

void _log(const char *format, ...){
    va_list argptr;
    fprintf(stderr,"(%d) [%sLOG%s]: ",getpid(),CYAN_FG,CLOSE_COLOR);
    va_start(argptr,format);
    vfprintf(stderr,format,argptr);
    fprintf(stderr,"\n");
    va_end(argptr);
}
void _warn(const char *format, ...){
    va_list argptr;
    fprintf(stderr,"(%d) [%sWARNING%s]: ",getpid(),YELLOW_FG,CLOSE_COLOR);
    va_start(argptr,format);
    vfprintf(stderr,format,argptr);
    fprintf(stderr,"\n");
    va_end(argptr);
}
void _error(const char *format, ...){
    va_list argptr;
    fprintf(stderr,"(%d) [%sERROR%s]: ",getpid(),RED_FG,CLOSE_COLOR);
    va_start(argptr,format);
    vfprintf(stderr,format,argptr);
    fprintf(stderr,"\n");
    va_end(argptr);
}
void _debug(const char *format, ...){
    va_list argptr;
    fprintf(stderr,"(%d) [%sDEBUG%s]: ",getpid(),MAGENTA_FG,CLOSE_COLOR);
    va_start(argptr,format);
    vfprintf(stderr,format,argptr);
    fprintf(stderr,"\n");
    va_end(argptr);
}

