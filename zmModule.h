
#ifndef zMorsModule_h
#define zMorsModule_h

#include "Arduino.h"

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
#define PORT_NOISE_OUT 0
#define PORT_NOISE_AMP 1

class zmModuleNoise : public zmModule {
  public:
    const float g_fScale = 2.0f / 0xffffffff;
    int32_t wtg_x1 = 0x67452301;
    int32_t wtg_x2 = 0xefcdab89;
    zmModuleNoise() {
      title = "Noise";
      portName[PORT_NOISE_OUT] = "out";
      portName[PORT_NOISE_AMP] = "amp";
      parameterName[0]       = "level";
      parameterDefaults[0]        = 0.5;
    }

    void genSample(float * bus) {
      wtg_x1 ^= wtg_x2;
      bus[portMap[PORT_NOISE_OUT]] = wtg_x2 * g_fScale *  parameterMap[0] * bus[portMap[PORT_NOISE_AMP]];
      wtg_x2 += wtg_x1;
    };
};

/******************************************************/

#define PORT_AD_OUT 0
#define PORT_AD_TR  1

class zmModuleAD : public zmModule {
  public:

    // float last_sample;
    float last_out;
    bool  state_rst;
    bool  attack_state;
    float trigger_level = 0.2;
    zmModuleAD() {
      title = "Env AD";
      portName[PORT_AD_OUT] = "out";
      portName[PORT_AD_TR]     = "tr";
      parameterName[0]         = "attack";
      parameterDefaults[0]          = 0.5;
      parameterName[1]         = "release";
      parameterDefaults[1]          = 0.5;
    }

    void genSample(float * bus) {

      if (state_rst != (bus[portMap[PORT_AD_TR]] >= trigger_level) ) {
        state_rst = (bus[portMap[PORT_AD_TR]] >= trigger_level);
        if (state_rst) {
          attack_state = true;
        }
      }

      if (attack_state) {
        last_out *=  1.0 +   (1.0 - parameterMap[0]) * 0.01;
        if (last_out >= 1.0) {
          attack_state = false;
        }
      } else {
        last_out *=  1.0 -  (1.0 - parameterMap[1]) * 0.001;

        // Auto Trigger if TR not connected
        if (portMap[PORT_AD_TR] == 0) {
          if (last_out < 0.0001) {
            attack_state = true;
            last_out = 0.0001;
          }
        }


      }


      last_out = RANGE(0.00001, last_out, 1.0);
      bus[portMap[PORT_AD_OUT]] = last_out;
    };
};

/******************************************************/


#define PORT_SEQ_OUT 0
#define PORT_SEQ_CLK 1

class zmModuleSeq : public zmModule {
  public:


    zmModuleSeq() {
      title = "Sequencer";
      portName[PORT_SEQ_OUT] = "out";
      portName[PORT_SEQ_CLK] = "clk";

      parameterName[0]       = "#1";
      parameterDefaults[0]        = 0.1;
      parameterName[1]       = "#2";
      parameterDefaults[1]        = 0.1;
      parameterName[2]       = "#3";
      parameterDefaults[2]        = 0.1;
      parameterName[3]       = "#4";
      parameterDefaults[3]        = 0.1;
      parameterName[4]       = "#5";
      parameterDefaults[4]        = 0.1;
      parameterName[5]       = "#6";
      parameterDefaults[5]        = 0.1;
      parameterName[6]       = "#7";
      parameterDefaults[6]        = 0.1;
      parameterName[7]       = "#8";
      parameterDefaults[7]        = 0.1;
    }

    unsigned int idx = 0;
    bool state_rst = true;
    float trigger_level = 0.2;


    void genSample(float * bus) {

      if (state_rst != (bus[portMap[PORT_SEQ_CLK]] >= trigger_level) ) {
        state_rst = (bus[portMap[PORT_SEQ_CLK]] >= trigger_level);
        if (state_rst) {
          idx++;
          if (idx > 7) {
            idx = 0;
          }
        }


        bus[portMap[PORT_NOISE_OUT]] = parameterMap[idx];
      }
    };


};


/******************************************************/
#define PORT_OSC_OUT_SAW 0
#define PORT_OSC_OUT_SIN 1
#define PORT_OSC_OUT_SQR 2
#define PORT_OSC_FRQ 3
#define PORT_OSC_AMP 4

class zmModuleOsc : public zmModule {
  public:
    float thea = 0;
    const float B = 4.0f / M_PI;
    const float C = -4.0f / (M_PI * M_PI);
    zmModuleOsc() {
      title = "OSC";
      portName[PORT_OSC_OUT_SAW] = "out saw";
      portName[PORT_OSC_OUT_SIN] = "out sin";
      portName[PORT_OSC_OUT_SQR] = "out sqr";
      portName[PORT_OSC_FRQ]     = "frq";
      portName[PORT_OSC_AMP]     = "amp";
      parameterName[0]           = "thea";
      parameterDefaults[0]            = 0.005;
      parameterName[1]           = "level";
      parameterDefaults[1]            = 1.0;
      parameterName[2]           = "pw";
      parameterDefaults[2]            = 0.5;
      parameterName[3]           = "fm_deep";
      parameterDefaults[3]            = 0.5;
      thea = 0;
    }

    void genSample(float * bus) {
      thea += (parameterMap[0] ) + bus[portMap[PORT_OSC_FRQ]] * parameterMap[3];

      if (thea < -1.0) {
        thea = -1.0;
      } else if (thea >= 1.0) {
        thea -= 2.0;
      }
      if (portMap[PORT_OSC_OUT_SAW]) {
        // saw out
        bus[portMap[PORT_OSC_OUT_SAW]] = thea * (parameterMap[1] + bus[portMap[PORT_OSC_AMP]]) ;
        bus[0] = 0.0;
      }

      if (portMap[PORT_OSC_OUT_SQR]) {
        // sqr out
        if ( thea > parameterMap[2] ) {
          bus[portMap[PORT_OSC_OUT_SQR]] =   (parameterMap[1] + bus[portMap[PORT_OSC_AMP]]) ;
        } else {
          bus[portMap[PORT_OSC_OUT_SQR]] =   (parameterMap[1] + bus[portMap[PORT_OSC_AMP]]) * -1.0;
        }
        bus[0] = 0.0;
      }


      if (portMap[PORT_OSC_OUT_SIN]) {
        // sin out
        float x = thea  * M_PI;
        bus[portMap[PORT_OSC_OUT_SIN]] = ( B * x + C * x * fabs(x) ) * (parameterMap[1] + bus[portMap[PORT_OSC_AMP]]) ;
        bus[0] = 0.0;
      }

    };
};


#define PORT_IN1 0
#define PORT_IN2 1
#define PORT_IN3 2
#define PORT_IN4 3
#define PIN_IN1 36
#define PIN_IN2 37
#define PIN_IN3 38
#define PIN_IN4 39

class zmModuleIn : public zmModule {
  public:

    zmModuleIn() {
      title = "Analog Input";
      pinMode(PIN_IN1, INPUT);
      portName[PORT_IN1] = "input1";
      pinMode(PIN_IN2, INPUT);
      portName[PORT_IN2] = "input2";
      pinMode(PIN_IN3, INPUT);
      portName[PORT_IN3] = "input3";
      pinMode(PIN_IN4, INPUT);
      portName[PORT_IN4] = "input4";
      analogReadResolution(12);
    }

    void genSample(float * bus) {
      // TODO: very Slow - RoundRobin
      if (portMap[PORT_IN1])       bus[portMap[PORT_IN1]] = (float)analogRead(PIN_IN1) / 2048.0 - 1.0; // ToDo Scale!!!
      if (portMap[PORT_IN2])       bus[portMap[PORT_IN2]] = (float)analogRead(PIN_IN2) / 2048.0 - 1.0; // ToDo Scale!!!
      if (portMap[PORT_IN3])       bus[portMap[PORT_IN3]] = (float)analogRead(PIN_IN3) / 2048.0 - 1.0; // ToDo Scale!!!
      if (portMap[PORT_IN4])       bus[portMap[PORT_IN4]] = (float)analogRead(PIN_IN4) / 2048.0 - 1.0; // ToDo Scale!!!
    };
};

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
      //analogWrite(PIN_VACTROL_LED, 2048.0 * (bus[portMap[PORT_VACTROL_PW_LED]]   + 1.0) )
      //analogWrite(PIN_VACTROL_IN, 2048.0 * (bus[portMap[PORT_VACTROL_PW_IN]]   + 1.0) )

    };
};



#endif
