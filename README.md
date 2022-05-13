# FreeRTOS-Calendar
Digital Calendar using Arduino Uno running FreeRTOS. <br/>
Final Project for Adv Microprocessors.

## Overview

This project utilizes an microcontroller and a Real Time Operating System to create a simple digital calendar that is capable of:
1. Accurately telling the time
2. Switching to tomorrow's date briefly 
3. Switching to yesterday's date briefly
4. Changing to the next day once the time reaches 12 am

The following hardware components were used in this project:
1. Arduino Uno
2. 3 push buttons
3. 16x2 LCD screen with backpack module

The following software components were used in this project:
1. Arduino IDE
2. FreeRTOS (Available from Arduino)
3. New-LiquidCrystal-master.zip for LCD functions

## How it works
Upon startup, the project will display 12 am on the  LCD screen as the time and will give the user the ability to adjust the hour, then minutes, then am/pm to their time. After setting the time, the program will allow the user to set their desired date by switching between the month, day, then year. Once the date has been set, the program will switch to the Serial Monitor and begin running the seconds starting from 0, going all the way to 59, then changing the necessary hours or minutes and day, month or year.
