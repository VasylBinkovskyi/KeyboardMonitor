#include <Arduino.h>
#include <KeyMonitor.h>

const long longPressLimit = 2000; //затримка для longPress
const int debouncLimit = 50;     //затримка антидребезга

/*
    Режими цифрової кнопки:
    1 - кнопка натиснута
    2 - кнопка була натиснута і відпущена
    3 - кнопка натиснута більше N секунд
    4 - кнопка була натиснута більше N секунд і відпущена
    5 - режим комбінацій аналогових кнопок


    Режими Аналогової кнопки ("A"):

    кнопка натиснута
    11 - перший режим, А кнопка №1
    12 - перший режим, А кнопка №2
    13 - перший режим, А кнопка №3

    кнопка була натиснута і відпущена
    21 - другий режим, А кнопка №1
    22 - другий режим, А кнопка №2
    23 - другий режим, А кнопка №3

    кнопка натиснута більше N секунд "longPressLimit"
    31 - третій режим, А кнопка №1
    32 - третій режим, А кнопка №2
    33 - третій режим, А кнопка №3

    кнопка була натиснута більше N секунд "longPressLimit" і відпущена
    41 - четвертий режим, А кнопка №1
    42 - четвертий режим, А кнопка №2
    43 - четвертий режим, А кнопка №3

    режим комбінацій аналогових кнопок
    51 - натиснута комбінація А кнопок 1+2
    52 - натиснута комбінація А кнопок 1+3
    53 - натиснута комбінація А кнопок 2+3

*/

int DigitalKeyMonitor(int pinNumber)
{

    static unsigned long debouncTimer = 0;
    static int isPressFlag = 0;

    bool bootonState = digitalRead(pinNumber);

    if (!bootonState)
    {
        debouncTimer = millis();
        if (isPressFlag == 1)
        {
            isPressFlag = 0;
            return 2;
        }
        if (isPressFlag == 2)
        {
            isPressFlag = 0;
            return 4;
        }
    }
    else if (millis() - debouncTimer > debouncLimit && millis() - debouncTimer < (debouncLimit + 10))
    {
        isPressFlag = 1;
    }
    else if (millis() - debouncTimer > longPressLimit)
    {
        isPressFlag = 2;
        return 3;
    }
    else if (isPressFlag == 1)
    {
        return 1;
    }
    return 0;
}

int AnalogKeyMonitor(int pinNumber)
{
    static unsigned long debouncTimer = 0;
    static int isPressFlag = 0;
    static int isButton = 0;

    //виставляєм відповідність рівнів напруги кнопкам і їх комбінаціям
    long maxRangeDegree = 650;  // 3.0; //максимальна напруга при відпущеный А кнопці
    long button1Degree = 58;    // 0.30;
    long button2Degree = 108;   // 0.55;
    long button3Degree = 221;   // 1.11; // рівні напруги які відповідають певним кнопкам і їх комбінаціям
    long ButtonHold_1and2 = 39; // 0.22;
    long ButtonHold_1and3 = 49; // 0.26;
    long ButtonHold_2and3 = 82; // 0.42;
    //ставимо толеранцію
    long tolerance = 2; // 0.025;

    long pinDegree = analogRead(pinNumber);

    // Serial.println(analogRead(pinNumber));

    if (pinDegree > maxRangeDegree - tolerance)
    {
        debouncTimer = millis(); // якщо кнопка була натиснута а потім відпущена повертаєм значення яке було присвоєно

        if (isPressFlag == 1)
        {
            isPressFlag = 0;
            isButton += 10;
            return isButton;
        }
        else if (isPressFlag == 2)
        {
            isPressFlag = 0;
            return isButton;
        }
    }
    else if (millis() - debouncTimer > longPressLimit) //кнопка нажата більше N секунд
    {
        isPressFlag = 2; // викидаєм флаг що кнопка була натиснута більше N секунд

        if (isButton != 0)
        {
            if (pinDegree > button1Degree - tolerance && pinDegree < button1Degree + tolerance)
            {
                isButton = 41;
                return 31;
            }
            else if (pinDegree > button2Degree - tolerance && pinDegree < button2Degree + tolerance)
            {
                isButton = 42;
                return 32;
            }
            else if (pinDegree > button3Degree - tolerance && pinDegree < button3Degree + tolerance)
            {
                isButton = 43;
                return 33;
            }
        }

        if (isButton < 40)
        {
            if (pinDegree > ButtonHold_1and2 - tolerance && pinDegree < ButtonHold_1and2 + tolerance)
            {
                isButton = 0;
                return 51;
            }
            else if (pinDegree > ButtonHold_1and3 - tolerance && pinDegree < ButtonHold_1and3 + tolerance)
            {
                isButton = 0;
                return 52;
            }
            else if (pinDegree > ButtonHold_2and3 - tolerance && pinDegree < ButtonHold_2and3 + tolerance)
            {
                isButton = 0;
                return 53;
            }
        }
    }
    else if (millis() - debouncTimer > debouncLimit && millis() - debouncTimer < (debouncLimit + 10)) //антидребезг
    {
        isPressFlag = 1; // викидаєм флаг що кнопка була натиснута

        // відповідно до рівня сигналу присвоюєм змінній певне значення яке буде повернуто коли кнопка буде відпущена
        if (pinDegree > button1Degree - tolerance && pinDegree < button1Degree + tolerance)
        {
            isButton = 11;
        }
        else if (pinDegree > button2Degree - tolerance && pinDegree < button2Degree + tolerance)
        {
            isButton = 12;
        }
        else if (pinDegree > button3Degree - tolerance && pinDegree < button3Degree + tolerance)
        {
            isButton = 13;
        }
        else
        {
            isButton = 0;
        }
    }
    else if (isPressFlag == 1)
    {
        return isButton;
    }
    return 0;
}
