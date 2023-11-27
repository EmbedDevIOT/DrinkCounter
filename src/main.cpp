/*
 * данная программа отображает цифры на 3-х семисегментных индикаторах
 * Hardware Connections (Breakoutboard to Arduino Nano):
 * Vin  - 5V (3.3V is allowed)
 * GND - GNDs
 * 74HC595 ST_CP - 4 (ESP32)
 * 74HC595 SH_CP - 12 (ESP32)
 * 74HC595 DS - 14 (ESP32)
 *
 */
#include "Arduino.h"
// устанавливаем тип семисегментного индикатора (с общим катодом или общим анодом)
bool flag = 1;
const bool commonCathode = false; // необходимо использовать значение true для семисегментного индикатора с общим катодом и значение false для семисегментного индикатора с общим анодом
// int dispDigit1 = 0;
// int dispDigit2 = 0;
// int dispDigit3 = 0;
int digital[3];
// шаблоны цифр для семисегментного индикатора
const byte digit_pattern[17] =
    {
        // 74HC595 Outpin Connection with 7segment display.
        // Q0 Q1 Q2 Q3 Q4 Q5 Q6 Q7
        // a  b  c  d  e  f  g  DP
        0b11111100, // 0
        0b01100000, // 1
        0b11011010, // 2
        0b11110010, // 3
        0b01100110, // 4
        0b10110110, // 5
        0b10111110, // 6
        0b11100000, // 7
        0b11111110, // 8
        0b11110110, // 9
        0b11101110, // A
        0b00111110, // b
        0b00011010, // C
        0b01111010, // d
        0b10011110, // E
        0b10001110, // F
        0b00000001  // .
};
// Pin connected to ST_CP of 74HC595
int latchPin = 4;
// Pin connected to SH_CP of 74HC595
int clkPin = 12;
// Pin connected to DS of 74HC595
int dtPin = 14;
// display value
int dispVal = 0;

bool increment = true;

int factor_digits(int num, int *digits, int limit)
{
  int digits_count = 0;

  // Отдельно обрабатываем ситуацию, когда число изначально равно нулю
  if (num == 0)
  {
    digits[0] = 0;
    return 1;
  }

  // Последовательно извлекаем цифры числа, пока они есть
  while (num > 0 && limit)
  {
    // Копируем очередную цифру в массив
    digits[digits_count++] = num % 10;
    // Переходим к следующей цифре
    num /= 10;
    // Уменьшаем допустимое количество оставшихся цифр
    limit--;
  }
  return num ? 0 : digits_count;
}

void Show(int drink_number)
{
  // https://ru.stackoverflow.com/questions/89649/%D0%9A%D0%B0%D0%BA-%D1%80%D0%B0%D0%B7%D0%B4%D0%B5%D0%BB%D0%B8%D1%82%D1%8C-%D1%87%D0%B8%D1%81%D0%BB%D0%BE-%D0%BD%D0%B0-%D1%86%D0%B8%D1%84%D1%80%D1%8B

  int factored_number[3];  // Массив для хранения результата
  int factored_number_len; // Количество цифр в числе

  factored_number_len = factor_digits(drink_number, factored_number, 3);

  for (int i = factored_number_len - 1; i >= 0; i--)
  {
    digital[i] = factored_number[i];
    // Serial.println(digital[i]);
  }
  // Serial.println(digital[i]); 

  // dispDigit1 = drink_number / 100;
  // dispDigit2 = drink_number % 10;
  // dispDigit3 = drink_number % 1;
  // dispDigit3 = 3;

  // printf("%d%d%d\n", digital[0], digital[1], digital[2]);
  digitalWrite(latchPin, LOW);
  shiftOut(dtPin, clkPin, LSBFIRST, ~(digit_pattern[digital[0]])); // 1. (Digit+DP)
  shiftOut(dtPin, clkPin, LSBFIRST, ~(digit_pattern[digital[1]])); // 1. (Digit+DP)
  shiftOut(dtPin, clkPin, LSBFIRST, ~(digit_pattern[digital[2]]));
  digitalWrite(latchPin, HIGH);
  flag = 0;
}
void setup()
{
  // put your setup code here, to run once:
  // set the serial port at 115200
  Serial.begin(115200);
  delay(1000);
  // зададим режим работы контактов, к которым подключен регистр сдвига 74HC595, на вывод данных
  pinMode(latchPin, OUTPUT); // ST_CP of 74HC595
  pinMode(clkPin, OUTPUT);   // SH_CP of 74HC595
  pinMode(dtPin, OUTPUT);    // DS of 74HC595
}
void loop()
{
  if (flag)
    Show(148);

  // put your main code here, to run repeatedly:

  // if(increment==true){

  //   dispVal += 1;
  //   if (dispVal == 8){
  //     increment=false;
  //   }
  // }else{
  //   printf("%d.\n", dispDigit1);
  //   digitalWrite(latchPin, LOW);
  //   // if(commonCathode == true){
  //   //   shiftOut(dtPin, clkPin, LSBFIRST, digit_pattern[dispDigit2]|digit_pattern[16]);   // 1. (Digit+DP)
  //   //   shiftOut(dtPin, clkPin, LSBFIRST, digit_pattern[dispDigit1]);
  //   // }else{
  //     shiftOut(dtPin, clkPin, LSBFIRST, ~(digit_pattern[dispDigit2]));   // 1. (Digit+DP)
  //     shiftOut(dtPin, clkPin, LSBFIRST, ~(digit_pattern[dispDigit1]));
  //   // }
  //   digitalWrite(latchPin, HIGH);
  //   dispVal -= 1;
  //   if (dispVal == 0){
  //     increment=true;
  //   }
  // }
  // delay(1000);
}