#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo doorServo;

#define BUZZER 11
#define PIR 12
#define SERVO_PIN 10

const byte ROWS = 4;
const byte COLS = 4;

// Corrected for your keypad wiring
char keys[ROWS][COLS] = {
  {'D','C','B','A'},
  {'#','9','6','3'},
  {'0','8','5','2'},
  {'7','4','1','*'}
};

byte rowPins[ROWS] = {2,3,4,5};
byte colPins[COLS] = {6,7,8,9};

Keypad keypad = Keypad(
  makeKeymap(keys),
  rowPins,
  colPins,
  ROWS,
  COLS
);

String password = "1234";
String entered = "";

void setup() {

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, HIGH);   // Active-LOW buzzer OFF

  pinMode(PIR, INPUT);

  doorServo.attach(SERVO_PIN);
  doorServo.write(0);           // Locked

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Smart Door Lock");

  delay(2000);

  lcd.clear();
  lcd.print("Enter Password");
}

void loop() {

  // Motion Detection
  if (digitalRead(PIR) == HIGH) {

    lcd.clear();
    lcd.print("Motion Alert!");

    digitalWrite(BUZZER, LOW);
    delay(500);
    digitalWrite(BUZZER, HIGH);

    delay(1000);

    lcd.clear();
    lcd.print("Enter Password");
  }

  char key = keypad.getKey();

  if (key) {

    if (key == '#') {

      lcd.clear();

      if (entered == password) {

        lcd.print("Access Granted");

        doorServo.write(90);    // Unlock

        delay(5000);

        doorServo.write(0);     // Lock

      } else {

        lcd.print("Access Denied");

        digitalWrite(BUZZER, LOW);
        delay(2000);
        digitalWrite(BUZZER, HIGH);
      }

      entered = "";

      delay(2000);

      lcd.clear();
      lcd.print("Enter Password");
    }

    else if (key == '*') {

      entered = "";

      lcd.clear();
      lcd.print("Cleared");

      delay(1000);

      lcd.clear();
      lcd.print("Enter Password");
    }

    else {

      entered += key;

      lcd.setCursor(0,1);

      for (int i = 0; i < entered.length(); i++) {
        lcd.print("*");
      }
    }
  }
}