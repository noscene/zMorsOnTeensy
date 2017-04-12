#ifndef zMorsModuleMIX_h
#define zMorsModuleMIX_h

#include "zmModule.h"


#define PORT_MIX_OUT  0
#define PORT_MIX_IN1  1
#define PORT_MIX_IN2  2

class zmModuleMix : public  zmModule {
  public:

    zmModuleMix() {
      title = "MIX";
      portName[PORT_MIX_OUT]   = "out:   ";
      portName[PORT_MIX_IN1]   = "in1:   ";
      portName[PORT_MIX_IN2]   = "in2:   ";
    }

    void genSample(float * bus) {
      bus[portMap[PORT_MIX_OUT]] = bus[portMap[PORT_MIX_IN1]] + bus[portMap[PORT_MIX_IN2]];
    };
};


#endif
