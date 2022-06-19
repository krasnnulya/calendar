





// ������� ���, ����� � ����
int y = 0;
int m = 0;
int d = 0;

// �������� �����������
void load_images()
{
    images[IMG_MAIN] = loadBMP("main.bmp");
    images[IMG_BACKGROUND] = loadBMP("background.bmp");
    images[IMG_ABOUT] = loadBMP("about.bmp");
    images[IMG_NOTE] = loadBMP("note.bmp");
    images[IMG_EDAY] = loadBMP("event_day.bmp");
}

// �������� �����������
void unload_images()
{
    free(images[IMG_MAIN]);
    free(images[IMG_BACKGROUND]);
    free(images[IMG_ABOUT]);
    free(images[IMG_NOTE]);
    free(images[IMG_EDAY]);
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


// �������� �������� ������ �� ������ (0-11 --> ������, �������...)
const char* month_str(int m)
{
    return mts[m];
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

int day_of_week()
{
    struct tm time = { 0 };
    time.tm_year = y+2022 - 1900;
    time.tm_mon = m;
    time.tm_mday = 1;
    mktime(&time);
    return (time.tm_wday + 6) % 7;
}

// �������� ���� �������� ���� ����� ��������� y
void update_year()
{
    clear_year();

    char yearStr[5];
    sprintf(yearStr, "%d", y+2022);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(YEAR_TEXT_X, YEAR_TEXT_Y, yearStr);
}

// �������� ���� �������� ������ ����� �������� m
void update_month()
{
    clear_month();
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(MONTH_TEXT_X, MONTH_TEXT_Y, month_str(m));
}

// ���������� ��� ���������
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
       
        if(_abracadabra_cast(years[]);