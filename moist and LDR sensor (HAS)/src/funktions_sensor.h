#include <Arduino.h>
#include <painlessMesh.h>
#include <check_msg.h>

#define MESH_PREFIX     "whateverYouLike"
#define MESH_PASSWORD   "somethingSneaky"
#define MESH_PORT       5555
#define SOIL_PIN    39
#define LDR_PIN     36
#define RELAY_PIN   25
#define PUMP_PIN    33
#define TRIGGER_PIN 32
#define ECHO_PIN    34
#define PUMP_PIN    33
#define SOUND_SPEED 0.034

Scheduler userScheduler;
painlessMesh mesh;

// Global Variables
int counter = 0, pumpcounter = 0, pumpoffcounter = 1800;
int min_soil = 50;      // min value for soil moister befor plant needs water
long duration;
float distanceCm;
float max_water = 8.00; // max distans water is alowed to fall away from sesor
bool light = false;

void incrementCounter();
void sendMessagewater();
void sendMessageLDR();
void pumptimer();
void pumpofftimer();

Task taskCounter(TASK_SECOND * 1, TASK_FOREVER, &incrementCounter);           // Counter Task - increment every second
Task taskpump(TASK_SECOND * 1, TASK_FOREVER, &pumptimer);                     // Pump timer - disables pump after some secends
Task taskpumpoff(TASK_SECOND * 1, TASK_FOREVER, &pumpofftimer);               // Pump off timer - prevents pump from acivating for a set time after activation
Task taskSendMessagewater(TASK_SECOND * 6, TASK_FOREVER, &sendMessagewater);  // set water to one message every 15 min
Task taskSendMessageLDR(TASK_SECOND * 3, TASK_FOREVER, &sendMessageLDR);      // set LDR to one message every minute

// Increment counter function
void incrementCounter() {
  counter++;
  if (counter >= 8) {                  // Set the limit as per your need, here it's 8 seconds
    light = !light;
    if (light == true) {
      digitalWrite(RELAY_PIN, HIGH);
    }
    else {
      digitalWrite(RELAY_PIN, LOW);
    }
    counter = 0;                       // Reset counter or stop the task if no longer needed
    taskCounter.disable();
  }
}

void pumptimer() {
  pumpcounter++;
  Serial.println(pumpcounter);
  if (pumpcounter >= 5) {                  // Set the limit as per your need, here it's 8 seconds
    pumpcounter = 0;                       // Reset pumpcounter or stop the task if no longer needed
    digitalWrite(PUMP_PIN, LOW);
    taskpump.disable();
  }
}

void pumpofftimer() {
  pumpoffcounter++;
  if (pumpoffcounter > 1800) {
    taskpumpoff.disable();
  }
}

void sendMessagewater() {
  digitalWrite(TRIGGER_PIN, LOW);           // Clears the TRIGGER_PIN
  delayMicroseconds(2);
  
  digitalWrite(TRIGGER_PIN, HIGH);          // Sets the TRIGGER_PIN on HIGH state for 10 micro seconds
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  
  duration = pulseIn(ECHO_PIN, HIGH);       // Reads the ECHO_PIN, returns the sound wave travel time in microseconds
  distanceCm = duration * SOUND_SPEED/2;    // Calculate the distance
    
  //Serial.print("Distance (cm): ");
  //Serial.println(distanceCm);

  if (distanceCm > max_water){              // if water low, turn off pump and send water message to turn on "not enough water" LED
    String msg = "water: " + String(1);
    mesh.sendBroadcast(msg);
    digitalWrite(PUMP_PIN, LOW);
    msg_water = 1;
    Serial.println("Message Sent: " + msg);
  }
  else {                                    // send water message to turn off "not enough water" LED
    String msg = "water: " + String(0);
    mesh.sendBroadcast(msg);
    msg_water = 0;
    Serial.println("Message Sent: " + msg);
  }
}

void sendMessageLDR() {
  String msg = "LDR: " + String(analogRead(LDR_PIN));
  mesh.sendBroadcast(msg, true);
  //Serial.println("Message Sent: " + msg);
}

void receivedCallback(uint32_t from, String &msg) {
  //Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
  check_msg(msg.c_str());
  if (msg_type(msg.c_str(), String("LDR"))){
    if (msg_LDR < msg_light && light == false) {
      taskCounter.enable();
    }
    else if (msg_LDR > msg_light && light == true) {
      taskCounter.enable();
    }
    else {
      taskCounter.disable();
    }
  }
  else if (msg_type(msg.c_str(), String("pump"))) {
    if (msg_pump == 0) {                      // if night time, turn off pump 
      digitalWrite(PUMP_PIN, LOW);
    }
    else if (analogRead(SOIL_PIN) < min_soil && msg_pump == 1 && msg_water == 0 && pumpoffcounter > 30) {    // if plant needs water and daytime, water plant and set time limit, 1800 = 30 min, 300 = 5 min
      digitalWrite(PUMP_PIN, HIGH);
      pumpoffcounter = 0;
      taskpump.enable();
      taskpumpoff.enable();
    }
  }
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.println("Changed connections");
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}