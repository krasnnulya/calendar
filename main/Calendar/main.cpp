#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "header.h"

// Текущие год, месяц и день
int y = 0;
int m = 0;
int d = 0;
int today = 0;

// Загрузка изображений
void load_images()
{
    images[IMG_MAIN] = loadBMP("main.bmp");
    images[IMG_BACKGROUND] = loadBMP("background.bmp");
    images[IMG_ABOUT] = loadBMP("about.bmp");
    images[IMG_NOTE] = loadBMP("note.bmp");
    images[IMG_EDAY] = loadBMP("event_day.bmp");
    images[IMG_NDAY] = loadBMP("now_day.bmp");
}

// Выгрузка изображений
void unload_images()
{
    free(images[IMG_MAIN]);
    free(images[IMG_BACKGROUND]);
    free(images[IMG_ABOUT]);
    free(images[IMG_NOTE]);
    free(images[IMG_EDAY]);
    free(images[IMG_NDAY]);
}

// Загрузить записи из файла
void load_notes()
{
    FILE* file = fopen("notes.txt", "r+");
    if(file != NULL)
    {
        char text[NOTE_SIZE];
        int i, j, k;

        while(fscanf(file,"%d:%d:%d:", &i, &j, &k) > 0 && fgets(text, NOTE_SIZE, file) != NULL)
        {
            int n = strlen(text);
            text[n-1] = '\0'; // убирается \n в конце
            strcpy(years[i].months[j].days[k].note, text);
        }

        fclose(file);
    }
}

// Сохранить записи на файл
void save_notes()
{
    FILE* file = fopen("notes.txt", "w");
    if(file != NULL)
    {
        for(int i = 0; i < 5; i++)
        {
            for(int j = 0; j < 12; j++)
            {
                int n = days_in_month(2022+i, j);
                for(int k = 0; k < n; k++)
                {
                    if(strlen(years[i].months[j].days[k].note) != 0)
                    {
                        fprintf(file, "%d:%d:%d:%s\n", i, j, k, years[i].months[j].days[k].note);
                    }
                }
            }
        }

        fclose(file);
    }
}

// Пустые записи проинициализировать
void init()
{
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 12; j++)
        {
            int n = days_in_month(2022+i, j);
            years[i].months[j].amount = n;

            for(int k = 0; k < n; k++)
            {
                years[i].months[j].days[k].note[0] = '\0';
            }
        }
    }
}


// Получить название месяца по номеру (0-11 --> январь, февраль...)
const char* month_str(int m)
{
    return mts[m];
}

// Очистить поле текущего месяца
void clear_month()
{
    setfillstyle(SOLID_FILL, BLUE_);
    bar(MONTH_X, MONTH_Y, MONTH_X+MONTH_W, MONTH_Y+MONTH_H);
}

// Очистить текст заметки
void clear_text()
{
    setfillstyle(SOLID_FILL, WHITE);
    putimage(NOTE_X, NOTE_Y, images[IMG_NOTE], 0);
}

// Очистить поле текущего года
void clear_year()
{
    setfillstyle(SOLID_FILL, BLUE_);
    bar(MONTH_X, MONTH_Y, MONTH_X+MONTH_W, MONTH_Y+MONTH_H);
}

// Очистить календарь (дни месяца)
void clear_calendar()
{
    setfillstyle(SOLID_FILL, BLUE_);
    bar(CALENDAR_X, CALENDAR_Y, CALENDAR_X+CALENDAR_W, CALENDAR_Y+CALENDAR_H);
}


//Распределение дней недели
int day_of_week()
{
    struct tm time = { 0 };
    time.tm_year = y+2022 - 1900;
    time.tm_mon = m;
    time.tm_mday = 1;
    mktime(&time);
    return (time.tm_wday + 6) % 7;
}


// Обновить поле текущего года новым значением y
void update_year()
{
    clear_year();

    char yearStr[5];
    sprintf(yearStr, "%d", y+2022);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(YEAR_TEXT_X, YEAR_TEXT_Y, yearStr);
}

// Обновить поле текущего месяца новым значение m
void update_month()
{
    clear_month();
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(MONTH_TEXT_X, MONTH_TEXT_Y, month_str(m));
}

// Нарисовать дни календаря
void draw_days()
{
    clear_calendar();
    settextjustify(CENTER_TEXT, CENTER_TEXT);
   
    int n = days_in_month(y+2022, m);
    int s = day_of_week();

    for(int i = 0; i < n; i++)
    {
        int dx = (DAY_W+DAY_DIST_X)*((i+s)%7);
        int dy = (DAY_H+DAY_DIST_Y)*((i+s)/7);
       
        if(years[y].months[m].days[i].note != 0) //выделение заметок
          putimage(PLANS_OFFSET_X+dx, PLANS_OFFSET_Y+dy, images[IMG_EDAY], 0); 
      
       /* int p;
        if(p == time.tm_mday)  //выделение текущего дня
           {
            today = 1;
            putimage(dx, dy, images[IMG_NDAY], 0);
           }*/

        char num[3];
        sprintf(num, "%d", i+1);
        outtextxy(DAY_X+dx, DAY_Y+dy, num);
    }
}

// Проверить, нажаты ли дни календаря и если да выставить d нажатым днем
int is_days_clicked(int cx, int cy)
{
    if(CALENDAR_X < cx && cx < CALENDAR_X+CALENDAR_W && CALENDAR_Y < cy && cy < CALENDAR_Y+CALENDAR_H)
    {
        int ux = cx-CALENDAR_X;
        int uy = cy-CALENDAR_Y;

        int n = days_in_month(y+2022, m);
        int s = day_of_week();

        for(int i = 0; i < n; i++)
        {
            int dx = (DAY_W+DAY_DIST_X)*((i+s)%7);
            int dy = (DAY_H+DAY_DIST_Y)*((i+s)/7);

            if(dx < ux && ux < dx+DAY_W && dy < uy && uy < dy+DAY_H)
            {
                d = i;
                return 1;
            }
        }
    }

    return 0;
}

// Обработчик заметки (ввод текста и сохранение)
void note_handler()
{
    putimage(NOTE_X, NOTE_Y, images[IMG_NOTE], 0);

    char out[NOTE_SIZE];
    memset(out, '\0', NOTE_SIZE);
    strcpy(out, years[y].months[m].days[d].note);

    settextjustify(LEFT_TEXT, CENTER_TEXT);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, USER_CHAR_SIZE);
    setusercharsize(2, 3, 2, 3);
    outtextxy(TEXT_X, TEXT_Y, out);

    int i = strlen(out);

    while(1)
    {
        if(kbhit())
        {
            char c = getch();
            if(c == 0) { c = getch(); continue; }

            clear_text();

            if(c == KEY_BACKSPACE) // стереть символ
            {
                if(i != 0)
                    out[--i] = '\0';

            }
            else if(32 <= (unsigned)c) // добавить символ
            {
                if(i != NOTE_SIZE-1)
                    out[i++] = c;
            }

            outtextxy(TEXT_X, TEXT_Y, out);
        }
         else if(mousebuttons() == 1) //сохранение
        {
            while(mousebuttons() != 0);

            int cx = mousex();
            int cy = mousey();

            if(26 < cx && cx < 156 && 673 < cy && cy < 698)
            {
                strcpy(years[y].months[m].days[d].note, out);
                save_notes();
                setusercharsize(1, 1, 1, 1);
                break;
            }
        }
    }
}

// Обновить календарь (заново нарисовать год, месяц и дни)
void calendar_update()
{
    setcolor(BLACK);
    setbkmode(TRANSPARENT);
    putimage(0, 0, images[IMG_BACKGROUND], 0);

    settextjustify(CENTER_TEXT, CENTER_TEXT);

    update_year();
    update_month();

    draw_days();
}

// Обработчик нажатий по календарю
void calendar_handler()
{
    while(1)
    {
        calendar_update();

        while(mousebuttons() == 0);
        while(mousebuttons() != 0);

        int cx = mousex();
        int cy = mousey();

        if(317 <= cx && cx <= 355 && 6 <= cy && cy <= 44)
        {
            break;
        }
        else if(47 <= cx && cx <= 65 && 61 <= cy && cy <= 90)
        {
            if(!(y == 0 && m == 0))
            {
                m--;
                if(m < 0)
                {
                    m = 11;
                    y--;
                }
            }
        }
        else if(292 <= cx && cx <= 310 && 61 <= cy && cy <= 90)
        {
            if(!(y == 4 && m == 11))
            {
                m++;
                if(m > 11)
                {
                    m = 0;
                    y++;
                }
            }
        }
        else if(is_days_clicked(cx, cy))
        {
            note_handler();
        }
    }
}

// Обработчик нажатий "О программе"
void about_handler()
{
    putimage(0, 0, images[IMG_ABOUT], 0);

    while(1)
    {
        while(mousebuttons() == 0);
        while(mousebuttons() != 0);

        int cx = mousex();
        int cy = mousey();

        if(135 <= cx && cx <= 218 && 476 <= cy && cy <= 510)
            break;
    }
}

// Главная функция
int main()
{
    int win = initwindow(361, 750);
    setcurrentwindow(win);

    cleardevice();
    load_images();

    init();
    load_notes();

    settextstyle(COMPLEX_FONT, HORIZ_DIR, 4);
    settextjustify(CENTER_TEXT, CENTER_TEXT);

    while(1)
    {
        putimage(0, 0, images[IMG_MAIN], 0);

        while(mousebuttons() == 0);
        while(mousebuttons() != 0);

        int cx = mousex();
        int cy = mousey();

        if(253 <= cx && cx <= 283 && 93 <= cy && cy <= 122)
        {
            about_handler();
        }
        else if(141 <= cx && cx <= 218 && 376 <= cy && cy <= 408)
        {
            calendar_handler();
        }
    }

    unload_images();
    save_notes();
    closegraph();
    return 0;
}