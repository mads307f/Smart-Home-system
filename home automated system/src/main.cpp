#include <SPI.h>
#include <iostream>
#include <startup.h>
#include <stdio.h>
#include <time.h>
#include <funktions.h>

void setup(){
  startup();
}

void loop() {
  mesh.update();
  /*------------------ Clock ------------------*/
  if (state == 1) {
    display.clearDisplay(), display.setTextSize(3), display.setCursor(20,22), display.println(rtc.getTime("%H:%M"));

    if (lastButtonStateB == 1 && digitalRead(BUTTON_B) == 0){
      lastButtonStateB = digitalRead(BUTTON_B);
      set_clock();
    }
    
    else if (lastButtonStateA == 1 && digitalRead(BUTTON_A) == 0){
      state_minus();
    }

    else if (lastButtonStateC == 1 && digitalRead(BUTTON_C) == 0){
      state_plus();
    }

    delay(10);
    yield();
    display.display();
  }

  /*------------------ Temp ------------------*/
  else if (state == 2) {
    // Reading temperature or humidity takes about 250 milliseconds! (its a very slow sensor)
    msg_hum = dht.readHumidity();
    msg_tempe = dht.readTemperature();

    // Check if any reads failed and exit early (to try again)
    if (isnan(msg_hum) || isnan(msg_tempe)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

    display.clearDisplay(), display.setTextSize(3), display.setCursor(30,5), display.print(msg_tempe), display.print(" C");
    display.setTextSize(3), display.setCursor(30,37), display.print(msg_hum), display.print(" %");

    if (lastButtonStateB == 1 && digitalRead(BUTTON_B) == 0){
      lastButtonStateB = digitalRead(BUTTON_B);
      // edit temp and or hum
    }
    
    else if (lastButtonStateA == 1 && digitalRead(BUTTON_A) == 0){
      state_minus();
    }

    else if (lastButtonStateC == 1 && digitalRead(BUTTON_C) == 0){
      state_plus();
    }

    delay(10);
    yield();
    display.display();
  }

  /*------------------ Light ------------------*/
  else if (state == 3) {
    display.clearDisplay(), display.setTextSize(2), display.setCursor(30,0), display.println("Lys");
    display.setTextSize(2), display.setCursor(0,20), display.println(msg_LDR);
    //display.setTextSize(2), display.setCursor(0,40), display.println("ae");
    //display.setTextSize(2), display.setCursor(20,40), display.println("o/");
    //display.setTextSize(2), display.setCursor(40,40), display.println("a");

    if (lastButtonStateB == 1 && digitalRead(BUTTON_B) == 0){
      lastButtonStateB = digitalRead(BUTTON_B);
      light_setting();
    }
    
    else if (lastButtonStateA == 1 && digitalRead(BUTTON_A) == 0){
      state_minus();
    }

    else if (lastButtonStateC == 1 && digitalRead(BUTTON_C) == 0){
      state_plus();
    }

    delay(10);
    yield();
    display.display();
  }

  lastButtonStateA = digitalRead(BUTTON_A);
  lastButtonStateB = digitalRead(BUTTON_B);
  lastButtonStateC = digitalRead(BUTTON_C);
  if (rtc.getHour(true) < 8 || rtc.getHour(true) > 19) {
    msg_pump = 0;
  }
  else {
    msg_pump = 1;
  }
  delay(90);
}