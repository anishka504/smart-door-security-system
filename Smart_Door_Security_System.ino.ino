#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

#define MQ135 A0
#define BUZZER 11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {

  dht.begin();

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, HIGH); // Active LOW buzzer OFF

  lcd.init();
  lcd.backlight();

  lcd.print("Industrial");
  lcd.setCursor(0,1);
  lcd.print("Monitoring");
  delay(2000);
}

void loop() {

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  int air = analogRead(MQ135);

  // Screen 1
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("T:");
  lcd.print(temp,1);
  lcd.print((char)223);
  lcd.print("C");

  lcd.setCursor(0,1);
  lcd.print("H:");
  lcd.print(hum,1);
  lcd.print("%");

  delay(3000);

  // Screen 2
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Air:");
  lcd.print(air);

  lcd.setCursor(0,1);

  if(air <= 60) {

    lcd.print("SAFE");

  } else if(air <= 100) {

    lcd.print("CAUTION");

  } else {

    lcd.print("DANGER");

    digitalWrite(BUZZER, LOW);
    delay(1000);
    digitalWrite(BUZZER, HIGH);
  }

  delay(3000);
}