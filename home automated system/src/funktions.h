#include <bit_map.h>

int max_states = 3;
int min_states = 1;
int state = 1;

void state_plus(){
  state += 1;
  if (state > max_states) {
    state = min_states;
  }
}

void state_minus(){
  state -= 1;
  if (state < min_states) {
    state = max_states;
  }
}

int light_state = 1;
void light_setting(){
  display.clearDisplay();
  while (true){
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0), display.print("t");    // tænd lys ved
    display.drawBitmap(12, 0, ea_16x16, 16, 16, WHITE);
    display.setCursor(24,0), display.print("nd lys ved");
    display.setTextSize(2);
    if (light_state == 1) {
      display.setCursor(20,32), display.print("dunkelt");
      msg_light = 750;
    }
    else if (light_state == 2) {
      display.setCursor(20,32), display.print("lidt lys");
      msg_light = 1500;
    }
    else if (light_state == 3) {
      display.setCursor(20,32), display.print("meget lys");
      msg_light = 2250;
    }
    if(!digitalRead(BUTTON_A)) light_state = light_state + 1;
    if (light_state == 4){
      light_state = 1;
    }
    if(!digitalRead(BUTTON_C)) light_state = light_state - 1;
    if (light_state == 0){
      light_state = 3;
    }

    display.display();
    delay(200);
    display.clearDisplay();

    if (lastButtonStateB == 1 && digitalRead(BUTTON_B) == 0){
      break;
    }
    lastButtonStateB = digitalRead(BUTTON_B);
    delay(20);
  }
}
