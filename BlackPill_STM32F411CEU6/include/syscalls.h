#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <sys/types.h>

#ifdef __cplusplus
    extern "C"
    {
#endif

pid_t _getpid(void);
void _exit(int return_code);
int _kill(int pid, int sig);

#ifdef __cplusplus
    }
#endif


#endif // SYSCALLS_H
