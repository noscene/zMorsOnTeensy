/***************************************************
  This is a zMors Modular Synth for Arduino
 ****************************************************/
#include <Encoder.h>
#include <TimerOne.h>

#include "zMorsGui.h"
#include "zMorsEngine.h"

zMorsGui      gui   = zMorsGui();
zMorsEngine   audio = zMorsEngine();

#define ENCODER_PORT1 33
#define ENCODER_PORT2 34
#define ENCODER_KNOB  35

Encoder myEnc(ENCODER_PORT1, ENCODER_PORT2);

static bool needUpdateScreen = false;
static bool oldKnobState = 0;
static bool encoderState =false;

void update_encoder() {
  static long newPosition = 0;
  static long oldPosition = 0;
  newPosition = myEnc.read() * 0.25;
  if (oldKnobState != digitalRead(ENCODER_KNOB)) {
    oldKnobState = digitalRead(ENCODER_KNOB);
    needUpdateScreen = true;
    if(oldKnobState){
      encoderState =!encoderState; // toggle new state
    }
  }
  if (newPosition == oldPosition) {
    return;
  }

  needUpdateScreen = true;

  if (encoderState) {  // Encoder gedreuckt ?
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
}

void renderAudio() {
  audio.loop();
}

void setup(void) {
  pinMode(35, INPUT_PULLUP);
  gui.init(&audio);
  Timer1.initialize(10);   // TODO: make this as global parm
  Timer1.attachInterrupt(renderAudio); // render single sample each xxx time
}

void loop() {
  update_encoder();
  delayMicroseconds(100);
  if (needUpdateScreen) {
    needUpdateScreen = false;
    gui.renderLines();
    gui.updateLines();
    if (!encoderState) {
      gui.updateDetalBox();

    }
    delayMicroseconds(10000);

  }


}

