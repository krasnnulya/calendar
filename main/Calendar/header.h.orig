#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include "graphics.h"

// Макросы определяют положения объектов
// Чтобы не вводить каждый раз вручную

#define BLUE_ COLOR(144, 185, 233)

//#define ERROR_FILE_OPEN -3

#define MONTH_X 75
#define MONTH_Y 62
#define MONTH_W 208
#define MONTH_H 28
#define MONTH_TEXT_X 180
#define MONTH_TEXT_Y 85

#define YEAR_X 75
#define YEAR_Y 100
#define YEAR_W MONTH_W
#define YEAR_H MONTH_H
#define YEAR_TEXT_X 180
#define YEAR_TEXT_Y 120

#define CALENDAR_X 23
#define CALENDAR_Y 173
#define CALENDAR_W 322
#define CALENDAR_H 289

#define DAY_X 40
#define DAY_Y 203
#define DAY_W 33
#define DAY_H 44

#define EDIT_X 271
#define EDIT_Y 601

#define NOTE_X 23
#define NOTE_Y 565
#define NOTE_W 319
#define NOTE_H 137

#define TEXT_X 35
#define TEXT_Y 633
#define TEXT_W 305
#define TEXT_H 60

#define DAY_DIST_X 14
#define DAY_DIST_Y 6

#define NOTE_SIZE 25

#define WEEK_X 39
#define WEEK_Y 163
#define WEEK_W 345
#define WEEK_H 174

#define WDAY_X 246
#define WDAY_Y 170

#define PLANS_OFFSET_X 35
#define PLANS_OFFSET_Y 205


// Изображения
IMAGE* images[6];

// Индексы в массиве
enum IMAGE_INDEX
{
    IMG_MAIN,
    IMG_BACKGROUND,
    IMG_ABOUT,
    IMG_NOTE,
    IMG_EDAY,
    IMG_NDAY
};

/*struct button
{
   int x1, x2, y1, y2;
} button;*/

typedef struct day
{     
    char note[NOTE_SIZE];
} day;

typedef struct month
{
    int amount;
    day days[31];
} month;

typedef struct year
{
    month months[12];
} year;

// Записи в виде дерева
year years[5];

// Кол-во дней в месяцах
const int dim[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const char* mts[12] = { "Январь",
                        "Февраль",
                        "Март",
                        "Апрель",
                        "Май",
                        "Июнь",
                        "Июль",
                        "Август",
                        "Сентябрь",
                        "Октябрь",
                        "Ноябрь",
                        "Декабрь"};

const char* week[13] = {"ПН", "ВТ", "СР", "ЧТ", "ПТ", "СБ", "ВС", "ПН", "ВТ", "СР", "ЧТ", "ПТ", "СБ"};

// Проверка на високосность
int is_leap_year(int year)
{
    if(year % 400 == 0)
        return 1;

    if(year % 100 == 0)
        return 0;

    if(year % 4 == 0)
        return 1;

    return false;
}

// Количество дней в месяце
int days_in_month(int year, int month)
{
    if(is_leap_year(year) && month == 1)
        return 29;

    else return dim[month];
}



   
#endif
