#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graphics.h"

// Макросы определяют положения объектов
// Чтобы не вводить каждый раз вручную
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

#define NOTE_X 50
#define NOTE_Y 600
#define NOTE_W 220
#define NOTE_H 40

#define NOTE_TEXT_X 50
#define NOTE_TEXT_Y 610

#define EDIT_X 280
#define EDIT_Y 600

#define NOTE_SIZE 50

// Изображения
IMAGE *images[9];

// Индексы в массиве
enum IMAGE_INDEX{  IMG_ADAY,  IMG_BACKGROUND,   IMG_DATA,
   IMG_EDAY,   IMG_NDAY,   IMG_WEEKENDS,   IMG_ABOUT,   IMG_EDIT,
   IMG_THESE};
   
// Запрос текущей даты
/*dat = tm.tm_mday;
weekday = tm.tm_wday+1;
month = tm.tm_mon + 1;
year = tm.tm_year + 1900;*/

// возвращает число дней в месяце
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

// возвращает день недели (0-понедельник, 6-воскресенье)
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
  
   
int date, d;
int curYear = 2022;
int curMonth =5;
int curWeekday = 0;
int curDay = 0;
int curPage = 0;


typedef struct day{
   char note[NOTE_SIZE];
   int isEmpty=0;
} day;

typedef struct month {
   int amount; // кол-во записей на день, если > 0 то ставится зеленая меткa
   day days[31];
} month;

// на несколько лет????!!!
typedef struct year{
   int amount; // кол-во записей на день, если > 0 то ставится зеленая метка
   month months[12];
} year;

month months[12]; //???

year years[5]; // Массив-дерево

// Кол-во дней в месяцах (невисокосных)
const int dim[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const char *mts[12] = { "Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"};

// Является ли год високосным?
int is_leap_year(int year1)
{
   if (year1 % 4 == 0)   return 1;
   if (year1 % 100 == 0) return 0;
   if (year1 % 400 == 0)   return 1;
   return 0;
}

// Количество дней в месяце
int days_in_month(int year1, int month1) {
   if (is_leap_year(year1) && month1 == 1)  return 29;
   else return dim[month1];
}

// Загрузка изображений
void load_images() {
   images[IMG_ADAY] = loadBMP("another_day.bmp");
   images[IMG_BACKGROUND] = loadBMP("background.bmp");
   images[IMG_DATA] = loadBMP("date_ok.bmp");
   images[IMG_EDAY] = loadBMP("event_day.bmp");
   images[IMG_NDAY] = loadBMP("now_day.bmp");
   images[IMG_WEEKENDS] = loadBMP("weekends.bmp");
   images[IMG_ABOUT] = loadBMP("about.bmp");
   images[IMG_EDIT] = loadBMP("edit.bmp");
   images[IMG_THESE] = loadBMP("these.bmp");
}

// Выгрузка изображений
void unload_images() {
   free(images[IMG_ADAY]);
   free(images[IMG_BACKGROUND]);
   free(images[IMG_DATA]);
   free(images[IMG_EDAY]);
   free(images[IMG_NDAY]);
   free(images[IMG_WEEKENDS]);
   free(images[IMG_ABOUT]);
   free(images[IMG_EDIT]);
   free(images[IMG_THESE]);
}


// Загрузить записи в оперативную память (все)
// Они хранятся в виде дерева
void load_notes()
{
   FILE *file = fopen("notes.txt", "r+");
   int m, d;
   if (file != NULL)
   {
      char text[NOTE_SIZE];

       while(fscanf(file,"%d:%d%:", &m, &d, text) > 0 && fgets(text, NOTE_SIZE, file) != NULL)
      {
         int n = strlen(text);
         text[n-1] = '\0'; // убирается \n в конце         
         strcpy(months[d].days[d].note, text);
      }

      fclose(file);
   }
}

// Сохранить записи на файл
void save_notes()
{
   int n;
   FILE *file = fopen("notes.txt", "w");
   if (file != NULL)
   {
         for (int m = 0; m < 12; m++)
         {

            for (int d = 0; d < n; d++)
            {
                  if (months[m].days[d].note[0] != 0)
                  {
                     fprintf(file, "%d %d %s\n", m,d, months[m].days[d].note);
                  }
            }
      }

      fclose(file);
   }
}

// Инициализация дерева (довольно громоздко)
void init()
{
   for (int y = 0; y < 5; y++)
   {
      for (int m = 0; m < 12; m++)
      {
         int n = days_in_month(2022+y, m);
         years[y].months[m].amount = n;
      }
   }
}

// Получаем название месяца по номеру
const char *month_str(int m)
{
   return mts[m];
}

// Очистить поле текущего месяца
void clear_month()
{
   setfillstyle(SOLID_FILL, COLOR(145, 185, 236));
   settextstyle(COMPLEX_FONT, HORIZ_DIR, 4);
   bar(70, 50, 280, 80);
}

// Очистить поле текущего года
void clear_year()
{
   setfillstyle(SOLID_FILL, COLOR(145, 185, 236));
   settextstyle(COMPLEX_FONT, HORIZ_DIR, 4);
   bar(70, 90, 280, 120);
}

// Обновить поле текущего года
void update_year()
{
   char yearStr[5];
   sprintf(yearStr, "%d", curYear);

   clear_year();
   outtextxy(CUR_YEAR_X, CUR_YEAR_Y, yearStr);
}

// Обновить поле текущего месяца
void update_month()
{
   clear_month();
   outtextxy(CUR_MONTH_X, CUR_MONTH_Y, month_str(curMonth));
}

 //дни текущего месяца     
/*int weekday( int date,int month1,int year1)
{
   int cnt, dayindex, wdaytab[] = { 6, 0, 1, 2, 3, 4, 5 };
   if( month1<3 ){
       month1+=12;
       year1--;
    }
   cnt = date + ((13 * month1 - 27) / 5) + year1;
   dayindex = (cnt + (year1 / 4) - (year1 / 100) + (year1 / 400)) % 7;
   return wdaytab[ dayindex ];
}*/

 //дни прошлого месяца   (последняя неделя)


 //дни следующего месяца   (первая неделя)


//Нарисовать числа с привязкой к дням недели
void draw_weekdays(int wday)
{
   setfillstyle(SOLID_FILL, COLOR(145, 185, 236));
   settextstyle(COMPLEX_FONT, HORIZ_DIR, 4);
   bar(CALENDAR_X, CALENDAR_Y, 360, 400);
   
   if (wday == 4 || wday == 5){
      putimage(50, 50, images[IMG_WEEKENDS], 0);
   }
   else {
      putimage(50, 50, images[IMG_THESE], 0);
   }   
}



// Нарисовать дни календаря
void draw_days(int y, int m)
{
   setfillstyle(SOLID_FILL, COLOR(145, 185, 236));
   bar(CALENDAR_X, CALENDAR_Y, 360, 400);

   int n = days_in_month(curYear, curMonth);

   for (int i = 0; i < n; i++)
   {
      int dx = 47*(i%7);
      int dy = 47*(i/7);

      putimage(DAYS_OFFSET_X+dx-10, DAYS_OFFSET_Y+dy-25, images[IMG_ADAY], 0);
      
      char num[5];
      sprintf(num, "%d", i+1);
      outtextxy(DAYS_OFFSET_X+dx+20, DAYS_OFFSET_Y+dy+5, num);
   }
}

// Проверить, нажаты ли дни календаря
int is_days_clicked(int x, int y)
{
    if (x >= DAYS_OFFSET_X && y >= DAYS_OFFSET_Y)
    {
       int ux = x-DAYS_OFFSET_X;
       int uy = y-DAYS_OFFSET_Y;

       int dayX = ux/47;
       int dayY = uy/47;

       int n = days_in_month(curYear, curMonth);
       int t = dayY*10+dayX;

       if (t < n)
       {
          if (ux <= dayX*47+100 && uy <= dayY*47+100)
          {
             curDay = t;
             return 1;
          } 
       }
    }

    return 0;
   }

// Заметки по дням
void draw_event_day()
{
    settextstyle(BOLD_FONT, HORIZ_DIR, 2);

    for(int j = 0; j < 31; j++)
    {
        int d = curPage+j;

        settextjustify(CENTER_TEXT, CENTER_TEXT);

        if(strlen(years[curYear].months[curMonth].days[curDay].note) != 0)
        {
            int x = NOTE_TEXT_X;
            int y = NOTE_TEXT_Y+NOTE_H*j;

            settextjustify(LEFT_TEXT, CENTER_TEXT);
            outtextxy(x, y, years[curYear].months[curMonth].days[curDay].note);
        }
    }
}

// Очистить поле заметки на экране
void clear_note()
{
   bar(NOTE_X,
       NOTE_Y+(curDay)*NOTE_H+curDay*2,
       NOTE_X+NOTE_W,
       NOTE_Y+(curDay+1)*NOTE_H+curDay*2);
}

// Обработчик заметки при нажатии на нее, 
//здесь происходит ввод в поле заметки, 
//сохранение, прекращение редактирования при отмене
void event_handler()
{
   settextjustify(LEFT_TEXT, CENTER_TEXT);
   settextstyle(COMPLEX_FONT, HORIZ_DIR, 2);

   putimage(EDIT_X, EDIT_Y+curDay*NOTE_H, images[IMG_EDIT], 0);

   int x = NOTE_TEXT_X;
   int y = NOTE_TEXT_Y+curDay*NOTE_H;

   char out[NOTE_SIZE];
   memset(out, '\0', NOTE_SIZE);
   int i = 0;
   
    int len = strlen(years[curYear].months[curMonth].days[curDay].note);
    if(len != 0)
    {
        strcpy(out, years[curYear].months[curMonth].days[curDay].note);
        i = len;
    }

   setfillstyle(SOLID_FILL, COLOR(226, 244, 255));
 
   int k = 0;

   while (1)
   {
      if (kbhit())
      {
         char c = getch();
         if(c==0) { c=getch(); continue; }
         
         if (c == KEY_ENTER) // сохранить заметку
         {
             // Изменяется кол-во записей на сегодня если в поле что-то есть. Это определяется k
             years[curYear].months[curMonth].amount += k;
             k = 0;

             strcpy(years[curYear].months[curMonth].days[curDay].note, out);
             putimage(EDIT_X, EDIT_Y+curDay*NOTE_H, images[IMG_EDAY], 0);

             break;
         }
         else // изменить
         {
            clear_note();

            if (c == KEY_BACKSPACE) // стереть символ
            {
               if (i != 0)
                  out[--i] = '\0';
                  if(i == 0) k = -1;
            }
            else if (32 <= (unsigned)c) // добавить символ
            {
               if (i != NOTE_SIZE-1)
               {
                  out[i++] = c;
                  k =1;
               }
            }

            outtextxy(x, y, out);
         }
      }
   }
}


// Обновить календарь
void calendar_update()
{
   setcolor(BLACK);
   setbkmode(TRANSPARENT);

   update_year();
   update_month();

   settextjustify(CENTER_TEXT, CENTER_TEXT);
   settextstyle(COMPLEX_FONT, HORIZ_DIR, 4);

   putimage(0, 0, images[IMG_BACKGROUND], 0);
   update_year();
   update_month();
   draw_days(curYear, curMonth);
}

// Обработчик календаря
void calendar_handler()
{
   
    calendar_update();

    while (1)
    {
 
      while (mousebuttons() == 0);
      while (mousebuttons() != 0);

      int x = mousex();
      int y = mousey();

      if (40  <= x && x <= 70 && 60 <= y && y <= 90)
      {
         if (!(curYear == 2022 && curMonth == 0))
         {
            curMonth--;
            if (curMonth < 0)
            {
               curMonth = 11;
               curYear--;
               update_year();
            }
         }
      }
      else if (290 <= x && x <= 320 && 60 <= y && y <= 90)
      {
         if (!(curYear == 2036 && curMonth == 11))
         {
            curMonth++;
            if (curMonth > 11)
            {
               curMonth = 0;
               curYear++;
               update_year();
            }
         }
      }
       else if (25 <= x && x <= 100 && 630 <= y && y <= 700)
       {
          break;
       }
       else if (320 <= x && x <= 350 && 5 <= y && y <= 50)
       {
          save_notes();
          unload_images();
          exit(0);
       }
      else if(is_days_clicked(x, y))
      {
         event_handler();
      }
       else continue;

       calendar_update();
   }
}

// Обработчик "О программе"
void about_handler()
{
   putimage(0, 0, images[IMG_ABOUT], 0);

   while (1)
   {
      while (mousebuttons() == 0);
      while (mousebuttons() != 0);

      int x = mousex();
      int y = mousey();

      if (130 <= x && x <= 220 && 470 <= y && y <= 510)
         break;
   }
}

int main()
{
    int win = initwindow(361, 750);
    setcurrentwindow(win);

    cleardevice();
    load_images();

    init();
    load_notes();

    while(1)
    {
        putimage(0, 0, images[IMG_DATA], 0);

        while(mousebuttons() == 0);
        while(mousebuttons() != 0);

        int x = mousex();
        int y = mousey();

        if(250 <= x && x <= 280)
        {
            if(90 <= y && y <= 120)
            {
                about_handler();
            }
        }
        
        if(140 <= x && x <= 220)
        {
            if(380 <= y && y <= 410)
            {
                calendar_handler();
            }
         }
           
          if(310 <= x && x <= 360)
        {
            if(0 <= y && y <= 50)
            {
                break;
            }
         }
    }  

    unload_images();
    save_notes();
    closegraph();
    return 0;
}

