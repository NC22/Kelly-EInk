

#include "Arduino.h"


#include "KellyCanvas.h"


#include "data/image/cat_vampi.h"
#include "data/image/rumba_400x300_2b.h"
#include "data/fonts/KellyFontC18x18.h"
#include "data/fonts/KellyFontC44x44.h"
#include "data/fonts/pixelFont44x44.h"

#include "config.h"
#include "KellyEInk.h"

#if defined(WAVESHARE_BW_42_UC8176) 
    #include "KellyEInk_42_UC8176_BW.h"
#elif defined(WAVESHARE_BW_42_SSD1683)
    #include "KellyEInk_42_SSD1683_BW_2BIT.h"
#elif defined(WAVESHARE_RY_BW_42_UC8176)
    #include "KellyEInk_42_UC8176_RBW.h"
#elif defined(WAVESHARE_RY_BW_42_UC8176_B)
    #include "KellyEInk_42_UC8176_RBW_B.h"
#endif

KellyEInk * createDisplayDriver() {

    #if defined(WAVESHARE_BW_42_UC8176) 

        KellyEInk_42_UC8176_BW * displayDriver = new KellyEInk_42_UC8176_BW(EPD_BUSY_PIN, EPD_RST_PIN, EPD_DC_PIN, EPD_CS_PIN, EPD_CLK_PIN, EPD_DIN_PIN);

    #elif defined(WAVESHARE_BW_42_SSD1683)

        KellyEInk_42_SSD1683_BW_2BIT * displayDriver = new KellyEInk_42_SSD1683_BW_2BIT(EPD_BUSY_PIN, EPD_RST_PIN, EPD_DC_PIN, EPD_CS_PIN, EPD_CLK_PIN, EPD_DIN_PIN);

    #elif defined(WAVESHARE_RY_BW_42_UC8176)

        KellyEInk_42_UC8176_RBW * displayDriver = new KellyEInk_42_UC8176_RBW(EPD_BUSY_PIN, EPD_RST_PIN, EPD_DC_PIN, EPD_CS_PIN, EPD_CLK_PIN, EPD_DIN_PIN);

    #elif defined(WAVESHARE_RY_BW_42_UC8176_B)

        KellyEInk_42_UC8176_RBW_B * displayDriver = new KellyEInk_42_UC8176_RBW_B(EPD_BUSY_PIN, EPD_RST_PIN, EPD_DC_PIN, EPD_CS_PIN, EPD_CLK_PIN, EPD_DIN_PIN);

    #endif

    return displayDriver;
}


int getDevidedBy8Num(int x, bool upper = false) {

    if (upper) x = x + (8 - (x % 8));
    else x = x - (x % 8);
    return x;
}

KellyEInk * screenController = NULL;

void setup() {
  // put your setup code here, to run once:
  // Serial.begin(460800);
  Serial.begin(460800);
  delay(200);

  Serial.println(F("DISPLAY test start..."));
  // KellyEInk_15_SSD1683_BW screenController = KellyEInk_15_SSD1683_BW(EPD_BUSY_PIN, EPD_RST_PIN, EPD_DC_PIN, EPD_CS_PIN);
  
  screenController = createDisplayDriver();

  screenController->initPins();


  // тест 2битных картинок
  // пример сконвертировать можно через скрипты Волны42 - не обязательно на самом устройстве 
  // https://github.com/NC22/Volna42BW/tree/main/_uiTools 
  // скрипт _uiTools/PREVIEW___client_eink_uploader.html
  // основные настройки и картинка -> 2-bit per pixel -> предпросмотр -> скачать предпросмотр -> скачать текстовый массив (txt)

  screenController->displayInit(2); // переключаем в 2битный режим
  screenController->display(gImage_400x300b1);

  // на канвасе тоже можно рисовать в 2битном режиме при необходимости

  delay(1400);

  screenController->displayInit(); // инициализируем уже в обычном режиме - 1бит

  //pinMode(SS, INPUT);

  
  KellyCanvas canvas = KellyCanvas(screenController->displayWidth, screenController->displayHeight);

  // рисуем на канвасе

  // осторожно с оперативкой - 2bit режим на 400х300 - 30кб 
  // для ESP8266 в 2bit режиме у меня через progmem и флеш читатются большие картинки отдельно 
  canvas.setBitsPerPixel(1); 
  canvas.setRotate(0);
  canvas.clear();

  if (screenController->displayHeight == 200)  
  canvas.setFont(&font18x18Config);
  else 
  canvas.setFont(&font44x44Config);

  Serial.println(F("Show image array & some text..."));

  // текс и картинка через полную отрисовку
  canvas.drawString(20, 20, "Тест булочки", true);
  canvas.drawImage(20, 100, &cat_vampi_88x100bw_settings, true);
  
  
  if (screenController->displayHeight > 200) {

    // манипуляции с текстом & расчет ширины в пикселях
    int posX = 20;
    int posY = 200;
    int padding = 5;
    canvas.setFont(&pixelFont44x44Config);
    uText text = canvas.getUText("Тест шрифта");  // расчет уже с учетом нового шрифта

    canvas.drawRect(posX, posY, text.pixelWidth + padding * 2, pixelFont44x44Config.height, 1);
    
    canvas.drawStringUtext(posX + padding, posY - 6, text, false);

  }  

  int numBaseX = 120; int numBaseY = 100;
  int numMaxWidth = 80; int numMaxHeight = 46;

  canvas.drawString(numBaseX, numBaseY, "1", true);

  // отправляем команды инициализации дисплею 
  // при желании для некоторых контроллеров можно попробовать отловить на сколько успешно проходит отправка (они дают обратную связь по digitalRead(ENK_PIN_BUSY))
  // но в драйвере нет ассинхронности

  screenController->displayInit();

  // отправляем команду вывода
  Serial.println(F("Show image for array..."));
  screenController->display(canvas.bufferBW);
  
  // инициализация для частичного вывода

  screenController->displayInit(1, true);

  int partialPointerX = numBaseX;
  int partialPointerY = numBaseY;

  if (canvas.rotation == 90) {

      canvas.applyXYMods(partialPointerX, partialPointerY);

      int tmp = screenController->displayHeight;

      numMaxHeight = numMaxWidth;
      numMaxWidth = tmp;

      partialPointerX -= numMaxWidth;
  }

  // динамический частичный вывод на "полной" скорости 
  // координаты частичного вывода должны быть кратны 8
  int partialXstart = getDevidedBy8Num(partialPointerX); 
  int partialYstart = getDevidedBy8Num(partialPointerY);
  int partialXend = getDevidedBy8Num(partialPointerX + numMaxWidth); 
  int partialYend = getDevidedBy8Num(partialPointerY + numMaxHeight);

  for (int i = 2; i <= 30; i++)  {

    canvas.drawRect(numBaseX, numBaseY, numMaxWidth, numMaxHeight, false);
    canvas.drawString(numBaseX, numBaseY, String(i), true);

    screenController->displayPartial(canvas.bufferBW, partialXstart, partialYstart, partialXend, partialYend);
  }

  
  screenController->displayInit();
  delay(400);
  screenController->displayClear();

  Serial.println(F("Sleep..."));
  screenController->displaySleep();  
 // screenController.endPins();
}

void loop() {
  // put your main code here, to run repeatedly:

}
