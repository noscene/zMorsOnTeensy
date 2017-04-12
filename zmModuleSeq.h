#ifndef zMorsModuleSeq_h
#define zMorsModuleSeq_h


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


        bus[portMap[PORT_SEQ_OUT]] = parameterMap[idx];
      }
    };


};

#endif
