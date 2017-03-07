
#ifndef zMorsModuleFilter_h
#define zMorsModuleFilter_h

#include "zmModule.h"

#define PORT_FILTER_OUT 0
#define PORT_FILTER_IN  1
#define PORT_FILTER_FRQ 2
#define PORT_FILTER_Q   3

class zmModuleFilter : public zmModule {
  public:

    zmModuleFilter() {
      title = "Filter";
      portName[PORT_FILTER_IN]  = "input";
      portName[PORT_FILTER_OUT] = "output";
      portName[PORT_FILTER_FRQ] = "frq";
      portName[PORT_FILTER_Q]   = "res";

      parameterName[0]       = "frq";
      parameterDefaults[0]        = 0.5;
      parameterName[1]       = "res";
      parameterDefaults[1]        = 0.5;
      
    }
    void genSample(float * bus) {
      bus[portMap[PORT_FILTER_OUT]] =  doFilter(bus[portMap[PORT_FILTER_IN]] ,
                                                RANGE(0.0, parameterMap[0] + bus[portMap[PORT_FILTER_FRQ]]  ,1.0) , 
                                                RANGE(0.0,parameterMap[1]  + bus[portMap[PORT_FILTER_Q]], 1.0)
                                                );
    };

    float doFilter(float in, float frq, float q) {
      static  float b_aflt1 = 0.0;
      static  float b_aflt2 = 0.0;
      static  float b_aflt3 = 0.0;
      static  float b_aflt4 = 0.0;
      float current_res = q;
      float b_fres = current_res * current_res * 100;
      float b_fenv = frq;
      float b_v = in;
      float b_rez = b_aflt4 - b_v; // no attenuation with rez, makes a stabler filter.
      b_v = b_v - (b_rez * b_fres); // b_fres = resonance amount. 0..4 typical "to selfoscillation", 0.6 covers a more saturated range.
      float b_vnc = b_v; // clip, and adding back some nonclipped, to get a dynamic like analog.
      // if (b_v > 1) {b_v = 1;} else if (b_v < -1) {b_v = -1;}
      b_v = RANGE(-1.0 , b_v, 1.0);
      b_v = b_vnc + ((-b_vnc + b_v) * 0.9840);
      b_aflt1 = b_aflt1 + ((-b_aflt1 + b_v) * b_fenv); // straightforward 4 pole filter, (4 normalized feedback paths in series)
      b_aflt2 = b_aflt2 + ((-b_aflt2 + b_aflt1) * b_fenv);
      b_aflt3 = b_aflt3 + ((-b_aflt3 + b_aflt2) * b_fenv);
      b_aflt4 = b_aflt4 + ((-b_aflt4 + b_aflt3) * b_fenv);
      return b_aflt4;
    }
};


#endif
