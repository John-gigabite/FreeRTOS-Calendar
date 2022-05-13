#include <Arduino_FreeRTOS.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void taskClockControl(void *param);
void taskNextSec(void *param);
void taskCheckTom(void *param);
void taskCheckYest(void *param);

TaskHandle_t Task_Handle1;
TaskHandle_t Task_Handle2;
TaskHandle_t Task_Handle3;
TaskHandle_t Task_Handle4;


int h = 12;
int m = 0;
int s = 0;
int flag = 1; //PM

// Time Set Buttons, 0 = pressed
int button1;
int button2;

// Pins definition for Time Set Buttons
int hs = 0; // pin 0 for Hours Setting
int ms = 1; // pin 1 for Minutes Setting

// Array for months and max days
int months[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int month = 0;
int day = 1;
int year = 2022;

int today[3];

// For accurate Time reading, use Arduino Real Time Clock and not just delay()
static uint32_t last_time, now = 0; // RTC
void updateClock (void) {
  if (h < 10) {
    lcd.print("0");
  } lcd.print(h); lcd.print(":"); if (m < 10)lcd.print("0"); lcd.print(m);
  lcd.print(":"); if (s < 10)lcd.print("0"); lcd.print(s); if (flag == 0)lcd.print("am"); if (flag == 1)lcd.print("pm");
}

void updateClock2 (void) {
  if (h < 10)Serial.print("0"); Serial.print(h); Serial.print(":"); if (m < 10)Serial.print("0"); Serial.print(m);
  Serial.print(":"); if (s < 10)Serial.print("0"); Serial.print(s); if (flag == 0)Serial.println("am"); if (flag == 1)Serial.println("pm");
}

void updateCalender(void) {
  if (months[month] < 10)lcd.print("0"); lcd.print(months[month]); lcd.print("/");
  if (day < 10)lcd.print("0"); lcd.print(day); lcd.print("/"); lcd.print(year);
}

void updateCalender2(void) {
  if (months[month] < 10)Serial.print("0"); Serial.print(months[month]); Serial.print("/");
  if (day < 10)Serial.print("0"); Serial.print(day); Serial.print("/"); Serial.print(year);
}

void clocksetup() {
  //setup lcd and buttons
  lcd.begin(16, 2);
  Wire.setClock(10000);
  lcd.clear();
  pinMode(hs, INPUT_PULLUP); // avoid external Pullup resistors for Button 1
  pinMode(ms, INPUT_PULLUP); // and Button 2
  now = millis(); // read RTC initial value

  //set clock
  lcd.setCursor(4, 0);
  lcd.print("Set Time");
  lcd.setCursor(3, 1);
  updateClock();
  bool clockSet1 = true; //hour loop
  bool clockSet2 = true; //minute loop
  bool clockSet3 = true; //am pm loop
  bool clockSet4 = true; //month loop
  bool clockSet5 = true; //day loop
  bool clockSet6 = true; //year loop

  //loop for setting hours
  while (clockSet1) {
    lcd.setCursor(3, 1);
    lcd.print("  ");
    for ( int i = 0 ; i < 5 ; i++) { // make 5 time 200ms loop, for faster Button response

      while ((now - last_time) < 200) //delay200ms
      {
        now = millis();
      }
      // inner 200ms loop
      last_time = now; // prepare for next loop
      button1 = digitalRead(hs); // Read Buttons
      button2 = digitalRead(ms);
      if (button1 == 0) {
        h = h + 1;
        if (h == 13) {
          h = 1;
        }
      }
      lcd.clear();
      lcd.setCursor(3, 1);
      updateClock();
      lcd.setCursor(4, 0);
      lcd.print("Set Time");
      if (button2 == 0) {
        clockSet1 = false;
      }
    }
  }
  //loop for setting minutes
  while (clockSet2) {
    lcd.setCursor(6, 1);
    lcd.print("  ");
    for ( int i = 0 ; i < 5 ; i++) { // make 5 time 200ms loop, for faster Button response
      while ((now - last_time) < 200) //delay200ms
      {
        now = millis();
      }
      // inner 200ms loop
      last_time = now; // prepare for next loop
      button1 = digitalRead(hs); // Read Buttons
      button2 = digitalRead(ms);
      if (button1 == 0) {
        m = m + 1;
        if (m == 60) {
          m = 0;
        }
      }
      lcd.clear();
      lcd.setCursor(3, 1);
      updateClock();
      lcd.setCursor(4, 0);
      lcd.print("Set Time");
      if (button2 == 0) {
        clockSet2 = false;
      }
    }
  }
  //loop for am and pm
  while (clockSet3) {
    lcd.setCursor(11, 1);
    lcd.print("  ");
    for ( int i = 0 ; i < 5 ; i++) { // make 5 time 200ms loop, for faster Button response
      while ((now - last_time) < 200) //delay200ms
      {
        now = millis();
      }
      // inner 200ms loop
      last_time = now; // prepare for next loop
      button1 = digitalRead(hs); // Read Buttons
      button2 = digitalRead(ms);
      if (button1 == 0) {
        flag = flag + 1;
        if (flag == 2) {
          flag =  0;
        }
      }
      lcd.clear();
      lcd.setCursor(3, 1);
      updateClock();
      lcd.setCursor(4, 0);
      lcd.print("Set Time");
      if (button2 == 0) {
        clockSet3 = false;
      }
    }
  }
  //Set up month
  while (clockSet4) {
    lcd.setCursor(3, 1);
    lcd.print("  ");
    for ( int i = 0 ; i < 5 ; i++) { // make 5 time 200ms loop, for faster Button response
      while ((now - last_time) < 200) //delay200ms
      {
        now = millis();
      }
      // inner 200ms loop
      last_time = now; // prepare for next loop
      button1 = digitalRead(hs); // Read Buttons
      button2 = digitalRead(ms);
      if (button1 == 0) {
        month = month + 1;
        if (month == 12) {
          month =  0;
        }
      }
      lcd.clear();
      lcd.setCursor(3, 1);
      updateCalender();
      lcd.setCursor(4, 0);
      lcd.print("Set Date");
      if (button2 == 0) {
        clockSet4 = false;
      }
    }
  }
  //set Day
  while (clockSet5) {
    lcd.setCursor(6, 1);
    lcd.print("  ");
    for ( int i = 0 ; i < 5 ; i++) { // make 5 time 200ms loop, for faster Button response
      while ((now - last_time) < 200) //delay200ms
      {
        now = millis();
      }
      // inner 200ms loop
      last_time = now; // prepare for next loop
      button1 = digitalRead(hs); // Read Buttons
      button2 = digitalRead(ms);
      if (button1 == 0) {
        day = day + 1;
        if (day > days[month]) {
          day =  1;
        }
      }
      lcd.clear();
      lcd.setCursor(3, 1);
      updateCalender();
      lcd.setCursor(4, 0);
      lcd.print("Set Date");
      if (button2 == 0) {
        clockSet5 = false;
      }
    }
  }
  lcd.clear();
  today[0] = day;
  today[1] = month;
  today[2] = year;
}

void setup() {
  Serial.begin(9600);
  Serial.println("version 1.53");
  pinMode(hs, INPUT_PULLUP); // avoid external Pullup resistors
  pinMode(ms, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  clocksetup();
  // setup threads
  xTaskCreate(taskClockControl, "Task1", 100, NULL, 1, &Task_Handle1);
  xTaskCreate(taskNextSec, "Task2", 100, NULL, 1, &Task_Handle2);
  xTaskCreate(taskCheckTom, "Task3", 100, NULL, 1, &Task_Handle3);
  xTaskCreate(taskCheckYest, "Task4", 100, NULL, 1, &Task_Handle4);
}

void loop() {}

//set up threads here
void taskClockControl(void *param) { //task 1
  (void) param;
  while (1) {
    day = today[0];
    month = today[1];
    year = today[2];
    updateCalender2();
    Serial.print("  ");
    updateClock2();
    s = s + 1;
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void taskNextSec(void *param) {// task 2
  (void) param;
  while (1) {
    Serial.println(" ");
    if (s == 60) {
      m = m + 1;
      s = 0;
      if (m == 60) {
        h = h + 1;
        m = 0;
        if ((h == 12) && (flag == 0)) { //handle am to pm
          flag = 1;
        }
        else if ((h == 12) && (flag == 1)) { //handle one day to the next
          day = day + 1;
          today[0] = day;
          flag = 0;
          if (day > days[month]) {
            day =  1;
            month = month + 1;
            today[1] = month;
            if (month == 12) {
              month = 1;
              year = year + 1;
              today[2] = year;
            }
          }
        }
        else if (h == 13) {
          h = 1;
        }
      }
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void taskCheckTom(void *param) { // task 3
  (void) param;
  while (1) {
    if (digitalRead(2) == 0) {
      day = day + 1;
      if (day > days[month]) {
        day =  1;
        month = month + 1;
        if (month == 12) {
          month = 1;
          year = year + 1;
        }
      }

      Serial.println("Tomorrow");
      updateCalender2();
      Serial.print("  ");
      updateClock2();
      Serial.println(" ");

    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void taskCheckYest(void *param) {// task 4
  (void) param;
  while (1) {
    if (digitalRead(0) == 0) {
      day = day - 1;
      if (day <= 0) {
        month = month - 1;
        if (month < 0) {
          month = 11;
          year = year - 1;
        }
        day = days[month];
      }

      Serial.println("Yesterday");
      updateCalender2();
      Serial.print("  ");
      updateClock2();
      Serial.println(" ");
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
