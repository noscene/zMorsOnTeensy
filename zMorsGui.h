
#ifndef zMorsGui_h
#define zMorsGui_h

#include "Arduino.h"
#include "zMorsEngine.h"

#include <SPI.h>
#include <SSD_13XX.h>



// Now we using faster Display Lib from sumotoy, thanks for the great work!
// SCL on Pin 13   TODO: check Pullup on Teensy
// SDA on Pin 11   TODO: check Pullup on Teensy

#define __CS1   10
#define __DC  9
#define __RST 14



// little helper class for a single line
class zMorsGuiLine {
  public:

    typedef enum  {
      TITLE = 0, PARM = 1, PORT = 2, PRESET =3
    } my_type;

    my_type type;
    int16_t color = WHITE;
    String text = "";
    unsigned int * portPtr;
    float * parmPtr;
//    typedef void  (* presetLoader)();
//    presetLoader  functPtr;
    int preset;
    zMorsEngine * ae;
    
    zMorsGuiLine() {
    };

    void stepUp() {
      if (type == PORT) {
        if (portPtr[0] < BUS_COUNT - 1) {
          portPtr[0]++;
        }
      } else if (type == PARM) {
        parmPtr[0] *= 1.1;
        if (parmPtr[0] >= 1.0)
          parmPtr[0] = 1.0;
      }else if (type == PRESET) {
          ae->resetModules();
          switch(preset){
              case 0:   ae->patch1();  break;
              case 1:   ae->patch2();  break;
              case 2:   ae->patch3();  break;
              case 3:   ae->patch4();  break;
            }
      }
    };
    void stepDown() {
      if (type == PORT) {
        if (portPtr[0] > 0) {
          portPtr[0]--;
        }

      } else if (type == PARM) {
        parmPtr[0] *= 0.9;
        if (parmPtr[0] <= 0.0)
          parmPtr[0] = 0.00001;
      }
    };
};


class zMorsGui {

  private:
    int idx = 0;
    zMorsEngine * ae;
    // Adafruit_SSD1331 * display;


    SSD_13XX tft = SSD_13XX(__CS1, __DC,__RST);

  public:
    int topLine = 0;
    int maxLine = 0;
    zMorsGuiLine lines[160];

    zMorsGui() {
      tft.begin();
      tft.setBrightness(15);
      tft.clearScreen();
  };

    void init(zMorsEngine * e) {
      ae = e;
      updateLines();
      renderLines();
    };


    void updateDetalBox() {
      tft.fillRect(20, 20, 56, 34, BLACK);
      tft.setTextColor(WHITE);
      tft.setCursor(20,20);
      tft.setTextScale(2);
      float v = *lines[topLine].parmPtr;
      tft.print(v);
    };
    
    
    
    void renderLines() {
      tft.clearScreen();
      tft.setTextColor(WHITE);
      tft.setTextScale(1);
      topLine = RANGE(0, topLine, maxLine);
      int y = 0;
      for (int p = topLine ; p < topLine + 7 ; p++) {
        tft.setCursor(0, y);
        tft.setTextColor(lines[p].color);
        tft.print(lines[p].text);
        y += 8;
      }

    };

    void updateLines() {
      int idx = 0;

       lines[idx].color = RED;
       lines[idx].text = "Patch BassSeq";
       lines[idx].type = zMorsGuiLine::PRESET;
       lines[idx].preset = 0;
       lines[idx].ae = ae;
       idx++;

       lines[idx].color = RED;
       lines[idx].text = "Patch HiHatSeq";
       lines[idx].type = zMorsGuiLine::PRESET;
       lines[idx].preset = 1;
       lines[idx].ae = ae;
       idx++;

       lines[idx].color = RED;
       lines[idx].text = "Patch Kick";
       lines[idx].type = zMorsGuiLine::PRESET;
       lines[idx].preset = 2;
       lines[idx].ae = ae;
       idx++;

       lines[idx].color = RED;
       lines[idx].text = "FM  Test";
       lines[idx].type = zMorsGuiLine::PRESET;
       lines[idx].preset = 3;
       lines[idx].ae = ae;
       idx++;
      
      for (int i = 0 ; i < 10 ; i++) {
        if (ae->modules[i] != NULL) {
          zmModule * m = ae->modules[i];
          lines[idx].color = YELLOW;
          lines[idx].text = m->title;
          lines[idx].type = zMorsGuiLine::TITLE;
          idx++;
          for (int p = 0 ; p < 10 ; p++) {
            if (m->portName[p].length() > 0) {
              lines[idx].color = BLUE;

              while(m->portName[p].length() < 7){
                m->portName[p].append(" ");
              }
              
              
              
              
              if (m->portMap[p] == 0) {
                lines[idx].text = " " + m->portName[p] + " --";
              } else {
                lines[idx].text = " " + m->portName[p] + " " +  m->portMap[p];
              }
              lines[idx].type = zMorsGuiLine::PORT;
              lines[idx].portPtr = &m->portMap[p];
              idx++;
            }
          }
          for (int p = 0 ; p < 10 ; p++) {
            if (m->parameterName[p].length() > 0) {
              lines[idx].color = GREEN;



              while(m->parameterName[p].length() < 7){
                m->parameterName[p].append(" ");
              }

              
              lines[idx].text = " " + m->parameterName[p] + " " + String( RANGE(0.01, m->parameterMap[p], 9.99) , 2).substring(0,4);    ;
              lines[idx].type = zMorsGuiLine::PARM;
              lines[idx].parmPtr = &m->parameterMap[p];
              idx++;
            }
          }
        }
      }
      maxLine = idx-1;
    };

};



#endif

