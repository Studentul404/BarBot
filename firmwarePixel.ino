#include <Adafruit_NeoPixel.h>

#define LED_PIN 3
#define NUM_LEDS 16

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

String currentCommand = "";                   // Переменная для хранения текущей команды
uint32_t snakeColor = strip.Color(0, 255, 0); // Цвет змейки по умолчанию (зеленый)

void setup()
{
    strip.begin();
    strip.show();       // Инициализация всех пикселей в "выкл"
    Serial.begin(9600); // Инициализация последовательного соединения
}

void loop()
{
    // Проверка на наличие новой команды
    if (Serial.available())
    {
        currentCommand = Serial.readStringUntil('\n'); // Считываем команду
        currentCommand.trim();                         // Убираем лишние пробелы и символы новой строки
    }

    // Выполнение команды
    if (currentCommand == "snake")
    {
        snakeColor = strip.Color(0, 255, 0); // Зеленый цвет
        snakeWithTailFade(100, 8);           // Змейка с затуханием хвоста
    }
    else if (currentCommand == "fire")
    {
        fireEffect(50);
    }
    else if (currentCommand == "rainbow")
    {
        rainbowCycle(50);
    }
    else if (currentCommand == "ukraine")
    {
        standWithUkraine(100);
    }
    else if (currentCommand == "cocacola")
    {
        cocaColaEffect(200);
    }
    else if (currentCommand == "fanta")
    {
        fantaEffect(200);
    }
    else if (currentCommand == "sprite")
    {
        spriteEffect(200);
    }
    else if (currentCommand == "pineapple")
    {
        snakeColor = strip.Color(230, 175, 37); // Желтый цвет
        snakeWithTailFade(100, 8);
    }
    else if (currentCommand == "strawberry")
    {
        snakeColor = strip.Color(255, 0, 0); // Красный цвет
        snakeWithTailFade(100, 8);
    }
    else if (currentCommand == "peach")
    {
        snakeColor = strip.Color(255, 218, 185); // Персиковый цвет
        snakeWithTailFade(100, 8);
    }
    else if (currentCommand == "kiwi")
    {
        snakeColor = strip.Color(0, 255, 0); // Зеленый цвет
        snakeWithTailFade(100, 8);
    }
    else if (currentCommand == "melon")
    {
        snakeColor = strip.Color(187, 246, 150); // Голубой цвет
        snakeWithTailFade(100, 8);
    }
    else if (currentCommand == "blackberry")
    {
        snakeColor = strip.Color(62, 9, 48); // Синий цвет
        snakeWithTailFade(100, 8);
    }
    else if (currentCommand == "caramel")
    {
        snakeColor = strip.Color(133, 70, 30); // Карамельный цвет
        snakeWithTailFade(100, 8);
    }
    else if (currentCommand == "choco")
    {
        snakeColor = strip.Color(75, 50, 19); // Шоколадный цвет
        snakeWithTailFade(100, 8);
    }
    else if (currentCommand == "cream")
    {
        snakeColor = strip.Color(240, 240, 255); // Голубовато-белый цвет
        snakeWithTailFade(100, 8);
    }
    else if (currentCommand == "milk") {
        milkEffect(200);
    } else 
    {
    }
}

// Змейка с затуханием хвоста
void snakeWithTailFade(uint8_t wait, uint8_t length)
{ // Задаем основной цвет (зеленый)
    int tailLength = length;                      // Длина хвоста

    for (int head = 0; head < strip.numPixels(); head++)
    {
        strip.clear(); // Очищаем все пиксели

        // Устанавливаем цвета для головы и хвоста
        for (int i = 0; i < tailLength; i++)
        {
            int pos = (head - i + strip.numPixels()) % strip.numPixels();           // Расчет позиции с учетом кругового кольца
            uint32_t dimmedColor = fadeColor(snakeColor, 255 - (i * (255 / tailLength))); // Уменьшение яркости для хвоста
            strip.setPixelColor(pos, dimmedColor);
        }

        strip.show();
        delay(wait);
    }
}

// Функция для уменьшения яркости цвета
uint32_t fadeColor(uint32_t color, uint8_t fadeAmount)
{
    byte r = (color >> 16) & 0xFF;
    byte g = (color >> 8) & 0xFF;
    byte b = color & 0xFF;

    r = (r * fadeAmount) / 255;
    g = (g * fadeAmount) / 255;
    b = (b * fadeAmount) / 255;

    return strip.Color(r, g, b);
}

// Огонь
void fireEffect(uint8_t wait)
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        strip.setPixelColor(i, strip.Color(255, random(50, 150), 0)); // Красный с оттенками
    }
    strip.show();
    delay(wait);
}

// Радуга
void rainbowCycle(uint8_t wait)
{
    uint16_t i, j;
    for (j = 0; j < 256; j++)
    { // Полный цикл всех цветов
        for (i = 0; i < strip.numPixels(); i++)
        {
            strip.setPixelColor(i, Wheel((i * 256 / strip.numPixels() + j) & 255));
        }
        strip.show();
        delay(wait);
    }
}

// Колесо цветов для эффекта радуги
uint32_t Wheel(byte WheelPos)
{
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85)
    {
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    else if (WheelPos < 170)
    {
        WheelPos -= 85;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    else
    {
        WheelPos -= 170;
        return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
}

void standWithUkraine(uint8_t wait)
{
    int halfLEDs = NUM_LEDS / 2;
    for (int offset = 0; offset < 16; offset++)
    {
        strip.clear();
        for (int i = 0; i < halfLEDs; i++)
        {
            if ((i + offset) % NUM_LEDS < halfLEDs)
            {
                strip.setPixelColor(i, strip.Color(255, 255, 0)); // Желтый цвет
            }
            else
            {
                strip.setPixelColor(i, strip.Color(0, 0, 255)); // Синий цвет
            }
        }
        for (int i = 8; i < 16; i++)
        {
            if ((i + offset) % NUM_LEDS < halfLEDs)
            {
                strip.setPixelColor(i, strip.Color(255, 255, 0)); // Желтый цвет
            }
            else
            {
                strip.setPixelColor(i, strip.Color(0, 0, 255)); // Синий цвет
            }
        }
        strip.show();
        delay(wait);
    }
}

// Эффект CocaCola (красный пиксель через одну)
// Эффект CocaCola (красный пиксель через одну) с вращением
void cocaColaEffect(uint8_t wait)
{
    for (int offset = 0; offset < NUM_LEDS; offset++)
    {
        strip.clear();
        for (int i = 0; i < NUM_LEDS; i++)
        {
            if ((i + offset) % 2 == 0)
            {
                strip.setPixelColor(i, strip.Color(255, 0, 0)); // Красный цвет
            }
        }
        strip.show();
        delay(wait);
    }
}

// Эффект Fanta (оранжевый пиксель через одну) с вращением
void fantaEffect(uint8_t wait)
{
    for (int offset = 0; offset < NUM_LEDS; offset++)
    {
        strip.clear();
        for (int i = 0; i < NUM_LEDS; i++)
        {
            if ((i + offset) % 2 == 0)
            {
                strip.setPixelColor(i, strip.Color(255, 102, 0)); // Оранжевый цвет
            }
        }
        strip.show();
        delay(wait);
    }
}

void milkEffect(uint8_t wait)
{
    for (int offset = 0; offset < NUM_LEDS; offset++)
    {
        strip.clear();
        for (int i = 0; i < NUM_LEDS; i++)
        {
            if ((i + offset) % 2 == 0)
            {
                strip.setPixelColor(i, strip.Color(200, 200, 200)); // Белый цвет
            }
        }
        strip.show();
        delay(wait);
    }
}


// Эффект Sprite (зелено-белый через одну) с вращением
void spriteEffect(uint8_t wait)
{
    for (int offset = 0; offset < NUM_LEDS; offset++)
    {
        strip.clear();
        for (int i = 0; i < NUM_LEDS; i++)
        {
            if ((i + offset) % 2 == 0)
            {
                strip.setPixelColor(i, strip.Color(0, 255, 0)); // Зеленый цвет
            }
            else
            {
                strip.setPixelColor(i, strip.Color(255, 255, 255)); // Белый цвет
            }
        }
        strip.show();
        delay(wait);
    }
}