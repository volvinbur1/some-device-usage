#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define RED 13
#define YELLOW 12
#define GREEN 11
#define TRIG 6
#define ECHO 5
#define BUZZER 2

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

void setup() {
  pinMode(6, OUTPUT);
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
}

void loop() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH);
  print_distance((int)calculate_distance(duration));
  delay(100);
}

void turnOFF_LEDs() {
  digitalWrite(RED, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(GREEN, LOW);
}

float calculate_distance(long duration) {
  float distance = 0.034 * duration / 2;

  int btone = map(distance, 2, 400, 1000, 4000);
  tone(3, btone);

  if (distance < 15) {
    turnOFF_LEDs();
    digitalWrite(RED, HIGH);
  }
  else if (distance < 35) {
      turnOFF_LEDs();
      digitalWrite(YELLOW, HIGH);
    }
    else {
      turnOFF_LEDs();
      digitalWrite(GREEN, HIGH);
    }
  
  return distance;
}

void print_distance(int distance) {
  if (distance < 500) {
    lcd.clear();
    lcd.print("Distance is:");
    lcd.setCursor(5, 1);
    //Serial.println(distance);
    lcd.print(distance);
    lcd.print(" sm");
    delay(50);
  }
}
