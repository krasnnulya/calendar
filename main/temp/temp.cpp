




// Макросы определяют положения объектов
// Чтобы не вводить каждый раз вручную
#define CUR_MONTH_X 180
#define CUR_MONTH_Y 80

#define CUR_YEAR_X 180
#define CUR_YEAR_Y 120

#define DAYS_OFFSET_X 25
#define DAYS_OFFSET_Y 200

#define PLANS_OFFSET_X 40
#define PLANS_OFFSET_Y 160

#define CALENDAR_X 50
#define CALENDAR_Y 230

#define NOTE_X 50
#define NOTE_Y 600
#define NOTE_W 220
#define NOTE_H 40

#define NOTE_TEXT_X 50
#define NOTE_TEXT_Y 610

#define EDIT_X 280
#define EDIT_Y 600

#define NOTE_SIZE 24

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
   if (file != NULL)
   {
       char text[NOTE_SIZE];
       int m, d;

       while(fscanf(file,"%d:%d%:", &m, &d, text) > 0 && fgets(text, NOTE_SIZE, file) != NULL)
      {
         _abracadabra_cast(months[m].days[d]);