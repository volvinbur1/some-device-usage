#include <IRremote.h>
#include <EEPROM.h>
#define LED_PIN1 6
#define LED_PIN2 5
#define LED_PIN3 3


IRrecv ir(7);
decode_results result;

unsigned long key_code_set[17];

void setup() {
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(LED_PIN3, OUTPUT);
  
  Serial.begin(9600);
  ir.enableIRIn();
  ir.blink13(true);
  
  if (EEPROM.read(0) == true) {
    int address = 1;
    for (byte i = 0; i < 17; i++) {
      unsigned long value;
      EEPROM.get(address, value);
      key_code_set[i] = value;
      address += sizeof(value);
    }  
  }
  else {
    byte indexer = 0;
    int address = 1;
    while (indexer < 17) {
      if (ir.decode(&result)) {
        if (result.value != 0xFFFFFFFF) {
          EEPROM.put(address, result.value);
          key_code_set[indexer] = result.value;
          indexer++;
          address += sizeof(result.value);
        }
        ir.resume();
      }
    }
    EEPROM.write(0, true);
  }

  Serial.println("All codes are written");
  
  for (byte i = 0; i < 17; i++) {
    Serial.println(key_code_set[i], HEX);
  }
}

byte led_state = LOW;
byte led_pwm = 0;
byte selected_led = LED_PIN1;

void loop() {
  if (ir.decode(&result)) {
    Serial.println(result.value, HEX);

      if (result.value == key_code_set[1]) {
        if (led_pwm - 20 <= 0){
          led_pwm = 0;
        } else {
          led_pwm -= 20;
        }

        analogWrite(selected_led, led_pwm);
      }
        
      if (result.value == key_code_set[2]) {
        led_state = ~led_state;
        digitalWrite(selected_led, led_state);
      }
       
      if (result.value == key_code_set[3]) {
        if (led_pwm + 20 >= 255) {
          led_pwm = 255;
        }
        else {
          led_pwm += 20;
        }

        analogWrite(selected_led, led_pwm);
      }

      if (result.value == key_code_set[5]) {
        digitalWrite(selected_led, LOW);
        led_state = LOW;
        led_pwm = 0;
        
        selected_led = LED_PIN1;
      }
      
      if (result.value == key_code_set[6]) {
        digitalWrite(selected_led, LOW);
        led_state = LOW;
        led_pwm = 0;
        
        selected_led = LED_PIN2;
      }
      
      if (result.value == key_code_set[7]) {
        digitalWrite(selected_led, LOW);
        led_state = LOW;
        led_pwm = 0;
        
        selected_led = LED_PIN3;
      }
    
    ir.resume();  
  }
  
}
