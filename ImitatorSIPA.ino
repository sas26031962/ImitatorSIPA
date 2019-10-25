/*
 Программа для имитации СИПА
 Собственный адрес платы: 192.168.1.4 (задаётся массивом ip)
 Адрес управляющего узла: 192.168.1.1 (задаётся массивом ip0)

 */
 
//=====================================================================
// Подключаемые модули
//=====================================================================
#include <SPI.h>         
#include <Ethernet.h>
#include <EthernetUdp.h>        
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "cCommunication.h"
#include "Global.h"
#include "config.h"
#include "sDisplay.h"

//=====================================================================
// Аппаратная конфигурация
//=====================================================================
// Область для управления OLED эканом
#define OLED_MOSI   22
#define OLED_CLK    24
#define OLED_CS     26
#define OLED_DC     28
#define OLED_RESET  30

#define KEY_1 7   // UP
#define KEY_2 6   // DOWN
#define KEY_3 5   // SELECT
#define KEY_4 8   // RESTART

//=====================================================================
// Константы
//====================================================================
#define REPEATE_TIME 500 // время повтора посылок

#define BUFFER_SIZE 26  // размер буфера для передачи
#define PACKET_LENGTH 32 // размер буфера для приёма

//=====================================================================
// Глобальные переменные
//=====================================================================
//
// IP адреса передатчика и приёмника
//
char ip0[4] = {192, 168, 1, 4}; 
char ip1[4] = {192, 168, 1, 1}; 

IPAddress local_ip(ip0[0], ip0[1], ip0[2], ip0[3]);
IPAddress remote_ip(ip1[0], ip1[1], ip1[2], ip1[3]);

//
// Локальный порт, который слушает программа
// 
unsigned int localPort = 25411;      
unsigned int destinationPort = 25104;      

//
// Буфера для приёма и передачи данных
//
char packetBuffer[BUFFER_SIZE];  // буфер для хранения входящего пакета,

char AnswerBuffer[BUFFER_SIZE];
//
// Экземпляр класса для реализации передачи и приёма информации по протоколу UDP
//
EthernetUDP Udp;

// 
// Параметры принятого пакета
//
int packetSize = 0;                             // размер принятого пакета

//
// Число принятых пакетов
//
int SinkPacketCounter = 0;
int SendPacketCounter = 0;


// Переменные для управления потоком по времени
long CurrentTime;
long PreviousTime;

cCommunication Com;//Коммуникационная составляющая

//
//Область сетевых настроек
//
byte Global::mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE};


Adafruit_SSD1306 sDisplay::display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

byte CheckSumView = 0;

//=====================================================================
// Начальная установка
//=====================================================================
void setup() 
{
  //
  // Инициализация микросхемы Ethernet
  //
  Ethernet.begin(Global::mac, local_ip);     //Запуск Ethernet
  Udp.begin(localPort);                      // Запуск UDP на приём

  // Инициализация кнопок управления
  pinMode(KEY_1, INPUT_PULLUP);  
  pinMode(KEY_2, INPUT_PULLUP);  
  pinMode(KEY_3, INPUT_PULLUP);  
  pinMode(KEY_4, INPUT_PULLUP);  

  //Запуск программного таймера
  CurrentTime = millis();
  PreviousTime = CurrentTime;

  // Инициализация
  sDisplay::display.begin(SSD1306_SWITCHCAPVCC);
  
  //
  // Отображение картинки по умолчанию
  // 
  sDisplay::display.display();
  delay(200);

  viewData();
  
}//End of void setup()

//=====================================================================
// Главный цикл
//=====================================================================
void loop()
{
  CurrentTime = millis();
  if(CurrentTime - PreviousTime > REPEATE_TIME)
  { 
    PreviousTime = CurrentTime;
    if(true)
    { 
      // Подготовка пакета данных
      prepareDataToSend();
      //Передача пакета
      Udp.beginPacket(remote_ip, destinationPort);
      Udp.write( AnswerBuffer,PACKET_LENGTH);
      Udp.endPacket();
    
      //Подсчёт переданных пакетов
      SendPacketCounter++;
    }

    viewData();
    
  }//End of if(CurrentTime - PreviousTime > REPEATE_TIME)

  // Приём пакета из буфера
  packetSize = Udp.parsePacket();
  
  // 
  // Разбор принятого пакета
  //
  if (packetSize > 0) 
  { 

    SinkPacketCounter++;// Подсчёт пакетов
    Udp.read(packetBuffer, PACKET_LENGTH);

    // Визуализация принятых данных
    
    
    // Очистка буфера пакетов
    for(int i = 0; i < PACKET_LENGTH; i++) packetBuffer[i] = ' ';
  }

}//End of void loop()

//=====================================================================
// Подпрограммы
//=====================================================================
//
// Вывод на дисплей
//
void viewData()
{
  //Очистка дисплея
  sDisplay::display.clearDisplay();
  //Установка параметров дисплея
  sDisplay::display.setTextSize(1);
  sDisplay::display.setTextColor(WHITE);
  sDisplay::display.setCursor(0,0);
  //Формирование текста
  sDisplay::display.println("ImitatorSIPA v1.0.0");
  sDisplay::display.print("LocalIP:");//
  sDisplay::display.println("192.168.1.4");// 
  sDisplay::display.print("Send:");// 
  sDisplay::display.println(SendPacketCounter);// 
  sDisplay::display.print("CheckSum:");// 
  sDisplay::display.println(CheckSumView);// 
  //Вывод на дисплей
  sDisplay::display.display();
}//End of void viewData()

/// <summary>
/// Подготовка данных для передачи
/// </summary>
void prepareDataToSend()
{
  //
  // Формирование ответного пакета
  //        
  int CheckSum = 0;
  //-----------------------------------
  //Формирование ответного пакета
  AnswerBuffer[0] = 2;//Номер версии
  AnswerBuffer[1] = 0;//Команда
  AnswerBuffer[2] = 0;//Код ошибки принятого сообщения
        
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //Период усреднения
  AnswerBuffer[3 + 0] = 1;//!!!this.SIPA.btAverage;
  //Период передачи
  AnswerBuffer[3 + 1] = 2;//!!!this.SIPA.btRequest;
  //Температура
  AnswerBuffer[3 + 2] = 3;//this.SIPA.Temperature.Byte0; 
  AnswerBuffer[3 + 3] = 17;//this.SIPA.Temperature.Byte1;
  //Влажность
  AnswerBuffer[3 + 4] = 5;//this.SIPA.Humidity.Byte0; 
  AnswerBuffer[3 + 5] = 53;//this.SIPA.Humidity.Byte1;
  //Атмосферное давление
  AnswerBuffer[3 + 6] = 0;//PRESSURE0;
  AnswerBuffer[3 + 7] = 3;//PRESSURE1;
  AnswerBuffer[3 + 8] = 200;//PRESSURE2;//this.SIPA.Pressure.Byte2;
  AnswerBuffer[3 + 9] = 3;//PRESSURE3;//this.SIPA.Pressure.Byte3;
  //Средняя скорость ветра
  AnswerBuffer[3 + 10] = 0;//!!!this.SIPA.SpeedAverage.Byte0;
  AnswerBuffer[3 + 11] = 0;//!!!this.SIPA.SpeedAverage.Byte1;
  //Минимальная скорость ветра
  AnswerBuffer[3 + 12] = 0;//!!!this.SIPA.SpeedMin.Byte0;
  AnswerBuffer[3 + 13] = 0;//!!!this.SIPA.SpeedMin.Byte1;
  //Максимальная скорость ветра
  AnswerBuffer[3 + 14] = 0;//!!!this.SIPA.SpeedMax.Byte0;
  AnswerBuffer[3 + 15] = 0;//!!!this.SIPA.SpeedMax.Byte1;
  //Среднее направление ветра
  AnswerBuffer[3 + 16] = 0;//!!!this.SIPA.DirectionAverage.Byte0;
  AnswerBuffer[3 + 17] = 0;//!!!this.SIPA.DirectionAverage.Byte1;
  //Минимальное направление ветра
  AnswerBuffer[3 + 18] = 0;//!!!this.SIPA.DirectionMin.Byte0;
  AnswerBuffer[3 + 19] = 0;//!!!this.SIPA.DirectionMin.Byte1;
  //Максимальное направление ветра
  AnswerBuffer[3 + 20] = 0;//!!!this.SIPA.DirectionMax.Byte0;
  AnswerBuffer[3 + 21] = 0;//!!!this.SIPA.DirectionMax.Byte1;
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  CheckSum = 0;
  for(int i = 0; i < BUFFER_SIZE - 1; i++)
  {
    CheckSum += AnswerBuffer[i];
  }
  AnswerBuffer[25] = (byte)(CheckSum & 255);

  CheckSumView = (byte)(CheckSum & 255);
      
}//End of void prepareDataToSend()
    

