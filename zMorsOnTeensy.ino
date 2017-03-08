/***************************************************
  This is a zMors Modular Synth for Arduino
 ****************************************************/
#include <Encoder.h>
#include <TimerOne.h>
#include "zMorsGui.h"
#include "zMorsEngine.h"

zMorsGui  gui = zMorsGui();
zMorsEngine  audio = zMorsEngine();
Encoder myEnc(33, 34);

void update_encoder() {
  static long newPosition = 0;
  static long oldPosition = 0;
  newPosition = myEnc.read() * 0.25;
  
  if (newPosition == oldPosition) {
    return;
  }
  
  

  if (digitalRead(35)) {  // Encoder gedreuckt ?
    if (newPosition > oldPosition ) {
      gui.topLine--;
    } else if (newPosition  < oldPosition) {
      gui.topLine++;
    }
  } else {
    if (newPosition > oldPosition ) {
      gui.lines[gui.topLine].stepDown();
    } else if (newPosition  < oldPosition) {
      gui.lines[gui.topLine].stepUp();
    }
  }
  oldPosition = newPosition;
  gui.renderLines();
  gui.updateLines();
}

void renderAudio() {
    audio.loop();
}

void setup(void) {
  pinMode(35, INPUT_PULLUP);
  gui.init(&audio);
  Timer1.initialize(25);
  Timer1.attachInterrupt(renderAudio); // blinkLED to run every 0.15 seconds
}

void loop() {
  update_encoder();
  delayMicroseconds(400);

}

