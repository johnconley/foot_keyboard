#include <Keyboard.h>

#define CHORD_DELAY 70
#define RESET_DELAY 110

int chord = 0;
// uint8_t key = 0;
int alt = 0;
boolean command = 0;
boolean shift = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
}

boolean keyPressed(int n) {
  return digitalRead(n) == LOW;
}

boolean anyKeyPressed() {
  return keyPressed(0) || keyPressed(1) || keyPressed(2) || keyPressed(3) || keyPressed(4);
}

// {key_to_be_pressed, modifier_key_to_press_it_with}
uint8_t alt0map[31][2] = {
  {0, 0},               // nil
  {' ', 0},             // A
  {KEY_LEFT_GUI, 0},    // B
  {KEY_UP_ARROW, 0},    // AB
  {' ', KEY_LEFT_GUI},  // C
  {KEY_BACKSPACE, 0},   // AC
  {KEY_LEFT_ARROW, 0},  // BC
  {'s', KEY_LEFT_GUI},             // ABC
  {0, 0},               // D nil (alt)
  {'{', KEY_LEFT_GUI},             // AD
  {'f', KEY_LEFT_GUI},             // BD
  {'.', 0},             // ABD
  {KEY_RIGHT_ARROW, 0}, // CD
  {'(', 0},             // ACD
  {KEY_ESC, 0},         // BCD
  {'[', KEY_LEFT_GUI},             // ABCD
  {KEY_RETURN, 0},      // E
  {'}', KEY_LEFT_GUI},             // AE
  {'c', KEY_LEFT_SHIFT},             // BE
  {'t', KEY_LEFT_GUI},             // ABE
  {'v', KEY_LEFT_GUI},             // CE
  {0, 0},               // ACE (nil)
  {'F', KEY_LEFT_GUI},               // BCE (nil for now)
  {0, 0},               // ABCE (nil)
  {KEY_DOWN_ARROW, 0},  // DE
  {')', 0},             // ADE
  {KEY_TAB, KEY_LEFT_GUI},         // BDE
  {']', KEY_LEFT_GUI},             // ABDE
  {'_', 0},             // CDE
  {0, 0},               // ACDE (nil)
  {'=', 0}              // BCDE
};

uint8_t alt1map[31][2] = {
  {0, 0},               // nil
  {'m', 0},             // A
  {'r', 0},             // B
  {'p', 0},             // AB
  {'i', 0},             // C
  {'b', 0},             // AC
  {'l', 0},             // BC
  {0, 0},             // ABC (nil for now)
  {0, 0},               // D nil (alt)
  {'d', 0},             // AD
  {'h', 0},             // BD
  {',', 0},             // ABD
  {'k', 0},             // CD
  {'a', 0},             // ACD
  {'u', 0},             // BCD
  {'x', 0},             // ABCD
  {'n', 0},             // E
  {'e', 0},             // AE
  {'j', 0},             // BE
  {'w', 0},             // ABE
  {':', 0},             // CE
  {0, 0},               // ACE (nil)
  {0, 0},               // BCE (nil for now)
  {0, 0},               // ABCE (nil)
  {'g', 0},             // DE
  {'o', 0},             // ADE
  {'"', 0},             // BDE
  {'y', 0},             // ABDE
  {'-', 0},             // CDE
  {0, 0},               // ACDE (nil)
  {'z', 0}              // BCDE
};

void pressKey(uint8_t key) {
  if (key == KEY_LEFT_GUI) {
    command = !command;
    return;
  }

  if (key == KEY_LEFT_SHIFT) {
    shift = !shift;
    return;
  }

  if (shift) Keyboard.press(KEY_LEFT_SHIFT);
  if (command) Keyboard.press(KEY_LEFT_GUI);

  Keyboard.press(key);
  delay(10);
  Keyboard.release(key);

  if (shift) Keyboard.release(KEY_LEFT_SHIFT);
  if (command) Keyboard.release(KEY_LEFT_GUI);

  // add logic
  command = 0;
  shift = 0;
}

void loop() {
  uint8_t keys[2];
  if (alt == 0) digitalWrite(LED_BUILTIN, HIGH);
  if (alt == 1) digitalWrite(LED_BUILTIN, LOW);
  if (anyKeyPressed()) {
    delay(CHORD_DELAY);

    chord = 0;
    // key = 0;
    if (keyPressed(0)) chord = chord + 1;
    if (keyPressed(1)) chord = chord + 2;
    if (keyPressed(2)) chord = chord + 4;
    if (keyPressed(3)) chord = chord + 8;
    if (keyPressed(4)) chord = chord + 16;

    if (chord == 8) {
      alt = (alt + 1) % 2;
    } else {
      if (alt == 0) { //keys = alt0map[chord];
        if (alt0map[chord][1] != 0) pressKey(alt0map[chord][1]);
        pressKey(alt0map[chord][0]);
      }
      if (alt == 1) { // key = alt1map[chord];
        if (alt1map[chord][1] != 0) pressKey(alt1map[chord][1]);
        pressKey(alt1map[chord][0]);
      }
    }

    delay(RESET_DELAY);
  }
}
