#include <painlessMesh.h>
#include <check_msg.h>

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555
#define   LED_PIN         33

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;
// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain
void sendMessagelight() ;
void sendMessagepump() ;

String receved_msg;

Task taskSendMessage( TASK_SECOND * 4 , TASK_FOREVER, &sendMessage );
Task taskSendMessagelight( TASK_SECOND * 4 , TASK_FOREVER, &sendMessagelight);
Task taskSendMessagepump( TASK_SECOND * 10 , TASK_FOREVER, &sendMessagepump);

void sendMessage() {
  String msg = "Hello from node ";
  msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  Serial.println("Message Sent: " + msg);
}

void sendMessagelight() {
  String msg = "light: " + String(msg_light);
  mesh.sendBroadcast( msg );
  Serial.println("Message Sent: " + msg);
}

void sendMessagepump() {
  String msg = "pump: " + String(msg_pump);
  mesh.sendBroadcast( msg );
  Serial.println("Message Sent: " + msg);
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  //Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
  check_msg(msg.c_str());
  if (msg_type(msg.c_str(), String("water"))){
    if (msg_water == 0 || msg_pump == 0) {
      digitalWrite(LED_PIN, LOW);    // turn off led if pump needs water
      Serial.println("led off");
    }
    else if (msg_water == 1) {
      digitalWrite(LED_PIN, HIGH);   // turn on led if pump needs water
      Serial.println("led on");
    }
  }
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}