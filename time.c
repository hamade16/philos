#include <stdio.h>
#include <sys/time.h>

int main()
{
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
     long time_mili = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
    return time_mili;
}