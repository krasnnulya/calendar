





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
    images[IMG_WEEKENDS] = loadBMP("weekends.bmp");
    images[IMG_ADAY] = loadBMP("another_day.bmp");
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
    free(images[IMG_WEEKENDS]);
    free(images[IMG_ADAY]);
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

//Дни предыдущего месяца
int day_prev()
{
    struct tm time = { 0 };
    time.tm_year = y+2022 - 1900;
    time.tm_mon = m-1;
    time.tm_mday = 1;
    mktime(&time);
    return (time.tm_wday + 6) % 7;
}


//Дни следующего месяца
int day_next()
{
    struct tm time = { 0 };
    time.tm_year = y+2022 - 1900;
    time.tm_mon = m+1;
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
    int k = day_prev();
    int h = day_next();

    for(int i = 0; i < n; i++)
    {
        int dx = (DAY_W+DAY_DIST_X)*((i+s)%7);
        int dy = (DAY_H+DAY_DIST_Y)*((i+s)/7);
       
        if(years[y].months[i].amount != 0)
          putimage(PLANS_OFFSET_X+dx, PLANS_OFFSET_Y+dy, images[IMG_EDAY], 0); //выделение заметок
        
        if(s == 4 || s == 5)
          putimage(WDAY_X+dx, WDAY_Y+dy, images[IMG_WEEKENDS], 0); //выделение выходных
        
       /* int p;
        if(p == time.tm_mday)  //выделение текущего дня
           {
            today = 1;
            putimage(dx, dy, images[IMG_NDAY], 0);
           }*/
        for(int prev = 0; prev < n; prev++)
        {
          int dx = (DAY_W+DAY_DIST_X)*((prev+k)%7);
          int dy = (DAY_H+DAY_DIST_Y)*((prev+k)/7);
        }
        
          for(int next = 0; next < n; next++)
        {
          int dx = (DAY_W+DAY_DIST_X)*((next+h)%7);
          int dy = (DAY_H+DAY_DIST_Y)*((next+h)/7);
        }

        char num[3];
        sprintf(num, "%d %d %d", i+1, prev-_abracadabra_cast(i);