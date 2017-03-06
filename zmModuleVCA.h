
#ifndef zMorsModuleVCA_h
#define zMorsModuleVCA_h

#include "zmModule.h"


#define PORT_VCA_OUT 0
#define PORT_VCA_IN  1
#define PORT_VCA_CV  2

class zmModuleVCA : public  zmModule {
  public:

    zmModuleVCA() {
      title = "VCA";
      portName[PORT_VCA_OUT]  = "out:   ";
      portName[PORT_VCA_IN]   = "in:    ";
      portName[PORT_VCA_CV]   = "cv:    ";
    }

    void genSample(float * bus) {
      bus[portMap[PORT_VCA_OUT]] = bus[portMap[PORT_VCA_IN]] * bus[portMap[PORT_VCA_CV]];
    };
};


#endif
