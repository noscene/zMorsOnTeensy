#ifndef zMorsModuleNoise_h
#define zMorsModuleNoise_h


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


#endif
