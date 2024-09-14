#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>

unsigned int on = 0;

int main()
{
    struct sigaction sa;
    struct itimerval timer;

    // sa.sa_handler = &PLACEHOLDER; passar o ponteiro da função que será chamada
    sa.sa_flags = SA_RESTART;
    sigaction(SIGALRM, &sa, NULL);

    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 1000;

    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 1000;

    setitimer(ITIMER_REAL, &timer, NULL);

    on = 1;

    while (on)
    {
        // estabelecer validacao para encerrar o loop
    }

    return 0;
}