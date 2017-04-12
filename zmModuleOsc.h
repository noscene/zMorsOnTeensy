#ifndef zMorsModuleOsc_h
#define zMorsModuleOsc_h



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


#endif
