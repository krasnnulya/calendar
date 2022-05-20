#include <time.h>
#include <stdio.h>
int weekday(struct tm *dt)
{ time_t t=mktime(dt);
   int wd=localtime(&t)->tm_wday-1; // 0 - понедельник
   if(wd<0) wd+=7;
   return ;
}
int main()
{
   time_t t=time(0);
   struct tm dt=*localtime(&t);
   
   dt.tm_mday=31;
   dt.tm_mon=3;
   t=mktime(&dt);
   printf("%d %d\n",dt.tm_mday,dt.tm_mon);
   
   //printf("%d\n",weekday(&dt));
}
