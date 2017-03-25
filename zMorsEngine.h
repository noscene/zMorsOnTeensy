
#ifndef zMorsEngine_h
#define zMorsEngine_h

#include <ArduinoJson.h>


#include "Arduino.h"
#include "zmModule.h"
#include "zmModuleFilter.h"
#include "zmModuleVCA.h"
#include "zmModuleMIX.h"

class zMorsEngine {

  public:
    float       bus[BUS_COUNT];
    zmModule *  modules[20] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

    zmModuleIn      * mIN  ;
    zmModuleOsc     * mOsc1 ;
    zmModuleOsc     * mOsc2 ;
    zmModuleNoise   * mNoise ;
    zmModuleMix     * mMix;
    zmModuleFilter  * mFilter;
    zmModuleVCA     * mVCA ;
    zmModuleSeq     * mSeq ;
    zmModuleOut     * mOut ;
    zmModuleAD      * mEnv1 ;
    
  public:
    zMorsEngine() {

      // create engine
      mIN     = new zmModuleIn();
      mOsc1   = new zmModuleOsc();
      mOsc2   = new zmModuleOsc();
      mNoise  = new zmModuleNoise();
      mMix    = new zmModuleMix();
      mFilter = new zmModuleFilter();
      mVCA    = new zmModuleVCA();
      mSeq    = new zmModuleSeq();
      mOut    = new zmModuleOut();
      mEnv1   = new zmModuleAD();

      // wishlist
      // 3. osc
      // 2. env
      // 2 offset/leveler
      // trigger 8 x 4  gen (bin Muster as parm)




      mOsc1->title="Osc1";
      mOsc2->title="Osc2";

      modules[0] = mIN;
      modules[1] = mOsc1;
      modules[2] = mOsc2;
      modules[3] = mNoise;
      modules[4] = mMix;
      modules[5] = mFilter;
      modules[6] = mEnv1;
      modules[7] = mVCA;
      modules[8] = mSeq;
      modules[9] = mOut;

      resetModules();
      patch4();
    };

    void resetModules(){
      for(int i =0 ; i< 10 ; i++){
         modules[i]->resetModule();       
      }  
    }
    
    void patch1() {
      // BassLine

      // Patch CV Sequencer
      mOsc1->portMap[PORT_OSC_OUT_SAW]    = mFilter->portMap[PORT_FILTER_IN] = 1;
      mFilter->portMap[PORT_FILTER_OUT]   = mOut->portMap[PORT_OUT_L] = 2;
      mOsc2->portMap[PORT_OSC_OUT_SQR]    = mSeq->portMap[PORT_SEQ_CLK] =  mEnv1 ->portMap[PORT_AD_TR] = 3;
      mSeq->portMap[PORT_SEQ_OUT]         = mFilter->portMap[PORT_FILTER_FRQ] = 4;

      // Connect AD env and set osc amp to 0 (add by env)
      mEnv1 ->portMap[PORT_AD_OUT]        = mOsc1->portMap[PORT_OSC_AMP] = 5;
      mOsc1->parameterMap[1] = 0.0;


      mOsc2->parameterMap[0] = 0.0003;
      mFilter->parameterMap[0] = 0.0003;
      mFilter->parameterMap[1] = 0.1;
      mSeq->parameterMap[0] = 0.2;
      mSeq->parameterMap[1] = 0.1;
      mSeq->parameterMap[2] = 0.0;
      mSeq->parameterMap[3] = 0.0;
      mSeq->parameterMap[4] = 0.5;
      mSeq->parameterMap[5] = 0.3;
      mSeq->parameterMap[6] = 0.9;
      mSeq->parameterMap[7] = 0.1;
      
    };


   void patch2() {
      // HiHats Seq
      // Patch CV Sequencer
      mNoise->portMap[PORT_NOISE_OUT]    = mFilter->portMap[PORT_FILTER_IN] = 1;
      mFilter->portMap[PORT_FILTER_OUT]   = mOut->portMap[PORT_OUT_L] = 2;
      mOsc2->portMap[PORT_OSC_OUT_SQR]    = mSeq->portMap[PORT_SEQ_CLK] =  mEnv1 ->portMap[PORT_AD_TR] = 3;
      mSeq->portMap[PORT_SEQ_OUT]         = mFilter->portMap[PORT_FILTER_FRQ] = 4;

      // Connect AD env and set osc amp to 0 (add by env)
      mEnv1 ->portMap[PORT_AD_OUT]        =  mNoise->portMap[PORT_NOISE_AMP] = 5;
      mOsc1->parameterMap[1] = 0.0;

      mOsc2->parameterMap[0] = 0.0005;
      mFilter->parameterMap[0] = 0.0003;
      mFilter->parameterMap[1] = 0.1;
      mSeq->parameterMap[0] = 0.4;
      mSeq->parameterMap[1] = 0.0;
      mSeq->parameterMap[2] = 0.3;
      mSeq->parameterMap[3] = 0.0;
      mSeq->parameterMap[4] = 0.5;
      mSeq->parameterMap[5] = 0.0;
      mSeq->parameterMap[6] = 1.1;
      mSeq->parameterMap[7] = 0.8;
      
    };


    void patch3() {
      // HiHats Seq
      // Patch CV Sequencer
      // Patch CV Sequencer
      mOsc1->portMap[PORT_OSC_OUT_SIN]    = mOut->portMap[PORT_OUT_L] = 1;
      mOsc2->portMap[PORT_OSC_OUT_SQR]    = mSeq->portMap[PORT_SEQ_CLK] =  mEnv1 ->portMap[PORT_AD_TR] = 3;
      mEnv1->portMap[PORT_AD_OUT]         = mOsc1->portMap[PORT_OSC_FRQ] = mOsc1->portMap[PORT_OSC_AMP] = 4;

      // Connect AD env and set osc amp to 0 (add by env)
      mSeq->portMap[PORT_SEQ_OUT]  =0;
      mOsc1->parameterMap[0] = 0.0020;    // frq
      mOsc1->parameterMap[1] = 1.0;       // amp
      mOsc1->parameterMap[3] = 0.016;       // fm deep
      mOsc2->parameterMap[0] = 0.00005;
      mEnv1->parameterMap[0] = 0.000001;
      mEnv1->parameterMap[1] = 0.48;
      
    };

    void patch4() {
      // FM Kick
      mOsc1->portMap[PORT_OSC_OUT_SIN]    = mOut->portMap[PORT_OUT_L] = 1;
      mOsc2->portMap[PORT_OSC_OUT_SIN]    = mOsc1->portMap[PORT_OSC_FRQ] = 2;
      mEnv1->portMap[PORT_AD_OUT]         = mOsc2->portMap[PORT_OSC_AMP] = 3;

      // Connect AD env and set osc amp to 0 (add by env)
      mSeq->portMap[PORT_SEQ_OUT]  =0;
      mOsc1->parameterMap[0] = 0.0020;    // frq
      mOsc1->parameterMap[1] = 1.0;       // amp
      mOsc1->parameterMap[3] = 0.016;       // fm deep
      mOsc2->parameterMap[0] = 0.00005;
      mEnv1->parameterMap[0] = 0.000001;
      mEnv1->parameterMap[1] = 0.48;
      
    };



    String patch2Json(){
      /*
      StaticJsonBuffer<200> jsonBuffer;
      
      JsonObject& root = jsonBuffer.createObject();
      root["version"] = 1.0;
      JsonArray& data = root.createNestedArray("modules");
      //data.add()

      */
      return String("");
    };

    // Render all Modules
    void loop() {
      for (int i = 0 ; i < 10 ; i ++) {
        if (modules[i] == NULL) {
          return;
        }
        modules[i]->genSample(&bus[0]);
        bus[0] = 0.0; // Bus 0 hat keine Verbindung
      }
    };


    // compute lineCount
    int lineCount() {
      int cnt = 0;
      for (int i = 0 ; i < 10 ; i++) {
        if (modules[i] != NULL) {
          cnt += modules[i]->lineCount();
        }
      }
      return cnt;
    }

};


#endif
