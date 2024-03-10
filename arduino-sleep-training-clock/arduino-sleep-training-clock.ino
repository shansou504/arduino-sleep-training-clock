#include <Wire.h>
#include <DS3231.h>
#include <LiquidCrystal_I2C.h>

/// *** INSTRUCTIONS *** ///

// To set the time and alarms adjust the variable values in the
// DEFINE VARIABLES section. Then uncomment the SET TIME and SET ALARMS
// sections under void setup(). Rely heavily on the DS3231 library
// documentation for a clearer understanding on what each variable
// means. The comments below are simply for quick reference. After
// setting the times and alarm variables and uncommenting the SET TIME
// and SET ALARMS sections, upload the code to the Arduino. Once the LCD
// screen displays the correct time and alarms as desired be sure to
// comment out the SET TIME and SET ALARMS sections and re-upload the
// code again so the time is not reset during each power cycle.
//
// DS3231 Library
// https://github.com/NorthernWidget/DS3231
//
// LiquidCrystal_I2C Library
// https://github.com/johnrickman/LiquidCrystal_I2C

/// *** DEFINE HARDWARE *** ///

DS3231 myRTC;
// set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27,20,4);

/// *** DEFINE VARIABLES *** ///

// mode12 (bool)
// true = 12-hour mode
bool mode12 = true;
byte theSecond = 0;
byte theMinute = 34;
// theHour (byte) (use 24 hour clock)
byte theHour = 15;
// theDoW (byte) Day of Week (1 = Sunday)
byte theDoW = 1;
byte theDate = 10; 
byte theMonth = 3;
// theYear (byte) (use 2 digits)
byte theYear = 24;
// hPM (bool) Used for DS3231::getHour() function
bool hPM;
// CenturyBit (bool) Used to roll over 99 to 00
bool CenturyBit;
// A1Day (byte) Sets the day/date for alarm 1 (1-7 day of week,
// 1-31 date)
byte A1Day = 1;
// A1Hour (byte) Sets the hour for alarm 1 (1-12 for 12-hour mode,
// 0-23 for 24-hour mode)
byte A1Hour = 7;
// A1Minute (byte) Sets the minute for alarm 1
byte A1Minute = 15;
// A1Second (byte) Sets the second for alarm 1
byte A1Second = 0;
// Alarm1Bits (byte) reference the DS3231 documentation
// https://github.com/NorthernWidget/DS3231/blob/master/Documentation/
// Alarms.md#alarm-bits-quick-reference
byte Alarm1Bits = 0b00001000;
// A1Dy (bool) Sets the day of week mode or date mode for alarm 1
// (true = DoW, false = date)
bool A1Dy = true;
// A1h12 (bool) Sets the clock mode for alarm 1 (true = 12-hour,
// false = 24-hour)
bool A1h12 = true;
// A1PM (bool) Sets AM/PM for alarm 1 when in 12-hour mode (true = PM,
// false = AM)
bool A1PM = false;
// A2Day (byte) Sets the day/date for alarm 2 (1-7 day of week,
// 1-31 date)
byte A2Day = 1;
// A2Hour (byte) Sets the hour for alarm 2 (1-12 for 12-hour mode,
// 0-23 for 24-hour mode)
byte A2Hour = 3;
// A2Minute (byte) Sets the minute alarm 2
byte A2Minute = 0;
// Alarm2Bits (byte) reference the DS3231 documentation
// https://github.com/NorthernWidget/DS3231/blob/master/Documentation/
// Alarms.md#alarm-bits-quick-reference
byte Alarm2Bits = 0b01000000;
// A2Dy (bool) Sets the day of week mode or date mode for alarm 2
// (true = DoW, false = date)
bool A2Dy = true;
// A2h12(bool) Sets the clock mode for alarm 2 (true = 12-hour,
// false = 24-hour)
bool A2h12 = true;
// A2PM (bool) Sets AM/PM for alarm 2 when in 12-hour mode (true = PM,
// false = AM)
bool A2PM = true;
// light (bool) variable to keep track of push button and alarm status
// (true = LCD backlight on, false = LCD baclight off)
bool light = false;
// button (const int) Input pin 2 on Arduino for push button
const int button = 2;
// buttonState (int) Monitors whether the button is pressed
int buttonState = 0;
// count (int) Timer to turn off backlight
int count = 0;


/// *** VOID SETUP *** ///

void setup() {
  // Begin I2C communication
  Wire.begin();

  // Initialize LCD
  lcd.init();
  lcd.clear();
  lcd.setCursor(0,0);

  /// *** SET TIME *** ///
  
  /*
  myRTC.setClockMode(mode12);
  myRTC.setSecond(theSecond);
  myRTC.setMinute(theMinute);
  myRTC.setHour(theHour);
  myRTC.setDoW(theDoW);
  myRTC.setDate(theDate);
  myRTC.setMonth(theMonth);
  myRTC.setYear(theYear);
  //*/

  /// *** SET ALARMS *** ///
  
  /* 
  myRTC.setA1Time(A1Day, A1Hour, A1Minute, A1Second, Alarm1Bits, A1Dy,
                  A1h12, A1PM);

  myRTC.setA2Time(A2Day, A2Hour, A2Minute, Alarm2Bits, A2Dy, A2h12,
                  A2PM);
  //*/

  /// *** TURN ON/OFF ALARMS *** ///
  
  myRTC.turnOnAlarm(1);
  myRTC.turnOffAlarm(2);

  /// *** INITIALIZE BUTTON *** ///
  
  pinMode(button, INPUT);
}

/// *** VOID LOOP *** ///

void loop() {
  
  /// *** GET TIME *** ///
  
  byte curHour = myRTC.getHour(mode12, hPM);
  byte curMinute = myRTC.getMinute();
  byte curSecond = myRTC.getSecond();

  /// *** GET TEMP *** ///
  
  float curTemp = myRTC.getTemperature();
  float ftemp = round((curTemp * 9 / 5 + 32));

  /// *** GET BUTTON STATE *** ///
  
  buttonState = digitalRead(button);
  if (buttonState == LOW) {
    light = !light;
  }

  /// *** PRINT TIME *** ///
  
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

  /// *** PRINT TEMP *** ///
  
  lcd.print(ftemp, 0);
  lcd.print(char(223));
  lcd.print("F");

  /// *** PRINT ALARM TIMES *** ///
  
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

  /// *** CHECK BOTH ALARMS AND TURN ON LIGHT VARIABLE *** ///

  /*
  if ((myRTC.checkIfAlarm(1)) || (myRTC.checkIfAlarm(2))) {
    light = true;
  }
  //*/

  /// *** CHECK ONLY ALARM 1 AND TURN ON LIGHT VARIABLE *** ///
  
  /*
  if (myRTC.checkIfAlarm(1)) {
    light = true;
  }
  //*/

  /// *** CHECK ONLY ALARM 1 AND TURN ON LIGHT VARIABLE - DISABLE ON WEEKENDS *** ///
  
  ///*
  if (myRTC.checkIfAlarm(1) && myRTC.getDoW() > 1 && myRTC.getDoW() < 6) {
    light = true;
  }
  //*/

  /// *** TURN OFF LIGHT AFTER 15 MINUTES *** ///
  
  if (light) {
    count = count + 1;
    if (count > 899) {
      count = 0;
      light = false;
    }
  } else {
    count = 0;
  }
 
  /// *** TOGGLE BACKLIGHT *** ///
  
  if (light) {
    lcd.backlight();
  } else {
    lcd.noBacklight();
  }
  
  /// *** DELAY *** ///
  
  delay(1000);
}
