




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

#define NOTE_X 474
#define NOTE_Y 52
#define NOTE_W 765
#define NOTE_H 106

#define NOTE_TEXT_X 25
#define NOTE_TEXT_Y 590

#define EDIT_X 280
#define EDIT_Y 550

#define NOTE_SIZE 50

// Изображения
IMAGE *images[9];

// Индексы в массиве
enum IMAGE_INDEX{  IMG_ADAY,  IMG_BACKGROUND,   IMG_DATA,
   IMG_EDAY,   IMG_NDAY,   IMG_WEEKENDS,   IMG_ABOUT,   IMG_EDIT,
   IMG_THESE};
   
// Запрос текущей даты
dat = tm.tm_mday;
weekday = _abracadabra_cast(tm);