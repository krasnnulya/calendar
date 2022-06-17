#include<stdio.h>
#include<time.h>
 
// ���������� ����� ���� � ������
int getmaxdays( int month,int year )
 {
int maxdays[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }, n;
if( month != 2 ) return maxdays[ month ];
n=28;
if( (year % 4)==0 )   n++;
if( (year % 100)==0 ) n--;
if( (year % 400)==0 ) n++;
return n;
 }
 
// ���������� ���� ������ (0-�����������, 6-�����������)
int weekday( int dat,int month,int year )
 {
int cnt, dayindex, wdaytab[] = { 6, 0, 1, 2, 3, 4, 5 };
if( month<3 )
 {
 month+=12;
 year--;
 }
cnt = dat + ((13 * month - 27) / 5) + year;
dayindex = (cnt + (year / 4) - (year / 100) + (year / 400)) % 7;
return wdaytab[ dayindex ];
 }
 
 
 
int main(void)
 {
   int i, j, p, pday, today, q, dat, month, year, prevmonth, prevyear;
    
   char tbuf[ 81 ];
   const char *wname[] = { "��", "��", "��", "��", "��", "��", "��" };
   const char *xname[] =
    {
    "�����������","�������","�����","�������",
    "�������","�������","�����������"
    };
    
   const char *mname[]=
    {
    "*", "������", "�������", "�����", "������",
    "���", "����", "����", "�������",
    "��������", "�������", "������", "�������"
    };
 
   time_t t = time(NULL);
   struct tm tm = *localtime( &t );
 
 
   // ������ ������� ����
   dat = tm.tm_mday;
   month = tm.tm_mon + 1;
   year = tm.tm_year + 1900;
 
   sprintf( tbuf,"%d %s %d %s", dat, mname[ month ], year, xname[ weekday(dat, month, year) ] );
   printf( "%36s\n", tbuf );
 
// ������ ���� ������
j = -1;
for( i=0; i<7; i++ )
 {
 sprintf( tbuf,"%s", wname[ i ] );
 printf( "%5s", tbuf );
 }
printf( "\n------------------------------------\n" );
 
// ����� �������� ��� 0 ����� ������ ����������� ������
p = 1 - weekday( 1, month, year );
i = 0;
j = 0;
today = 0;
 
// ������� ���������
do
 {
 pday = p;
 q = getmaxdays( month, year );
 
 // ����� ���������� ������
 if( p>q )
  {
  pday = -1;
  }
 // ����� ����������� ������
 if( p<=0 )
  {
  prevyear = year;
  prevmonth = month - 1;
  if( prevmonth==0 )
   {
   prevmonth = 12;
   prevyear--;
   }
  //pday = getmaxdays( prevmonth, prevyear ) + p;
  if( p==0 ) pday = getmaxdays( prevmonth, prevyear ) + p;
  else pday = p;
  }
 
 // ������� ����
 if( p==dat )
  {
  today = 1;
  }
 
 sprintf( tbuf, "%s%d", (today) ? "*" : "", pday );
 printf( "%5s", (pday<0) ? " " : tbuf );
 
 today = 0;
 p++;
 i++;
 if( i==7 )
  {
  i=0;
  j++;
  printf( "\n" );
  }
 } while( !((i==0) && (p>getmaxdays( month, year))) );
printf( "------------------------------------\n" );
printf( "* - ����������� ����\n" );
getchar();
return 0;
}