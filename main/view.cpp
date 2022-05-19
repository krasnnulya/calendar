#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "header.h"

int curYear = 2022;
int curMonth = 4;
int curWeekday = 0;
int curDay = 0;
int curPage = 0;

// Загрузка изображений
void load_images()
{
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
void unload_images()
{
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

// Перевод из даты в UNIX time
time_t date_to_time(date *d)
{
   struct tm timeInfo = {0};

   timeInfo.tm_mday = d->day+1;
   timeInfo.tm_mon = d->month;
   timeInfo.tm_year = d->year - 1900;

   return mktime(&timeInfo);
}

// Перевод из UNIX time в дату
void time_to_date(time_t t, date *entry)
{
   struct tm *timeInfo;
   timeInfo = localtime(&t);

   entry->day = timeInfo->tm_mday-1;
   entry->month = timeInfo->tm_mon;
   entry->year = timeInfo->tm_year + 1900;
}

// Загрузить записи в оперативную память (все)
// Они хранятся в виде дерева
void load_notes()
{
   FILE *file = fopen("notes.txt", "r+");
   if (file != NULL)
   {
      char line[100];

      while (fgets(line, 100, file))
      {
         char text[100];

         char *p = strtok(line, ":");
         time_t t = strtoul(p, NULL, 10);

         p = strtok(NULL, ":");
         strcpy(text, p);

         date e;
         time_to_date(t, &e);

         strtok(text, "\n");

         years[e.year-2022].months[e.month].amount++;
         years[e.year-2022].months[e.month].days[e.day].isEmpty = 0;
         strcpy(years[e.year-2022].months[e.month].days[e.day].note, text);
      }

      fclose(file);
   }
}

// Сохранить записи на файл
void save_notes()
{
   FILE *file = fopen("notes.txt", "w");
   if (file != NULL)
   {
      for (int y = 0; y < 5; y++)
      {
         for (int m = 0; m < 12; m++)
         {
            int n = days_in_month(2022+y, m);

            for (int d = 0; d < n; d++)
            {
                  if (years[y].months[m].days[d].isEmpty == 0)
                  {
                     date entry;

                     entry.day = d;
                     entry.month = m;
                     entry.year = y+2022;

                     time_t t = date_to_time(&entry);

                     char out[100];
                     sprintf(out, "%lld:%s\n", t, years[y].months[m].days[d].note);
                     fputs(out, file);
                  }
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

// Получаем название недели по номеру
//const char *weekday_str(int wd)
//{
  // return wname[wd];
//}


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
int weekday( int date,int month,int year )
{
   int cnt, dayindex, wdaytab[] = { 6, 0, 1, 2, 3, 4, 5 };
   if( month<3 )
    {
       month+=12;
       year--;
    }
   cnt = date + ((13 * month - 27) / 5) + year;
   dayindex = (cnt + (year / 4) - (year / 100) + (year / 400)) % 7;
   return wdaytab[ dayindex ];
}

 //дни прошлого месяца   (последняя неделя)


 //дни следующего месяца   (первая неделя)


//Нарисовать числа с привязкой к дням недели
void draw_weekdays( int wday)
{
   setfillstyle(SOLID_FILL, COLOR(145, 185, 236));
   settextstyle(COMPLEX_FONT, HORIZ_DIR, 4);
   bar(CALENDAR_X, CALENDAR_Y, 360, 400);
   
   if (wday == 4 || wday==5){
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


// Очистить поле заметки на экране
void clear_note()
{
   bar(NOTE_X,
       NOTE_Y,
       NOTE_X+NOTE_W,
       NOTE_Y);
}

// Обработчик заметки при нажатии на нее, здесь происходит ввод в поле заметки
// сохранение, и т.п.
void hour_handler()
{
   settextjustify(LEFT_TEXT, CENTER_TEXT);
   settextstyle(COMPLEX_FONT, HORIZ_DIR, 2);

   putimage(EDIT_X, EDIT_Y, images[IMG_EDIT], 0);

   int x = NOTE_TEXT_X;
   int y = NOTE_TEXT_Y;

   char out[NOTE_SIZE];
   memset(out, '\0', NOTE_SIZE);
   int i = 0;

   if (years[curYear-2022].months[curMonth].days[curDay].isEmpty == 0)
   {
      strcpy(out, years[curYear-2022].months[curMonth].days[curDay].note);
      i = strlen(out);
   }

   setfillstyle(SOLID_FILL, COLOR(226, 244, 255));

   while (1)
   {
      if (kbhit())
      {
         char c = getch();

         if (c == KEY_ENTER)
         {
            if (out[0] != '\0')
            {
               years[curYear-2022].months[curMonth].days[curDay].isEmpty = 0;
               years[curYear-2022].months[curMonth].amount++;
            }
            else
            {
               years[curYear-2022].months[curMonth].days[curDay].isEmpty = 1;
               years[curYear-2022].months[curMonth].amount--;
            }

            strcpy(years[curYear-2022].months[curMonth].days[curDay].note, out);
            putimage(EDIT_X, EDIT_Y, images[IMG_ADAY], 0);

            break;
         }
         else
         {
            clear_note();

            if (c == KEY_BACKSPACE)
            {
               if (i != 0)
                  out[--i] = '\0';
            }
            else if (32 <= (unsigned)c)
            {
               if (i != NOTE_SIZE-1)
                  out[i++] = c;
            }

            outtextxy(x, y, out);
         }
      }
   }
}

// Обработчик дня
void day_handler()
{

   while (1)
   {
      while (mousebuttons() == 0);
      while (mousebuttons() != 0);

      int x = mousex();
      int y = mousey();

      if (114 <= x && x <= 308 && 72 <= y && y <= 140)
      {
         break;
      }
      else if (114 <= x && x <= 308 && 174 <= y && y <= 240)
      {
         save_notes();
         unload_images();
         exit(0);
      }
      else if (114 <= x && x <= 308 && 498 <= y && y <= 565)
      {
         if (curPage > 0)
            curPage--;
      }
      else if (114 <= x && x <= 308 && 586 <= y && y <= 653)
      {
         if (curPage < 3)
            curPage++;
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

      if (40 <= x && x <= 70 && 100 <= y && y <= 130)
      {
         if (2022 < curYear)
         {
            curYear--;
         }
      }
      else if (290 <= x && x <= 320 && 100 <= y && y <= 30)
      {
         if (curYear < 2035)
         {
            curYear++;
         }
      }
      else if (40  <= x && x <= 70 && 60 <= y && y <= 90)
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
      else if (is_days_clicked(x, y))
      {
         day_handler();
      }
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
            if(260 <= y && y <= 340)
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

