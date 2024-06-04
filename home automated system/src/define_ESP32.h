// OLED FeatherWing buttons map to different pins depending on board.
// The I2C (Wire) bus may also be different.
#if defined(ESP32)
  #define BUTTON_A 2
  #define BUTTON_B 0
  #define BUTTON_C 4
  #define WIRE Wire
#endif