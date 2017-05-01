#include <Keyboard.h>

#define CHORD_DELAY 70
#define RESET_DELAY 110

int chord = 0;
uint8_t key = 0;
int alt = 0;
boolean command = 0;
boolean shift = 0;

void setup() {
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
  return digitalRead(0) == LOW || digitalRead(1) == LOW || digitalRead(2) == LOW ||
    digitalRead(3) == LOW || digitalRead(4) == LOW;
}

uint8_t alt0map[31] = {
  0,               // nil
  ' ',             // A
  KEY_LEFT_GUI,    // B
  KEY_DOWN_ARROW,  // AB
  KEY_LEFT_SHIFT,  // C
  KEY_BACKSPACE,   // AC
  KEY_LEFT_ARROW,  // BC
  's',             // ABC
  0,               // D nil (alt)
  '{',             // AD
  'f',             // BD
  '.',             // ABD
  KEY_RIGHT_ARROW, // CD
  '(',             // ACD
  KEY_ESC,         // BCD
  '[',             // ABCD
  KEY_RETURN,      // E
  '}',             // AE
  'c',             // BE
  't',             // ABE
  'v',             // CE
  0,               // ACE (nil)
  0,               // BCE (nil for now)
  0,               // ABCE (nil)
  KEY_UP_ARROW,    // DE
  ')',             // ADE
  KEY_TAB,         // BDE
  ']',             // ABDE
  '_',             // CDE
  0,               // ACDE (nil)
  '='              // BCDE
};

// uint8_t[][3] altsMap = {};
void pressKeyAndReset() {
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
  Keyboard.release(key);

  if (shift) Keyboard.release(KEY_LEFT_SHIFT);
  if (command) Keyboard.release(KEY_LEFT_GUI);

  // add logic
  chord = 0;
  alt = 0;
  command = 0;
  shift = 0;
}

void loop() {
  if (anyKeyPressed()) {
    delay(CHORD_DELAY);

    if (keyPressed(0)) chord = chord + 1;
    if (keyPressed(1)) chord = chord + 2;
    if (keyPressed(2)) chord = chord + 4;
    if (keyPressed(3)) chord = chord + 8;
    if (keyPressed(4)) chord = chord + 16;

    if (chord == 8) {
      alt = (alt + 1) % 3;
    } else {
      key = alt0map[chord];
      pressKeyAndReset();
    }

    delay(RESET_DELAY);
  }

}
