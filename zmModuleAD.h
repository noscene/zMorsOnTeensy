#ifndef zMorsModuleAD_h
#define zMorsModuleAD_h



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


#endif
