#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "header.h"

// ������� ���, ����� � ����
int GlobY = 0;
int GlobM = 0;
int GlobD = 0;

//����������� ������� ���� � ����������
/*int today()
{
   time_t  t;
   struct tm*  aTm;
   t = time(NULL);
   aTm = localtime(&t);         
     aTm->tm_year +1900;
     aTm->tm_mon +=1;
     aTm->tm_mday = 1;
   return 0;
}*/

//��������� ������
/*int buttons[7][4] = {
   {26, 156, 673, 698}, //���������� ������ ����������
   {317, 355, 6, 44}, //���������� ������ ������ � ������� ����
   {47, 65, 61, 90}, //���������� ������ ������������ �� ����� �����
   {292, 310, 61, 90}, //���������� ������ ������������ �� ����� ������
   {135, 218, 476, 510}, //���������� ������ ����������� � ������� ���� �� ���� "� ���������"
   {253, 283, 93, 122}, //���������� ������ ������ ���� "� ���������"
   {141, 218, 376, 408} //���������� ������ ������ ��������� ���������
}*/

// �������� �����������
void load_images()
{
    images[IMG_MAIN] = loadBMP("main.bmp");
    images[IMG_BACKGROUND] = loadBMP("background.bmp");
    images[IMG_ABOUT] = loadBMP("about.bmp");
    images[IMG_NOTE] = loadBMP("note.bmp");
    images[IMG_EDAY] = loadBMP("event_day.bmp");
    images[IMG_NDAY] = loadBMP("now_day.bmp");
}

// �������� �����������
void unload_images()
{
    free(images[IMG_MAIN]);
    free(images[IMG_BACKGROUND]);
    free(images[IMG_ABOUT]);
    free(images[IMG_NOTE]);
    free(images[IMG_EDAY]);
    free(images[IMG_NDAY]);
}

// ��������� ������ �� �����
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
            text[n-1] = '\0'; // ��������� \n � �����
            strcpy(years[i].months[j].days[k].note, text);
        }

        fclose(file);
    }
}

// ��������� ������ �� ����
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

// ������ ������ �������������������
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


// �������� ���� �������� ������
void clear_month()
{
    setfillstyle(SOLID_FILL, BLUE_);
    bar(MONTH_X, MONTH_Y, MONTH_X+MONTH_W, MONTH_Y+MONTH_H);
}

// �������� ����� �������
void clear_text()
{
    setfillstyle(SOLID_FILL, WHITE);
    putimage(NOTE_X, NOTE_Y, images[IMG_NOTE], 0);
}

// �������� ���� �������� ����
void clear_year()
{
    setfillstyle(SOLID_FILL, BLUE_);
    bar(MONTH_X, MONTH_Y, MONTH_X+MONTH_W, MONTH_Y+MONTH_H);
}

// �������� ��������� (��� ������)
void clear_calendar()
{
    setfillstyle(SOLID_FILL, BLUE_);
    bar(CALENDAR_X, CALENDAR_Y, CALENDAR_X+CALENDAR_W, CALENDAR_Y+CALENDAR_H);
}


//������������� ���� ������
int day_of_week()
{
    struct tm time = { 0 };
    time.tm_year = GlobY+2022 - 1900;
    time.tm_mon = GlobM;
    time.tm_mday = 1;
    mktime(&time);
    return (time.tm_wday + 6) % 7;
}


// �������� ���� �������� ���� ����� ��������� GlobY
void update_year()
{
    clear_year();

    char yearStr[5];
    sprintf(yearStr, "%d", GlobY+2022);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(YEAR_TEXT_X, YEAR_TEXT_Y, yearStr);
}

// �������� ���� �������� ������ ����� �������� GlobM
// �������� �������� ������ �� ������ (0-11 --> ������, �������...)
void update_month()
{
    clear_month();
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(MONTH_TEXT_X, MONTH_TEXT_Y, mts[GlobM]);
}


// ���������� ��� ���������
void draw_days()
{
    clear_calendar();
    settextjustify(CENTER_TEXT, CENTER_TEXT);
   
    int n = days_in_month(GlobY+2022, GlobM);
    int s = day_of_week();
   // int p = today();

    for(int i = 0; i < n; i++)
    {
        int dx = (DAY_W+DAY_DIST_X)*((i+s)%7);
        int dy = (DAY_H+DAY_DIST_Y)*((i+s)/7);
       
        if(years[GlobY].months[GlobM].days[i].note != 0) //��������� �������
          putimage(PLANS_OFFSET_X+dx, PLANS_OFFSET_Y+dy, images[IMG_EDAY], 0); 
      

      /* if(p == today)  //��������� �������� ���
           {
            GlobD = 1;
            putimage(dx, dy, images[IMG_NDAY], 0);
           }*/

        char num[3];
        sprintf(num, "%d", i+1);
        outtextxy(DAY_X+dx, DAY_Y+dy, num);
    }
}

// ���������, ������ �� ��� ��������� � ���� �� ��������� GlobD ������� ����
int is_days_clicked(int cx, int cy)
{
    if(CALENDAR_X < cx && cx < CALENDAR_X+CALENDAR_W && CALENDAR_Y < cy && cy < CALENDAR_Y+CALENDAR_H)
    {
        int ux = cx-CALENDAR_X;
        int uy = cy-CALENDAR_Y;

        int n = days_in_month(GlobY+2022, GlobM);
        int s = day_of_week();

        for(int i = 0; i < n; i++)
        {
            int dx = (DAY_W+DAY_DIST_X)*((i+s)%7);
            int dy = (DAY_H+DAY_DIST_Y)*((i+s)/7);

            if(dx < ux && ux < dx+DAY_W && dy < uy && uy < dy+DAY_H)
            {
                GlobD = i;
                return 1;
            }
        }
    }

    return 0;
}

// ���������� ������� (���� ������ � ����������)
void note_handler()
{
    putimage(NOTE_X, NOTE_Y, images[IMG_NOTE], 0);

    char out[NOTE_SIZE];
    memset(out, '\0', NOTE_SIZE);
    strcpy(out, years[GlobY].months[GlobM].days[GlobD].note);

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

            if(c == KEY_BACKSPACE) // ������� ������
            {
                if(i != 0)
                    out[--i] = '\0';

            }
            else if(32 <= (unsigned)c) // �������� ������
            {
                if(i != NOTE_SIZE-1)
                    out[i++] = c;
            }

            outtextxy(TEXT_X, TEXT_Y, out);
        }
         else if(mousebuttons() == 1) //����������
        {
            while(mousebuttons() != 0);

            int cx = mousex();
            int cy = mousey();

            if(26 < cx && cx < 156 && 673 < cy && cy < 698)
            {
                strcpy(years[GlobY].months[GlobM].days[GlobD].note, out);
                save_notes();
                setusercharsize(1, 1, 1, 1);
                break;
            }
        }
    }
}

// �������� ��������� (������ ���������� ���, ����� � ���)
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

// ���������� ������� �� ���������
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
            if(!(GlobY == 0 && GlobM == 0))
            {
                GlobM--;
                if(GlobM < 0)
                {
                    GlobM = 11;
                    GlobY--;
                }
            }
        }
        else if(292 <= cx && cx <= 310 && 61 <= cy && cy <= 90)
        {
            if(!(GlobY == 4 && GlobM == 11))
            {
                GlobM++;
                if(GlobM > 11)
                {
                    GlobM = 0;
                    GlobY++;
                }
            }
        }
        else if(is_days_clicked(cx, cy))
        {
            note_handler();
        }
    }
}

// ���������� ������� "� ���������"
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

// ������� �������
int main()
{
    int win = initwindow(361, 750, "���������");
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