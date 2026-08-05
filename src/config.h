

// под ESP8266
// типовая схема подключения и пины под ESP8266 & ESP32 -> https://volna42.com/ru/scheme/

#define EPD_BUSY_PIN 2   // D4 - GPIO - 2 (+20кОм резистор на VCC)
#define EPD_RST_PIN  12  // D6 - GPIO - 12 (MISO) 
#define EPD_DC_PIN   0   // D3 - GPIO - 0
#define EPD_CS_PIN   15  // D8 - GPIO - 15 (CS) или опционально GND (-1)
#define EPD_CLK_PIN  -1  // Всегда D5 - GPIO 14 (SCLK) - SPI  
#define EPD_DIN_PIN  -1  // Всегда D7 - GPIO 13 (MOSI) - SPI

/*
 Драйвера могут частично подходить и для других дисплеев 
 если используется тот же контроллер но нужно правильно инициализировать
 ширину \ высоту (displayDriver->displayWidth \ displayDriver->displayHeight)
 
 Документацию от производителя на контроллеры e-ink дисплея (SSD1683 & UC8176)
 которые используют WaveShare & Weact можно найти в папке _DOCS 
*/

// проверенные преднастроенные библиотеки под конкретные дисплеи

// #define WAVESHARE_BW_42_UC8176       // Waveshare 4.2inch, REV 2.1 [B&W]
#define WAVESHARE_BW_42_SSD1683         // Waveshare 4.2inch, REV 2.2 | WeAct 4.2inch [B&W, 4-colors grayscale displays]
// #define WAVESHARE_RY_BW_42_UC8176    // Waveshare 4.2inch, REV 2.1 [B&W + Red or B&W + Yellow, 3-colors (separate buffers)]
// #define WAVESHARE_RY_BW_42_UC8176_B  // Waveshare 4.2inch, REV ??? - если при выводе WAVESHARE_RY_BW_42_UC8176 инвертирует цвета (фон красным \ отдельные элементы белым \ черным)
// #define HELTEC_BW_15_S810F           // SSD1683 Heltec 1.54inch 200x200 or WeAct 1.54 200x200 [B&W]
// #define WAVESHARE_R_BW_15_SSD1683    // Waveshare 1.54inch 200x200 [B&W + Red, 3-colors (separate buffers)]
