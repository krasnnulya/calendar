#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include "graphics.h"

// ������� ���������� ��������� ��������
// ����� �� ���������� �������
#define CUR_MONTH_X 180
#define CUR_MONTH_Y 80

#define CUR_YEAR_X 180
#define CUR_YEAR_Y 120

#define DAYS_OFFSET_X 25
#define DAYS_OFFSET_Y 200

#define PLANS_OFFSET_X 20
#define PLANS_OFFSET_Y 190

#define CALENDAR_X 50
#define CALENDAR_Y 230

#define TIME_X 435
#define TIME_Y 110
#define TIME_H 107

#define NOTE_X 474
#define NOTE_Y 52
#define NOTE_W 765
#define NOTE_H 106

#define NOTE_TEXT_X 500
#define NOTE_TEXT_Y 110

#define EDIT_X 340
#define EDIT_Y 87

#define NOTE_SIZE 51

// �����������
IMAGE *images[9];

// ������� � �������
enum IMAGE_INDEX
{
   IMG_ADAY,
   IMG_BACKGROUND,
   IMG_DATA,
   IMG_EDAY,
   IMG_NDAY,
   IMG_WEEKENDS,
   IMG_ABOUT,
   IMG_EDIT,
   IMG_THESE
};

// ��������� ��� ���� (������)
typedef struct date
{
   int year;
   int month;
   int weekday;
   int day;
} date;

typedef struct day
{
   int isEmpty; // ������ �� ������? ���� ���� � ������� ���-�� ����, � ������� '\0'
   char note[NOTE_SIZE];
} day;


typedef struct month
{
   int amount; // ���-�� ������� �� ����, ���� > 0 �� �������� ������� ����a
   day days[31];
} month;

typedef struct year
{
   int amount; // ���-�� ������� �� ����, ���� > 0 �� �������� ������� �����
   month months[12];
} year;

year years[5]; // ������-������

// ���-�� ���� � ������� (������������)
const int dim[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const char *mts[12] = { "������", "�������", "����", "������", "���", "����", "����", "������", "��������", "�������", "������", "�������"};

// ���-�� ���� � ������
const char *wname[7] = { "��", "��", "��", "��", "��", "��", "��" };
const char *xname[7] =
 {
 "�����������","�������","�����","�������",
 "�������","�������","�����������"
 };

 
 

// �������� �� ��� ����������?
int is_leap_year(int year)

{
   if (year % 4 == 0)
      return 1;

   if (year % 100 == 0)
      return 0;

   if (year % 400 == 0)
      return 1;

   return false;
}

// ���������� ���� � ������
int days_in_month(int year, int month)
{
   if (is_leap_year(year) && month == 1)
      return 29;
   else return dim[month];
}




#endif
