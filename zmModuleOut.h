
#ifndef zMorsModuleOut_h
#define zMorsModuleOut_h


/******************************************************/

// #define DAC0(a) *(volatile int16_t *)&(DAC0_DAT0L)=a
// #define DAC1(a) *(volatile int16_t *)&(DAC1_DAT1L)=a

#define PORT_OUT_L 0
#define PORT_OUT_R 1
#define PIN_OUT_L 21
#define PIN_OUT_R 22
class zmModuleOut : public zmModule {
  public:

    zmModuleOut() {
      title = "Analog Out";
      // analogWriteFrequency(PIN_OUT, 375000);
      portName[PORT_OUT_L] = "Left ";
      portName[PORT_OUT_R] = "Right";
      pinMode(PIN_OUT_L, OUTPUT);
      pinMode(PIN_OUT_R, OUTPUT);
      analogWriteResolution(12);
    }

    void genSample(float * bus) {
      analogWriteDAC0(2048.0 * (bus[portMap[PORT_OUT_L]]   + 1.0)   );
      analogWriteDAC1(2048.0 * (bus[portMap[PORT_OUT_R]]   + 1.0)  );

    };
};


#endif
