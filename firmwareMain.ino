// Libraries
#include <LiquidCrystal.h>
#include <Servo.h>

// Pinouts
#define NUM_LEDS 16
#define analogPin A5
#define ForceSensor A0
#define termometerPin A4

// Version
#define VERSION "BARBOTOS v1.0.5"

// Initial values
short iceCount = 20;
short iceUsed = 0;
short totalDrinkVolume = 0;

// LCD
int lcd_old = 0;

// Flags
bool isContinue = true;

// Menu
short fruitId = 1;
short normalWidth = 15;

// Drinks
const char *drinks[4] = {"Milkshake", "Coca-Cola", "Fanta", "Sprite"};
short drink[7] = {250, 0, 0, 0, 0, 0, 0};

// Drinks consumed
short drinksConsumed[4] = {0, 0, 0, 0};

// Animations
const char *fruits[6] = {"pineapple", "strawberry", "peach", "kiwi", "melon", "blackberry"};
const char *animations[4] = {"rainbow", "snake", "ukraine", "fire"};
short animationId = 0;
// Temperature
short temps[16] = {};
class BarBotInterface
{
public:
    BarBotInterface(uint8_t rs, uint8_t en, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
        : lcd(rs, en, d4, d5, d6, d7) {}

    void begin()
    {
        lcd.begin(16, 2);
        createCustomChars();
        lastButtonId = 0;
    }

    void setXY(int x, int y)
    {
        lcd.setCursor(x, y);
    }

    /**
     * Asks user to input a number within specified range with step and returns it
     * 
     * @param question The question to ask user
     * @param min Minimum value of the number
     * @param max Maximum value of the number
     * @param step The step of the number. Default is 1
     * @return The number entered by user
     */
    short askNumber(const char *question, int min, int max, int step = 1)
    {
        lcd.clear();
        lcd.print(question);
        lcd.setCursor(0, 1);
        short oldnum = -1;
        short num = min;
        bool isConfirmed = false;

        while (!isConfirmed)
        {
            int buttonId = getButtonId();
            if (buttonId == 7)
            { // Up button
                if (num < max)
                {
                    num += step;
                }
            }
            else if (buttonId == 6)
            { // Down button
                if (num > min)
                {
                    num -= step;
                }
            }
            else if (buttonId == 5)
            { // OK button
                isConfirmed = true;
            }
            if (num != oldnum)
            {
                lcd.setCursor(0, 1);
                lcd.print(num);
                oldnum = num;
            }
            delay(20); // Debouncing delay
        }
        return num;
    }

    /**
     * Prompts the user with a yes or no question and waits for input.
     *
     * This function displays the given question on the LCD screen and presents
     * the options "Yes" and "No". It waits for the user to press a button to
     * select their choice.
     *
     * @param question The yes/no question to display on the LCD.
     * @return Returns true if the user selects "Yes", false if the user selects "No".
     */
    bool askYesNo(const char *question)
    {
        lcd.clear();
        lcd.print(question);
        lcd.setCursor(0, 1);
        lcd.print("Yes  |  No");

        while (true)
        {
            int buttonId = getButtonId();
            if (buttonId == 6)
            { // OK button
                lcd.setCursor(0, 1);
                lcd.print("Yes");
                return true;
            }
            else if (buttonId == 7)
            { // Menu button
                lcd.setCursor(0, 1);
                lcd.print("No ");
                return false;
            }
            delay(200);
        }
    }

    /**
     * Prompts the user with a multiple choice question and waits for input.
     *
     * This function displays the given question on the LCD screen and presents
     * the options provided by the caller. It waits for the user to press a button
     * to select their choice. The caller can provide up to 5 options.
     *
     * @param question The question to display on the LCD.
     * @param options An array of strings representing the options to display.
     * @param optionCount The number of options provided in the options array.
     * @return The index of the selected option in the options array.
     */
    short askVariant(const char *question, const char *options[], int optionCount)
    {
        lcd.clear();
        lcd.print(question);
        short currentOption = 0;
        displayOption(options, currentOption, optionCount);

        while (true)
        {
            short oldOption = currentOption;
            int buttonId = getButtonId();
            if (buttonId == 6)
            { // Left button
                if (currentOption > 0)
                {
                    currentOption--;
                }
            }
            else if (buttonId == 7)
            { // Right button
                if (currentOption < optionCount - 1)
                {
                    currentOption++;
                }
            }
            else if (buttonId == 5)
            { // OK button
                return currentOption;
            }
            if (currentOption != oldOption)
            {
                displayOption(options, currentOption, optionCount);
            }
            delay(25);
        }
    }

    short askDrink()
    {
        lcd.clear();
        write("Choose drink on");
        lcd.setCursor(0, 1);
        write("buttons ->");
        while (true)
        {
            int buttonId = getButtonId();
            if (buttonId >= 1 && buttonId <= 4)
            {
                Serial.println(buttonId--);
                return buttonId--;
            }
            else if (buttonId == 8)
            {
                askSettings();
                break;
            }
            else if (buttonId == 6)
            {
                isContinue = false;
                break;
            }
            delay(15);
        }
        return -1;
    }

    void cls()
    {
        lcd.clear();
    }

    void write(const char *message)
    {
        lcd.print(message);
    }

    void writeChar(short i)
    {
        lcd.write(i);
    }

/**
 * Displays the current option on the LCD screen.
 *
 * This function positions the cursor and prints the current option in the
 * provided options array onto the LCD screen. It also displays arrow indicators
 * if there are previous or subsequent options to the current selection.
 *
 * @param options An array of option strings to be displayed.
 * @param currentOption The index of the currently selected option in the options array.
 * @param optionCount The total number of options available in the options array.
 */
    void displayOption(const char *options[], short currentOption, int optionCount)
    {
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(4, 1);
        lcd.print(options[currentOption]);
        if (currentOption > 0)
            lcd.setCursor(0, 1), lcd.write('<');
        if (currentOption < optionCount - 1)
            lcd.setCursor(15, 1), lcd.write('>');
        delay(100);
    }


    /**
     * Displays the filling progress of a drink on the LCD screen.
     *
     * This function displays the name of the drink and a progress bar
     * indicating the filling progress on the LCD screen. It is used to
     * display the progress of pouring a drink.
     *
     * @param drinkName The name of the drink being poured.
     * @param pouringTime The time required to pour the drink in milliseconds.
     */
    void displayFillingProgress(const char *drinkName, unsigned int pouringTime)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        if (drinkName != "Calibration")
            lcd.write(5);
        else
            lcd.write(4);
        lcd.print("  ");
        if (drinkName != "fruit")
        {
            lcd.print(drinkName);
        }
        else
        {
            String fruitName = fruits[fruitId];
            fruitName[0] = toupper(fruitName[0]);
            lcd.print(fruitName);
            lcd.print(" ");
            lcd.print("");
        }

        short intervals = pouringTime / 16;

        for (int i = 0; i < 16; i++)
        {
            lcd.setCursor(i, 1);
            lcd.write(6); // выводим символ заполнения
            delay(intervals);
        }
    }

    bool checkGlass()
    {
        for (int i = 0; i < 15; i++)
        {
            temps[i] = temps[i + 1];
        }
        temps[15] = getTemperature();
        return getWeight() > normalWidth;
    }

    /**
     * Reads the temperature from the thermometer pin and maps it to a reasonable
     * temperature range in degrees Celsius. The formula used is based on the
     * assumption that the thermometer is a thermistor with a B value of 3950.
     *
     * @return The current temperature in degrees Celsius as a short.
     */
    short getTemperature()
    {
        return map(((analogRead(termometerPin) - 20) * 3.04), 0, 1023, -40, 125);
    }

private:
    LiquidCrystal lcd;
    short lastButtonId;
    void createCustomChars()
    {
        uint8_t HeartChar[] = {0x00, 0x00, 0x0a, 0x15, 0x11, 0x0a, 0x04, 0x00};
        uint8_t GlassChar[] = {0x00, 0x1f, 0x1f, 0x1f, 0x0e, 0x04, 0x04, 0x0e};
        uint8_t fullChar[] = {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f};
        uint8_t gradusChar[] = {0x07, 0x05, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00};
        byte waitChar[] = {
            0x1F,
            0x0E,
            0x0E,
            0x04,
            0x04,
            0x0E,
            0x0E,
            0x1F};

        lcd.createChar(1, HeartChar);
        lcd.createChar(2, gradusChar);
        lcd.createChar(4, waitChar);
        lcd.createChar(5, GlassChar);
        lcd.createChar(6, fullChar);
    }

    /**
     * Reads the value from the analog pin and determines which button (if any)
     * is pressed. If a button is pressed, the corresponding button ID is
     * returned. If a button is released, the previously pressed button ID is
     * returned. If no button is pressed, 0 is returned. If a button is still
     * being held, -1 is returned.
     *
     * @return The ID of the pressed button, the previously pressed button, or
     *         -1 if a button is still being held.
     */
    short getButtonId()
    {
        short buttonId = 0;
        short sensorValue = analogRead(analogPin); // Чтение аналогового значения с пина

        // Сравнение значения с порогами для каждой кнопки
        if (sensorValue > 900)
        {
            buttonId = 4; // Кнопка Sprite
        }
        else if (sensorValue > 750)
        {
            buttonId = 3; // Кнопка Coca-Cola
        }
        else if (sensorValue > 600)
        {
            buttonId = 2; // Кнопка Fanta
        }
        else if (sensorValue > 400)
        {
            buttonId = 1; // Кнопка Milkshake
        }
        else if (sensorValue > 150)
        {
            buttonId = 5; // Кнопка OK
        }
        else if (sensorValue > 50)
        {
            buttonId = 6; // Кнопка влево
        }
        else if (sensorValue > 28)
        {
            buttonId = 7; // Кнопка вправо
        }
        else if (sensorValue > 5)
        {
            buttonId = 8;
        }
        else
        {
            buttonId = 0; // Нет нажатия
        }

        // Проверяем, отпущена ли кнопка
        if (buttonId == 0 && lastButtonId != 0)
        {
            short releasedButton = lastButtonId; // Запоминаем отпущенную кнопку
            lastButtonId = buttonId;             // Обновляем состояние
            return releasedButton;               // Возвращаем код отпущенной кнопки
        }

        // Обновляем состояние последней нажатой кнопки
        lastButtonId = buttonId;

        // Если кнопка не отпущена, возвращаем -1
        return -1;
    }

    /**
     * Opens the settings menu, allowing the user to configure the ice count,
     * calibrate the sensors, set the glass volume, select an animation, select
     * a topping, view the info menu, or exit the settings menu.
     *
     * The user is presented with a menu of options and is allowed to select
     * one of the options using the buttons. The selected option is executed
     * and the user is returned to the menu. The user can exit the menu by
     * selecting the "Exit" option.
     */
    void askSettings()
    {
        short menu = 0;
        const char *settings[7] = {"Ice count", "Calibrate", "Volume", "Animation", "Topping", "Info", "Exit"};
        while (menu != 5)
        {
            menu = askVariant("Select setting:", settings, 7);
            switch (menu)
            {
            case 0:
                iceCount = askNumber("Ice count", 0, 50, 1);
                break;
            case 1:
                calibrate();
                break;
            case 2:
                drink[0] = askNumber("Glass volume in ml?", 100, 500, 25);
                break;
            case 3:
                animationId = askVariant("Select animation:", animations, 5);
                break;
            case 4:
                fruitId = askVariant("Select fruit", fruits, 6);
                break;
            case 5:
                infoMenu();
                break;
            case 6:
                lcd.clear();
                return;
            }
        }
    }

    /**
     * Shows a menu with information about the barbot.
     *
     * The user is presented with a menu of options and is allowed to select
     * one of the options using the buttons. The selected option is executed
     * and the user is returned to the menu. The user can exit the menu by
     * selecting the "Exit" option.
     */
    void infoMenu()
    {
        lcd.clear();
        while (true)
        {
            const char *variants[7] = {"Ice", "Volume", "Statistic", "Temperature", "Version", "Back", "Exit"};
            short menu = askVariant("Get info about:", variants, 7);
            if (menu == 0)
            {
                if (iceCount != 0)
                {
                    lcd.clear();
                    lcd.print("Ice count: ");
                    lcd.print(iceCount);
                }
                else
                {
                    lcd.clear();
                    lcd.print("No ice in box!");
                }
                delay(5000);
            }
            else if (menu == 1)
            {
                showVolume();
            }
            else if (menu == 2)
            {
                statistic();
            }
            else if (menu == 3)
            {
                temperature();
            }
            else if (menu == 4)
            {
                lcd.clear();
                lcd.print("Version: ");
                lcd.setCursor(0, 1);
                lcd.print(VERSION);
                delay(5000);
            }
            else if (menu == 5)
            {
                return;
            }
            else if (menu == 6)
            {
                isContinue = false;
                return;
            }
        }
    }

    /**
     * Shows the current glass volume on the display and waits 2.5 seconds.
     */
    void showVolume()
    {
        lcd.clear();
        lcd.print("Volume: ");
        lcd.print(drink[0]);
        lcd.write(" ");
        lcd.print("ml");
        delay(2500);
    }
    /**
     * Starts the calibration process of the weight sensor.
     *
     * The process consists of the following steps:
     * 1. Clears the display and shows "Calibration:".
     * 2. Waits 500ms.
     * 3. Clears the display and shows "Please remove glass".
     * 4. Waits 1000ms.
     * 5. Clears the display and measures the current weight.
     * 6. Shows the filling progress bar with the label "Calibration".
     * 7. Prints "Calibration done!" on the display.
     * 8. Waits 1000ms.
     */
    void calibrate()
    {
        lcd.clear();
        lcd.print("Calibration:");
        delay(500);
        lcd.clear();
        lcd.print("Please remove glass");
        delay(1000);
        lcd.clear();
        normalWidth = getWeight();
        displayFillingProgress("Calibration", 2500);
        lcd.print("Calibration done!");
        delay(1000);
        return;
    }

    /**
     * Shows the current temperature on the display and
     * waits until the cancel button is pressed.
     * While the temperature is being displayed, it
     * checks if a glass is present.
     */
    void temperature()
    {
        while (true)
        {
            lcd.clear();
            lcd.print("Temp: ");
            lcd.print(getTemperature());
            lcd.write(" ");
            lcd.write(2);
            lcd.print("C");
            bool isGlass = checkGlass();
            delay(1000);
            if (getButtonId() == 8)
            {
                break;
            }
        }
        return;
    }

    /**
     * Shows different statistics about the consumed drinks
     * and temperature.
     *
     * Statistics:
     * - Number of consumed drinks
     * - Volume of consumed drinks
     * - Number of used ice cubes
     * - Number of consumed milkshakes, sprite and fanta and
     *   coca-cola
     * - Average temperature of the last 16 temperature
     *   measurements
     * - Back to main menu
     */
    void statistic()
    {
        while (true)
        {
            const char *stats[6] = {"Drinks", "Ice used", "MS & Sprites", "Fanta & Cola", "Temperature", "Back"};
            short menu = askVariant("Statistic of:", stats, 6);
            if (menu == 0)
            {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Drinks: ");
                lcd.print(drinksConsumed[0] + drinksConsumed[1] + drinksConsumed[2] + drinksConsumed[3]);
                lcd.setCursor(0, 1);
                lcd.print("Volume: ");
                lcd.print(totalDrinkVolume);
                delay(5000);
            }
            else if (menu == 1)
            {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Ice used: ");
                lcd.print(iceUsed);
                delay(5000);
            }
            else if (menu == 2)
            {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Milshakes: ");
                lcd.print(drinksConsumed[0]);
                lcd.setCursor(0, 1);
                lcd.print("Sprite: ");
                lcd.print(drinksConsumed[3]);
                delay(5000);
            }
            else if (menu == 3)
            {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Fanta: ");
                lcd.print(drinksConsumed[1]);
                lcd.setCursor(0, 1);
                lcd.print("Coca-Cola: ");
                lcd.print(drinksConsumed[2]);
                delay(5000);
            }
            else if (menu == 4)
            {
                lcd.clear();
                short sumTemp = 0;
                for (int i = 0; i < 16; i++)
                {
                    sumTemp += temps[i];
                }
                lcd.setCursor(0, 0);
                lcd.print("Avrg temp: ");
                lcd.print(sumTemp / 60);
                lcd.write(" ");
                lcd.write(2);
                lcd.print("C");
                delay(5000);
            }
            else if (menu == 5)
            {
                break;
            }
        }
        return;
    }
};


/*
CustomHC595(): Default constructor that does nothing.
CustomHC595(int latchPin, int dataPin, int clockPin): Constructor that initializes the shift register with the specified latch, data, and clock pins.
begin(int dataPin, int latchPin, int clockPin): Initializes the shift register with the specified pins and sets all pins to output mode.
write(uint16_t num, bool state): Sets the state of a specific pin (identified by num) to either high or low.
setAll(): Sets all pins to high.
clearAll(): Sets all pins to low.
update(): Updates the shift register's state by shifting out the data in the buffer.
amount(): Returns the total number of pins available on the shift register.
*/
template <const uint16_t _AMNT>
class CustomHC595
{
public:
    CustomHC595() {}

    CustomHC595(int latchPin, int dataPin, int clockPin)
    {
        begin(latchPin, dataPin, clockPin);
    }

    void begin(int dataPin, int latchPin, int clockPin)
    {
        this->latchPin = latchPin;
        this->dataPin = dataPin;
        this->clockPin = clockPin;

        pinMode(latchPin, OUTPUT);
        pinMode(dataPin, OUTPUT);
        pinMode(clockPin, OUTPUT);
        clearAll();
    }

    void write(uint16_t num, bool state)
    {
        if (num < (_AMNT << 3))
        {
            if (state)
                bitSet(buffer[_AMNT - 1 - (num / 8)], num % 8);
            else
                bitClear(buffer[_AMNT - 1 - (num / 8)], num % 8);
            update();
        }
        else
        {
            Serial.print("Error: Pin number out of range: ");
            Serial.println(num);
        }
    }

    void setAll()
    {
        memset(buffer, 0xFF, _AMNT);
        update();
    }

    void clearAll()
    {
        memset(buffer, 0, _AMNT);
        update();
    }

    void update()
    {
        digitalWrite(latchPin, LOW);
        for (int i = 0; i < _AMNT; i++)
        {
            shiftOut(dataPin, clockPin, MSBFIRST, buffer[i]);
        }
        digitalWrite(latchPin, HIGH);
    }

    uint16_t amount()
    {
        return (_AMNT << 3);
    }

private:
    int latchPin;
    int dataPin;
    int clockPin;
    uint8_t buffer[_AMNT] = {0}; // Инициализация буфера
};

class CocktailMaker
{
public:
    void begin(int latchPin, int dataPin, int clockPin)
    {
        this->latchPin = latchPin;
        this->dataPin = dataPin;
        this->clockPin = clockPin;

        Serial.begin(9600); // Начало работы Serial
        hc595.begin(latchPin, dataPin, clockPin);
        iceBroker.attach(3, 555, 2400);
        iceBroker.write(0);
    }

/**
 * @brief Tests the pouring system by sequentially activating each pin.
 *
 * This function iterates over a set of 8 pins, turning each one on for 500 milliseconds,
 * then off for another 500 milliseconds. It prints the pin number being tested to the
 * Serial monitor for debugging purposes.
 */
    void testPouringSystem()
    {
        for (short i = 0; i < 8; i++)
        {
            Serial.print("Testing pin: ");
            Serial.println(i);
            hc595.write(i, true);
            delay(500);
            hc595.write(i, false);
            delay(500);
        }
    }

    /**
     * @brief Starts pouring a specific liquid/topping
     * @param name The name of the liquid/topping to pour
     * @param volume The volume of the liquid/topping to pour in ml
     * @return The time it will take to pour the liquid/topping in ms
     */
    short startPouring(const char *name, short volume)
    {
        short time = volume * 100;

        if (strcmp(name, "Coca-Cola") == 0)
        {
            hc595.write(5, true);
        }
        else if (strcmp(name, "Fanta") == 0)
        {
            hc595.write(6, true);
        }
        else if (strcmp(name, "Sprite") == 0)
        {
            hc595.write(7, true);
        }
        else if (strcmp(name, "Milkshake") == 0)
        {
            hc595.write(0, true);
        }
        else if (strcmp(name, "chocolate") == 0)
        {
            hc595.write(2, true);
            time *= 5;
        }
        else if (strcmp(name, "caramel") == 0)
        {
            hc595.write(4, true);
            time *= 5;
        }
        else if (strcmp(name, "fruit") == 0)
        {
            hc595.write(3, true);
            time *= 5;
        }
        else if (strcmp(name, "cream") == 0)
        {
            hc595.write(1, true);
            time *= 2;
        }
        else
        {
            Serial.println("Error: Invalid ingredient name");
            time = 0;
        }

        return time;
    }

    /**
     * Stops pouring all liquids/toppings immediately.
     * 
     * This function simply clears all outputs of the shift register, thus stopping
     * all pumps and solenoids that are currently running.
     */
    void stopPouring()
    {
        hc595.clearAll();
    }

    /**
     * Breaks an ice cube using the servo motor.
     * 
     * If there are any ice cubes left, this function will break one by setting
     * the servo motor to the "break ice" position, waiting 1.2 seconds, and then
     * setting it back to the "rest" position. The number of ice cubes left is
     * then decremented.
     */
    void breakIce()
    {
        if (iceCount-- >= 0)
        {
            // Управление мотором
            iceBroker.write(90);
            delay(1200);
            iceBroker.write(0);
            iceCount -= 1;
            iceUsed += 1;
        }
    }

private:
    CustomHC595<2> hc595;
    int latchPin, dataPin, clockPin;
    Servo iceBroker;
};

BarBotInterface interface(13, 12, 4, 5, 6, 7);
CocktailMaker cm;
/**
 * Initializes the serial interface, the LCD interface, and the cocktail maker.
 * This is called once at the beginning of the program.
 */
void setup()
{
    Serial.begin(9600);
    interface.begin();
    cm.begin(A1, A2, A3);
}

/**
 * Reads the force sensor value and maps it to a weight in grams.
 * 
 * The sensor value is read from the analog pin specified by the ForceSensor constant.
 * The value is then mapped to a weight in grams using the map() function. The mapping
 * is defined as follows: 0-904 sensor value maps to 0-1000 grams.
 * 
 * @return The weight in grams.
 */
int getWeight()
{
    int sensorData = analogRead(ForceSensor);
    return map(sensorData, 0, 904, 0, 1000);
}

/**
 * Pours a drink of the specified name and volume.
 * 
 * This function checks if a glass is present before starting the pouring process.
 * If a glass is detected, it calculates the time required to pour the specified volume
 * of the drink, displays the filling progress, and stops the pouring once completed.
 * 
 * @param name The name of the drink to be poured.
 * @param volume The volume of the drink to be poured in mililiters.
 */
void pourDrink(const char *name, short volume)
{
    if (!interface.checkGlass())
        return;
    short time = cm.startPouring(name, volume);
    interface.displayFillingProgress(name, time);
    cm.stopPouring();
}

/**
 * Asks user to choose drink from menu and stores it in global variable drink.
 * If user chooses "Back", stops the program and returns to main loop.
 */
void askDisplayDrink()
{
    drink[1] = interface.askVariant("Choose drink:", drinks, 4);
    if (drink[1] == -1)
    {
        isContinue = false;
        return;
    }
}
/**
 * Asks user about ice or cream for the milkshake
 *
 * @returns void
 */
void askIceOrcream()
{
    const char *options[] = {"Cream", "Ice"};
    drink[2] = interface.askVariant("Choose ice/cream:", options, 2);
}
void askIce()
{
    drink[3] = interface.askNumber("How much ice?", 0, iceCount);
}

/**
 * Asks user about toppings for the drink
 *
 * @returns void
 */
void askToppings()
{
    drink[4] = interface.askYesNo("Chocolate topping?") ? 1 : 0;
    drink[5] = interface.askYesNo("Caramel topping?") ? 1 : 0;
    char *chr2 = " ?";
    char *buffer = new char[strlen(fruits[fruitId]) + strlen(chr2) + 1 + 1];
    strcpy(buffer, fruits[fruitId]);
    strcat(buffer, " ");
    strcat(buffer, chr2);
    // use buffer
    drink[6] = interface.askYesNo(buffer) ? 1 : 0;
}

/**
 * Cooks the drink according to the given parameters in the "drink" array
 *
 * @returns void
 */
void cookDrink()
{
    // Максимальный объем стакана
    short glassVolume = drink[0];
    // Объем кубика льда
    short iceCubeVolume = 25;

    // Определение типа напитка
    switch (drink[1])
    {
    case 0: // milkshake
        Serial.println(F("milk"));
        break;
    case 1: // coca-cola
        Serial.println(F("cocacola"));
        break;
    case 2: // fanta
        Serial.println(F("fanta"));
        break;
    case 3: // sprite
        Serial.println(F("sprite"));
        break;
    default:
        break;
    }
    // Проверка, что стакан находится на месте
    if (!interface.checkGlass())
        return;
    short drinkVolume;
    if (drink[1] == 0)
    {
        // Пропорции компонентов
        float milkProportion = 0.4;
        float creamProportion = 0.4;
        float toppingProportion = 0.2;

        // Расчет максимальных объемов
        short maxMilkVolume = glassVolume * milkProportion;
        short maxcreamVolume = glassVolume * creamProportion;
        short maxToppingVolume = glassVolume * toppingProportion;

        // Объем льда и мороженого (если указано)
        short iceVolume = drink[3] * iceCubeVolume;
        short creamVolume = (drink[2] == 0) ? maxcreamVolume : 0;

        // Проверка, что лед не превышает допустимый объем
        if (iceVolume > maxcreamVolume)
        {
            interface.write("Error: too much ice");
            return;
        }
        // Корректировка молока в зависимости от льда и мороженого
        short actualMilkVolume = maxMilkVolume;
        if (iceVolume < maxcreamVolume)
        {
            drinkVolume += (maxcreamVolume - iceVolume);
        }

        pourDrink(drinks[0], drinkVolume);

        // Обработка топпингов для milkshake
        short toppingCount = drink[4] + drink[5] + drink[6];
        short toppingVolume = (toppingCount > 0) ? maxToppingVolume / toppingCount : 0;
        // Добавление топпингов
        if (drink[4] == 1)
        {
            Serial.println(F("choco"));
            pourDrink("chocolate", toppingVolume);
        }
        if (drink[5] == 1)
        {
            Serial.println(F("caramel"));
            pourDrink("caramel", toppingVolume);
        }
        if (drink[6] == 1)
        {
            Serial.println(fruits[fruitId]);
            pourDrink("fruit", toppingVolume);
        }

        // Расчет оставшегося объема для напитка
        drinkVolume = glassVolume - iceVolume - creamVolume - (toppingCount * (maxToppingVolume / 3));
        // Проверка на отрицательный объем
        if (drinkVolume < 0)
        {
            interface.write("Error: invalid drink volume");
            return;
        }
        // Добавляем мороженое
        if (drink[2] == 0)
        {
            Serial.println(F("cream"));
            pourDrink("cream", creamVolume);
        }
    }
    else
    {
        drinkVolume = glassVolume - drink[3] * iceCubeVolume;
        // Налить молоко или основной напиток
        pourDrink(drinks[drink[1]], drinkVolume);
    }
    // Добавляем лед
    for (short i = 0; i < drink[3]; i++)
    {
        cm.breakIce();
        delay(500);
    }
    cm.stopPouring();
    // Вывод информации о готовности
    lcd_old = 0;
    // Счетчик напитков в статистике
    drinksConsumed[drink[1]]++;
    Serial.println(drinksConsumed[drink[1]]);
    totalDrinkVolume += drinkVolume;
    // Сообщение о готовности и возвращение значений в ноль
    for (short i = 1; i < 7; i++)
    {
        drink[i] = 0;
    }
    while (interface.checkGlass())
    {
        if (lcd_old == 0)
        {
            interface.cls();
            interface.setXY(0, 0);
            interface.write("Your drink");
            interface.setXY(1, 6);
            interface.write("is ready!");
            interface.writeChar(1);
            interface.write(" ");
            interface.writeChar(5);
        }
        delay(1000);
    }
    interface.cls();
    interface.setXY(0, 0);
    interface.write("Thanks");
    interface.setXY(0, 1);
    interface.write("for order!");
    delay(1000);
    return;
}


/**
 * @brief A function that waits for a glass to be placed on the platform.
 * 
 * It shows a message "Put your glass" on the LCD and prints the current animation
 * name to the serial port.
 * 
 * @param none
 * @return none
 */
void waitingForGlass()
{
    if (lcd_old == 0)
    {
        interface.cls();
        interface.setXY(0, 0);
        interface.write("Put your glass");
        lcd_old = 1;
        Serial.println(animations[animationId]);
    }
}
/**
 * @brief The main loop of the program.
 * 
 * This is the main loop of the program, which is responsible for 
 * interacting with the user and making drinks. It checks if a glass
 * is present, asks the user which drink they want, asks for the
 * toppings, makes the drink and then displays a message thanking
 * the user.
 */

void loop()
{
    while (!interface.checkGlass())
    {
        waitingForGlass();
        delay(100);
    }
    drink[1] = interface.askDrink();
    if (isContinue)
    {
        if (drink[1] == 0)
        {
            askIceOrcream();
            if (drink[2] != 1)
            {
                askToppings();
            }
            else
            {
                askIce();
                askToppings();
            }
        }
        else
        {
            askIce();
        }
        cookDrink();
        lcd_old = 0;
    }
    else
    {
        interface.cls();
        interface.setXY(0, 0);
        interface.write("Thanks! ");
        delay(1000);
        isContinue = true;
        lcd_old = 0;
    }
}