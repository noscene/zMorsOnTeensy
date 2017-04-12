
#ifndef zMorsModuleIn_h
#define zMorsModuleIn_h

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


#endif
