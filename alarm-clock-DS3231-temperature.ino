#include <Wire.h>
#include <DS3231.h>
#include <LiquidCrystal_I2C.h>

// DEFINE CLASSES
DS3231 myRTC;
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// DEFINE VARIABLES
bool mode12 = true; // true = 12-hour mode
byte theSecond = 0; // to set the second
byte theMinute = 41; // to set the minute
byte theHour = 14; // to set the hour (use 24 hour clock)
byte theDoW = 4; // to set the day of week (1 = Sunday)
byte theDate = 21; // to set the date
byte theMonth = 12; // to set the month
byte theYear = 22; // two digit
bool hPM; // boolean used for DS3231::getHour() function
bool CenturyBit; // boolean used to roll over 99 to 00
byte A1Day = 1; // to set the day/date for alarm 1 (1-7 day of week, 1-31 date)
byte A1Hour = 7; // to set the hour for alarm 1 (1-12 for 12-hour mode, 0-23 for 24-hour mode)
byte A1Minute = 30; // to set the minute alarm 1 
byte A1Second = 0; // to set the second for alarm 1
byte Alarm1Bits = 0b00001000; // https://github.com/NorthernWidget/DS3231/blob/master/Documentation/Alarms.md#alarm-bits-quick-reference
bool A1Dy = true; // to set day of week mode or date mode for alarm 1 (true DoW, false date)
bool A1h12 = true; // to set clock mode for alarm 1 (true 12-hour, false 24-hour)
bool A1PM = false; // to set AM/PM for alarm 1 when in 12-hour mode (true PM, false AM)
byte A2Day = 1; // to set the day/date for alarm 2 (1-7 day of week, 1-31 date)
byte A2Hour = 3; // to set the hour for alarm 2 (1-12 for 12-hour mode, 0-23 for 24-hour mode)
byte A2Minute = 0; // to set the minute alarm 2
byte Alarm2Bits = 0b01000000; // https://github.com/NorthernWidget/DS3231/blob/master/Documentation/Alarms.md#alarm-bits-quick-reference
bool A2Dy = true; // to set day of week mode or date mode for alarm 2 (true DoW, false date)
bool A2h12 = true; // to set clock mode for alarm 21 (true 12-hour, false 24-hour)
bool A2PM = true; // to set AM/PM for alarm 1 when in 12-hour mode (true PM, false AM)
bool light = false; // variable to keep track of push button and alarm status (true LCD backlight on, false LCD baclight off)
const int button = 2; // input pin 2 on Arduino for push button
int buttonState = 0; // to monitor whether the button is pressed
int count = 0; // timer to turn off backlight

void setup() {
  // BEGIN I2C COMMUNICATION
  Wire.begin();

  // INITIALIZE LCD
  lcd.init();
  lcd.clear();
  lcd.setCursor(0,0);

  // SET TIME
  /*
  myRTC.setClockMode(mode12);
  myRTC.setSecond(theSecond);
  myRTC.setMinute(theMinute);
  myRTC.setHour(theHour);
  myRTC.setDoW(theDoW);
  myRTC.setDate(theDate);
  myRTC.setMonth(theMonth);
  myRTC.setYear(theYear);

  // SET ALARMS
  myRTC.setA1Time(A1Day, A1Hour, A1Minute, A1Second, Alarm1Bits, A1Dy,
                  A1h12, A1PM);

  myRTC.setA2Time(A2Day, A2Hour, A2Minute, Alarm2Bits, A2Dy, A2h12,
                  A2PM);
  */

  // TURN ON ALARMS
  myRTC.turnOnAlarm(1);
  myRTC.turnOnAlarm(2);

  // INITIALIZE BUTTON
  pinMode(button, INPUT);
}

void loop() {
  // GET TIME
  byte curHour = myRTC.getHour(mode12, hPM);
  byte curMinute = myRTC.getMinute();
  byte curSecond = myRTC.getSecond();

  // GET TEMP
  float curTemp = myRTC.getTemperature();
  float ftemp = round((curTemp * 9 / 5 + 32));

  ///* GET BUTTON STATE
  buttonState = digitalRead(button);
  if (buttonState == LOW) {
    light = !light;
  }

  // PRINT TIME
  lcd.setCursor(0,0);
  if (curHour < 10) {
    lcd.print(" ");
  }
  lcd.print(curHour);
  lcd.print(":");
  if (curMinute < 10) {
    lcd.print("0");
  }
  lcd.print(curMinute);
  lcd.print(":");
  if (curSecond < 10) {
    lcd.print("0");
  }
  lcd.print(curSecond);
  if (mode12) {
    if (hPM) {
      lcd.print(" PM");
    } else {
      lcd.print(" AM");
    }
  }
  lcd.print(" ");

  // PRINT TEMP
  lcd.print(ftemp, 0);
  lcd.print(char(223));
  lcd.print("F");

  // PRINT ALARM TIMES
  lcd.setCursor(0,1);
  if (A1Hour < 10) {
    lcd.print(" ");
  }
  lcd.print(A1Hour);
  lcd.print(":");
  if (A1Minute < 10) {
    lcd.print("0");
  }
  lcd.print(A1Minute);
  if (A1h12) {
    if (A1PM) {
      lcd.print("PM ");
    } else {
      lcd.print("AM ");
    }
  }
  if (A2Hour < 10) {
    lcd.print(" ");
  }
  lcd.print(A2Hour);
  lcd.print(":");
  if (A2Minute < 10) {
    lcd.print("0");
  }
  lcd.print(A2Minute);
  if (A2h12) {
    if (A2PM) {
      lcd.print("PM");
    } else {
      lcd.print("AM");
    }
  }

  // CHECK ALARMS AND TURN ON LIGHT VARIABLE
  if ((myRTC.checkIfAlarm(1)) || (myRTC.checkIfAlarm(2))) {
    light = true;
  }

  // TURN OFF LIGHT AFTER 10 MINUTES
  if (light) {
    count = count + 1;
    if (count > 599) {
      count = 0;
      light = false;
    }
  } else {
    count = 0;
  }
 
  // TOGGLE BACKLIGHT
  if (light) {
    lcd.backlight();
  } else {
    lcd.noBacklight();
  }
  
  delay(1000);
}
