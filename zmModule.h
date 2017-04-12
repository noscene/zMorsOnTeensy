
#ifndef zMorsModule_h
#define zMorsModule_h

#define BUS_COUNT 32
#define RANGE(vmin,v,vmax)  ( (isnan(v)) ? 0.0 :   min(max(v,vmin),vmax) )

/******************************************************/

class zmModule {
  public:

    float sampleRate = 40000.0;

    unsigned int   portMap[10]                 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    float          parameterMap[10]            = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    float          parameterDefaults[10]       = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    String         parameterName[10];
    String         portName[10];
    String         title                  = "unKnown";

    zmModule() {
      // analogReadResolution(12);
      // analogWriteResolution(10);
    };

    virtual void genSample(float * bus) {};

    int portCount() {
      for (int i = 0 ; i < 10 ; i++) {
        if (portName[i].length() == 0)
          return i;
      }
      return 10;
    }

    int parameterCount() {
      for (int i = 0 ; i < 10 ; i++) {
        if (parameterName[i].length() == 0)
          return i;
      }
      return 10;
    }

    void resetModule() {
      for (int i = 0 ; i < 10 ; i++) {
        portMap[i] = 0;
      }
      for (int i = 0 ; i < 10 ; i++) {
        parameterMap[i] = parameterDefaults[i];
      }
    };

    int lineCount() {
      return   portCount() + 1 + parameterCount(); // ports+title+parameter
    }

};



/******************************************************/
// here we try some "analog mocho sauce" :-) by selfmade vactrol
// TODO: try use different Condensators via OpenCollector Output ?!?!?!
// TODO: try prefilter PWM Out

#define PORT_VACTROL_PW_LED 0   // it the LED via PW
#define PORT_VACTROL_PW_IN  1   // its Audio RC hardware input via PW
#define PORT_VACTROL_RETURN 2   // its from ADC

#define PIN_VACTROL_LED  3
#define PIN_VACTROL_IN   4

class zmModuleVactrol : public zmModule {
  public:

    zmModuleVactrol() {
      title = "Vactrol";
      analogWriteResolution(12);
      analogWriteFrequency(PIN_VACTROL_LED, 375000);  // FTM1
      analogWriteFrequency(PIN_VACTROL_IN,  375000);
      portName[PORT_VACTROL_PW_LED] = "Led";
      portName[PORT_VACTROL_PW_IN]  = "input";
      portName[PORT_VACTROL_RETURN] = "output";
    }

    void genSample(float * bus) {
      analogWrite(PIN_VACTROL_LED, 2048.0 * (bus[portMap[PORT_VACTROL_PW_LED]]   + 1.0) );
      analogWrite(PIN_VACTROL_IN, 2048.0 * (bus[portMap[PORT_VACTROL_PW_IN]]   + 1.0) );

    };
};



#endif
