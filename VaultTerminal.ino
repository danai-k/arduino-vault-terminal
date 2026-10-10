#include <Wire.h>
#include "WaveshareLCD.h"
#include <LiquidCrystal_I2C.h>

#define PIN_BUTTON 2
#define CLEAR_BUTTON 3
#define GREEN_LED 6
#define RED_LED 5
#define BUZZER 7

void victorySound();

LiquidCrystal_I2C Biglcd(0x27, 20, 4);

int prevDial = -1;
int prevReading = -1;
int dialValue = 0;
int currStep = 0;
int slots = 3;
int Code[3];
int secretCode[3] = {67, 33, 25}; // Secret Combination - Can be changed !
int triesLeft = 5;


void setup() {
  // Big screen
  Biglcd.init();
  Biglcd.backlight();

  Biglcd.setCursor(0, 0);
  Biglcd.print("*** VAULT SECURE ***");
  Biglcd.setCursor(0,1);
  Biglcd.print("CODE: -- -- --");
  Biglcd.setCursor(0, 2);
  Biglcd.print("DIAL: [ ");
  
  // Small screen
  lcd_init();
  lcd_set_cursor(0, 0);
  lcd_print("TRIES LEFT: 5         ");
  lcd_set_cursor(0, 1);
  lcd_print("HINT: - - -           ");

  // Pins
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(CLEAR_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
}


void loop() {
  // 1. POTENTIOMETER 
  int reading = analogRead(A0);

  // short-term memory to prevent flickering
  if (abs(reading - prevReading) > 5)
  {
    prevReading = reading;
    // convert value to desired limit
    dialValue = map(reading, 0, 1023, 0, 99);

    if ( dialValue != prevDial )
    {
    Biglcd.setCursor(8, 2);
    Biglcd.print(dialValue);
    Biglcd.print(" ]        ");
    prevDial = dialValue;
    }
  }
  delay(50);

  // 2. Buttons
  // PIN_BUTTON
  int buttonState = digitalRead(PIN_BUTTON);
  if (buttonState == LOW && currStep < 3) // button is pressed
  {
    Code[currStep] = dialValue;
    if (currStep == 0)
    {
      Biglcd.setCursor(6, 1);
      Biglcd.print(dialValue);
    }
    else if ( currStep == 1)
    {
      Biglcd.setCursor(9, 1);
      Biglcd.print(dialValue);
    }
    else if ( currStep == 2)
    {
      Biglcd.setCursor(12, 1);
      Biglcd.print(dialValue);
    }
    currStep++;
    delay(300);

    if (currStep == 3)
    {
      lcd_set_cursor(6, 1);
      for (int i = 0; i < slots; i++)
      {
        if ( Code[i] == secretCode[i]) { lcd_print("= "); } // correct !
        else if ( Code[i] < secretCode[i] ) { lcd_print("^ "); } //  number needs to be higher
        else { lcd_print("v "); } // number needs to be lower
      }

      Biglcd.setCursor(0, 3);
      if (secretCode[0] == Code[0] && secretCode[1] == Code[1] && secretCode[2] == Code[2])
      {
        Biglcd.print(">> ACCESS GRANTED <<");
        victorySound();
      }
      else 
      { 
        digitalWrite(RED_LED, HIGH);
        tone(BUZZER, 1000, 1000);
        Biglcd.print (">> ACCESS DENIED <<"); 

        triesLeft--; 
        lcd_set_cursor(12, 0); 
        lcd_print(triesLeft);
      }
      currStep = 4;
    }
  }

  // CLEAR_BUTTON
  int clearButtonState = digitalRead(CLEAR_BUTTON);
  if (clearButtonState == LOW)
  {
    if (triesLeft > 0)
    {
      currStep = 0;
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, LOW);
      Biglcd.setCursor(6, 1);
      Biglcd.print("-- -- --");

      Biglcd.setCursor(0, 3);
      Biglcd.print("                    "); // 20 black spaces to erase row 

      lcd_set_cursor(6, 1);
      lcd_print("- - -       ");    
    }
    else
    {
      digitalWrite(RED_LED, HIGH);
      tone(BUZZER, 200, 500);

      Biglcd.setCursor(0, 3);
      Biglcd.print(">>> SYSTEM LOCKED! <<<");
      lcd_set_cursor(0, 0);
      lcd_print("SECURITY LOCKOUT");
      lcd_set_cursor(0, 1);
      lcd_print("PLEASE REBOOT   ");
    }
    delay(300);
  }
}

void victorySound(){
  analogWrite(GREEN_LED, 50);
  tone(BUZZER, 523, 100);
  delay(120);

  analogWrite(GREEN_LED, 150);
  tone(BUZZER, 659, 100);
  delay(120);

  analogWrite(GREEN_LED, 255);
  tone(BUZZER, 784, 100);
  delay(150);

  tone(BUZZER, 1047);

  for (int i = 255; i >= 50; i -= 2) 
  {
    analogWrite(GREEN_LED, i);
    delay(4);
  }
  for (int i = 50; i <= 255; i += 2) 
  {
    analogWrite(GREEN_LED, i);
    delay(4);
  }
        
  noTone(BUZZER);              
  analogWrite(GREEN_LED, 255); 
}
