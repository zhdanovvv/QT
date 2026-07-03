#include <unistd.h>
#include <errno.h>

pid_t _getpid(void) 
{
    return 1;
}

int _kill(int pid, int sig) 
{
    (void)pid;
    (void)sig;
    errno = EINVAL;
    return -1;
}
