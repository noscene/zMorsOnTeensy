
#ifndef zMorsGui_h
#define zMorsGui_h

#include "Arduino.h"
#include "zMorsEngine.h"

// You can use any (4 or) 5 pins
#define sclk 13
#define mosi 11
#define cs   10
#define rst  9
#define dc   8

// Color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

#include <Adafruit_GFX.h>
#include "Adafruit_SSD1331.h"
#include <SPI.h>

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
    Adafruit_SSD1331 * display;
  public:
    int topLine = 0;
    zMorsGuiLine lines[100];

    zMorsGui() {
      display =  new Adafruit_SSD1331(cs, dc, rst);
      display->begin();
      display->fillScreen(BLACK);
    };

    void init(zMorsEngine * e) {
      ae = e;
      updateLines();
      renderLines();
    };

    void renderLines() {
      display->fillRect2(0, 0, 128, 64, BLACK);
      display->setTextColor(WHITE);
      topLine = RANGE(0, topLine, 62);
      int y = 0;
      for (int p = topLine ; p < topLine + 7 ; p++) {
        display->setCursor(0, y);
        display->setTextColor(lines[p].color);
        display->print(lines[p].text);
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
              lines[idx].text = " " + m->parameterName[p] + " " +  m->parameterMap[p];
              lines[idx].type = zMorsGuiLine::PARM;
              lines[idx].parmPtr = &m->parameterMap[p];
              idx++;
            }
          }
        }
      }
    };

};



#endif

