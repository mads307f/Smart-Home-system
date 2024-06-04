#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <ESP32Time.h>
#include <define_ESP32.h>
#include <mesh.h>
#include <DHT.h>

#define DHTPIN 25
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &WIRE);
ESP32Time rtc;
//ESP32Time rtc(3600);  // offset in seconds GMT+1

int lastButtonStateA = 1,lastButtonStateB = 1, lastButtonStateC = 1;
int buttonState = 1;

int minute = 0, hour = 0;

void set_clock() {
  display.clearDisplay();
  while (true){
    rtc.setTime(0, minute, hour, 17, 1, 2024);
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0), display.print("indstil ur");
    display.setTextSize(3);
    display.setCursor(20,32), display.print(rtc.getTime("%H:%M"));
    if(!digitalRead(BUTTON_A)) minute = minute + 1;
    if (minute == 60){
      minute = 0;
    }
    if(!digitalRead(BUTTON_C)) hour = hour + 1;
    if (hour == 60){
      hour = 0;
    }

    if (lastButtonStateB == 1 && digitalRead(BUTTON_B) == 0){
      break;
    }
    
    lastButtonStateB = digitalRead(BUTTON_B);
    display.display();
    delay(200);
    display.clearDisplay();
  }
}

void startup() {
  Serial.begin(9600);

//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessagelight );
  userScheduler.addTask( taskSendMessagepump );
  userScheduler.addTask( taskSendMessage );
  //taskSendMessage.enable();
  taskSendMessagelight.enable();
  taskSendMessagepump.enable();

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
  display.clearDisplay();
  display.display();

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);
  dht.begin();
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  set_clock();
}