#include <funktions_sensor.h>

void setup() {
  Serial.begin(9600);
  mesh.setDebugMsgTypes(ERROR | STARTUP);
  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  pinMode(RELAY_PIN, OUTPUT);   // Define RELAY_PIN as OUTPUT and set it to low (off)
  digitalWrite(RELAY_PIN, LOW);
  pinMode(PUMP_PIN, OUTPUT);    // Define PUMP_PIN as OUTPUT and set it to low (off)
  digitalWrite(PUMP_PIN, LOW);
  pinMode(TRIGGER_PIN, OUTPUT); // Sets the TRIGGER_PIN as an Output
  pinMode(ECHO_PIN, INPUT);     // Sets the ECHO_PIN as an Input

  userScheduler.addTask(taskSendMessagewater);
  userScheduler.addTask(taskSendMessageLDR);
  userScheduler.addTask(taskCounter);
  userScheduler.addTask(taskpump);
  userScheduler.addTask(taskpumpoff);
  taskSendMessagewater.enable();
  taskSendMessageLDR.enable();
}

void loop() {
  mesh.update();
}