





// Текущие год, месяц и день
int GlobY = 0;
int GlobM = 5;
int GlobD = 0;

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
            years[i].months[j].amount++;
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
                if(years[i].months[j].amount != 0)
                {
                   for(int k = 0; k < n; k++)
                   {
                       if(strlen(years[i].months[j].days[k].note) != 0)
                       {
                           fprintf(file, "%d:%d:%d:%s\n", i, j, k, years[i].months[j].days[k].note);
                       }
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


/*void notes() {
    FILE *note = NULL;
    int number;
 
    note = fopen("D:/c/note.bin", "wb");
    if (note == NULL) {
        sprintf("Error opening file");
        getch();
        exit(ERROR_FILE_OPEN);
    }
 
    scanf("%d", &number);
    fwrite(&number, sizeof(int), 1, note);
 
    fclose(output);
    _getch();
}*/

// Получить название месяца по номеру (0-11 --> январь, февраль...)
const char* month_str(int GlobM)
{
    return mts[GlobM];
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
    time.tm_year = GlobY+2022 - 1900;
    time.tm_mon = GlobM;
    time.tm_mday = 1;
    mktime(&time);
    return (time.tm_wday + 6) % 7;
}


// Обновить поле текущего года новым значением GlobY
void update_year()
{
    clear_year();

    char yearStr[5];
    sprintf(yearStr, "%d", GlobY+2022);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(YEAR_TEXT_X, YEAR_TEXT_Y, yearStr);
}

// Обновить поле текущего месяца новым значение GlobM
void update_month()
{
    clear_month();
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(MONTH_TEXT_X, MONTH_TEXT_Y, month_str(GlobM));
}

//Возвращение текущей даты у компьютера
int today(int &year1, int &month1, int &day1)
{
   const time_t t1 = time(NULL);
   struct tm* aTm = localtime(&t1);
   year1 = aTm->tm_year+1900;
   month1 = aTm->tm_mon+1;
   day1 = aTm->tm_mday;
   return 0;
}


// Нарисовать дни календаря
void draw_days()
{
    clear_calendar();
    settextjustify(CENTER_TEXT, CENTER_TEXT);
   
    int n = days_in_month(GlobY+2022, GlobM);
    int s = day_of_week();
   // int p = today(GlobY, GlobM, GlobD);

    for(int i = 0; i < n; i++)
    {
        int dx = (DAY_W+DAY_DIST_X)*((i+s)%7);
        int dy = (DAY_H+DAY_DIST_Y)*((i+s)/7);
      
       
        if(_abracadabra_cast(years[GlobY].months[GlobM]);