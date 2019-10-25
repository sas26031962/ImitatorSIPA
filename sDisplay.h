/*
 Структура, описывающая OLED-дисплей
 и методы управления им в данном проекте
 */
#ifndef S_DISPLAY
#define S_DISPLAY
//========================================================================
//---------------------------- ПОДКЛЮЧАЕМЫЕ ФАЙЛЫ ------------------------
//========================================================================
 
//========================================================================
//-------------------------------- КОНСТАНТЫ -----------------------------
//========================================================================
#define TITLE "Pinger ver. 3.1.21" //Информация о программе

  
//========================================================================
//------------------------- ОБЪЯВЛЕНИЕ КЛАССОВ И СТРУКТУР ----------------
//========================================================================

// Структура, управляющая выводом данных на дисплей

struct sDisplay{
// 
//Атрибуты
//  
  static Adafruit_SSD1306 display;
  static void install();
  static void displaySelectIP();
  static void displayResult();  
};

//
// Методы
//
void sDisplay::install()
{
  display.begin(SSD1306_SWITCHCAPVCC);
  display.display();
  delay(200);

}//End of sDisplay::install()

void sDisplay::displaySelectIP()
{
/*
      //
      // Вывод изображения на  дисплей
      //
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      //Формирование текста
      display.println("Select remote IP");
      display.print("Key code:");
      display.println(cKey::Code);
      display.print("Addr:");//
      display.print(unit::addrX[0]);// 
      display.print(".");// 
      display.print(unit::addrX[1]);// 
      display.print(".");// 
      display.print(unit::addrX[2]);// 
      display.print(".");// 
      display.println(unit::addrX[3]);// 
      display.println(unit::uName);
      
     //Вывод на дисплей
      display.display();
*/      
}//End of sDisplay::displaySelectIP()

void sDisplay::displayResult()
{
    /*
    //---
    //Очистка дисплея
    display.clearDisplay();
    //Установка параметров дисплея
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    //Формирование текста
    display.println(TITLE);
    //--
    if(PingResult.statusX)
    {
     display.print("Addr:");//
     display.print(PingResult.addr[0]);// 
     display.print(".");// 
     display.print(PingResult.addr[1]);// 
     display.print(".");// 
     display.print(PingResult.addr[2]);// 
     display.print(".");// 
     display.println(PingResult.addr[3]);// 

     display.print("Ping#:");//
     display.print(PingResult.n);// 
     display.print(" Delay:");// 
     display.print(PingResult.t);// 
     display.println("ms");// 
     display.print("Bytes: ");// 
     display.print(PingResult.len);// 
     display.print(" TTL: ");// 
     display.println(PingResult.ttl);// 
    }
    else
    {
     display.print("Echo Rq failed: ");// 
     display.println(PingResult.statusVal);// 
     display.println("Press RESET to exit");// 
    }

    //Вывод на дисплей
    display.display();
    //--- 
  */
}//End of sDisplay::displayResult()
//--------------------------------------------------------------------------------
#endif
