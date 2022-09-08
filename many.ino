#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <DS1302.h>
#define PIN_RELAY 3
#define PIN_PHOTO_SENSOR A0
#define PIN_LED 4
#define PIN_PHOTO_SENSOR A0

DHT dht(2, DHT11);
DS1302 rtc(8, 9, 10);                                //инициализация платы DS1302
Time t;                                              //объявляем ссылку на класс time
LiquidCrystal_I2C lcd(0x27, 16, 2);                  //Устанавливаем ЖК-дисплей по адресу 0x27, 16 символов и 2 строки

void setup()
{
  lcd.begin(16, 2);                                  // Инициируем монитор
  lcd.init();                                        //Включаем подсветку дисплея
  lcd.backlight();                                   //Устанавливаем курсор на первую строку и нулевой символ.
  Serial.begin(9600);                                // подключаемся к порту
  dht.begin();                                       //инициализируем датчик влажности и температуры
  rtc.halt(false);                                   //инициализируем пины питания
  rtc.writeProtect(false);                           //Установим время, и отключить защиту от записи
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, HIGH);
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);
  // Если у вас в DS1302 уже сохранено установленное время, то нужно за комментировать
/*  rtc.setDOW(FRIDAY);                                // Устанавливаем день недели FRIDAY
  rtc.setTime(21, 6, 10);                           // Устанавливаем время 20:30:10 (24часа формат времени)
  rtc.setDate(5, 10, 2018);                          // Устанавливаем число месяц и год
*/
}

void loop()
{
  int val = analogRead(PIN_PHOTO_SENSOR);
  if (val < 800) {
    // Светло, выключаем реле
    digitalWrite(PIN_RELAY, HIGH);
        digitalWrite(PIN_LED, LOW);
  } else {
    // Темновато, включаем лампочку
    digitalWrite(PIN_RELAY,  LOW);
      digitalWrite(PIN_LED, HIGH);
  }
  t = rtc.getTime();
  lcd.setCursor(0, 0);                               // Переносим курсор на первую строку
  lcd.print(rtc.getDOWStr());                        // Выводим день недели
  lcd.print(" ");                                    // Отступ
  lcd.println(rtc.getTimeStr());                     // Выводим время
  lcd.print(" ");                                    // Отступ
  lcd.setCursor(0, 1);                               // Переносим курсор на следующую строку
  lcd.print(rtc.getDateStr());                       // Выводим дату
  lcd.print(" ");                                    // Отступ

  Serial.print(rtc.getDOWStr());                     // Выводим день недели
  Serial.print(" ");                                 // Отступ
  Serial.print(rtc.getDateStr());                    // Выводим дату
  Serial.print(" -- ");                              // Отступ
  Serial.println(rtc.getTimeStr());                  // Выводим время

  delay (1000);                                      // Ставим задержку в 1 секунду
  lcd.clear();                                       //очищаем монитор

  float h = dht.readHumidity();                      //Измеряем влажность
  float t = dht.readTemperature();                   //Измеряем температуру
  if (isnan(h) || isnan(t)) {                        // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
    Serial.println("Ошибка считывания");
    return;
  }

  //выводим на лсд монитор
  lcd.setCursor(0, 0);                               // Переносим курсор на первую строку
  lcd.print(h);                                      // Выводим день недели
  lcd.print(" % ");                                  // Отступ
  lcd.print(t);                                      // Выводим дату
  lcd.print(" C ");                                  // Отступ
  lcd.setCursor(0, 1);                               // Переносим курсор на первую строку
  lcd.print(analogRead(PIN_PHOTO_SENSOR));           // Выводим день недели
  lcd.print(" lm ");                                 // Отступ
  //Выводим на монитор порта
  Serial.print("Влажность: ");
  Serial.print(h);
  Serial.print(" %");
  Serial.print("Температура: ");
  Serial.print(t);
  Serial.println(" *C ");                             //Вывод показателей на экран

  delay (1000);                                       // Ставим задержку в 1 секунду
  lcd.clear();                                        //очищаем монитор
}
