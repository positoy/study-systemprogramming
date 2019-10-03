#include <stdio.h>
#include <time.h>
#include <sys/time.h>

/*
    time_t timestamp = time();

    # takes time_t
    ctime, gmtime, localtime

    # takes struct tm
    asctime, mktime
    
    # takes struct timeval
    gettimeofday, settimeofday
*/

int main()
{
    time_t timestamp = time(NULL);
    printf("timestamp(%lu)\n", timestamp);

    char* timestr = ctime(&timestamp); // localtime
    printf("%s", timestr);

    struct tm* bdt = gmtime(&timestamp); // GMT
    printf("sec(%d) min(%d) hour(%d) mday(%d) mon(%d) year(%d) wday(%d) yday(%d) isdst(%d)\n",
        bdt->tm_sec, bdt->tm_min, bdt->tm_hour, bdt->tm_mday, bdt->tm_mon, bdt->tm_year, bdt->tm_wday, bdt->tm_yday, bdt->tm_isdst);
    
    struct tm* bdt2 = localtime(&timestamp); // GMT
    printf("sec(%d) min(%d) hour(%d) mday(%d) mon(%d) year(%d) wday(%d) yday(%d) isdst(%d)\n",
        bdt2->tm_sec, bdt2->tm_min, bdt2->tm_hour, bdt2->tm_mday, bdt2->tm_mon, bdt2->tm_year, bdt2->tm_wday, bdt2->tm_yday, bdt2->tm_isdst);

    char* timestr2 = asctime(bdt2); // localtime
    printf("%s", timestr2);

    time_t timestamp2 = mktime(bdt2);
    printf("epoch time(%lu)\n", timestamp2);

    struct timeval tv;
    gettimeofday(&tv, NULL);
    printf("timestamp sec(%lu) usec(%lu)\n", tv.tv_sec, tv.tv_usec);
}