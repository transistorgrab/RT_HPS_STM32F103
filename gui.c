/** this file contains functions for the graphical user interface
 * author: ansc
 * date:   2022-05-06
 *
 * OLED pixel density: 0.3 mm/px => 10 px = 3 mm
 * 10 point font: 7 px widest character + 2 px spacing; 7 px height
 * 12 point font: 8 px widest character + 1 px spacing; 9 px height
 */

#include "gfx.h"

typedef enum {
  OUTPUT_OFF,
  OUTPUT_ON,
  MENU_ON,
  MENU_OFF
}guistatus;

typedef struct guistates_ {
  guistatus A;
  guistatus B;
  guistatus L;
  guistatus R;
  guistatus ROT;
} gui_states;

typedef enum {
  VOL_A_SET,
  CUR_A_SET,
  VOL_B_SET,
  CUR_B_SET,
  INACTIVE      // set when MENU = OFF
}rot_elements;
#define ROT_ELEMENTS ((int)4) // used to cycle through active elements

// variables for the variable text labels
GHandle ghSetVolA, ghSetVolB, ghSetCurA, ghSetCurB, ghAdcCurA, ghAdcCurB, ghAdcVolA, ghAdcVolB;

// this function creates and places the variable text areas that need regular updating
static void guiCreateWidgets(void){

  GWidgetInit wi1, wi2, wi3, wi4, wi5, wi6, wi7, wi8;
  gwinSetDefaultFont(gdispOpenFont("DejaVuSans10"));   // Set default font
  gwinSetDefaultStyle(&BlackWidgetStyle, gTrue);         // Set default widget style

  int disp_width  = gdispGetWidth();
  int disp_height = gdispGetHeight();

  // label parameters
  // ghSetVolA: 5 characters * decimal point = 5×9 + 1×3 = 48 px
  gwinWidgetClearInit(&wi1);
  wi1.g.show   = gTrue;
  wi1.g.x      = 22;
  wi1.g.y      =  1;
  wi1.g.width  = 48;
  wi1.g.height =  9;
  wi1.text     = "A.setV";
  ghSetVolA    = gwinLabelCreate(NULL, &wi1);

  // ghSetCurA: 5 characters + decimal point = 5×9 + 1×3 = 48 px
  gwinWidgetClearInit(&wi2);
  wi2.g.show   = gTrue;
  wi2.g.x      = wi1.g.x + wi1.g.width + 1; // place right of wi1
  wi2.g.y      = wi1.g.y;   // same y position
  wi2.g.width  = 48;
  wi2.g.height = wi1.g.height;
  wi2.text     = "A.setC";
  ghSetCurA    = gwinLabelCreate(NULL, &wi2);

  // ghSetVolB: 5 characters + decimal point = 5×9 + 1×3 = 48 px
  gwinWidgetClearInit(&wi3);
  wi3.g.show   = gTrue;
  wi3.g.x      = disp_width/2 + wi1.g.x + 1; // place right of half display width
  wi3.g.y      = wi1.g.y;   // same y position
  wi3.g.width  = 48;
  wi3.g.height = wi1.g.height;
  wi3.text     = "B.setV";
  ghSetVolB    = gwinLabelCreate(NULL, &wi3);

  // ghSetCurB: 5 characters + decimal point = 5×9 + 1×3 = 48 px
  gwinWidgetClearInit(&wi4);
  wi4.g.show   = gTrue;
  wi4.g.x      = wi3.g.x + wi3.g.width + 1; // place right of wi3
  wi4.g.y      = wi1.g.y;   // same y position
  wi4.g.width  = 48;
  wi4.g.height = wi1.g.height;
  wi4.text     = "B.setC";
  ghSetCurB    = gwinLabelCreate(NULL, &wi4);

  // ghAdcVolA: 5 characters + decimal point = 5×10 + 1×3 = 53 px
  gwinWidgetClearInit(&wi5);
  wi5.g.show   = gTrue;
  wi5.g.x      = 13;
  wi5.g.y      = 12;
  wi5.g.width  = 53;
  wi5.g.height = 11;
  wi5.text     = "A.adcV";
  ghAdcVolA    = gwinLabelCreate(NULL, &wi5);

  // ghAdcCurA: 5 characters + decimal point = 5×10 + 1×3 = 53 px
  gwinWidgetClearInit(&wi6);
  wi6.g.show   = gTrue;
  wi6.g.x      = wi5.g.x + wi5.g.width + 1;
  wi6.g.y      = wi5.g.y;
  wi6.g.width  = wi5.g.width;
  wi6.g.height = wi5.g.height;
  wi6.text     = "A.adcC";
  ghAdcCurA    = gwinLabelCreate(NULL, &wi6);

  // ghAdcVolB: 5 characters + decimal point = 5×10 + 1×3 = 53 px
  gwinWidgetClearInit(&wi7);
  wi7.g.show   = gTrue;
  wi7.g.x      = disp_width/2 + wi5.g.x + 1; // right from half of display
  wi7.g.y      = wi5.g.y;
  wi7.g.width  = wi5.g.width;
  wi7.g.height = wi5.g.height;
  wi7.text     = "B.adcV";
  ghAdcVolB    = gwinLabelCreate(NULL, &wi7);

  // ghAdcCurB: 5 characters + decimal point = 5×10 + 1×3 = 53 px
  gwinWidgetClearInit(&wi8);
  wi8.g.show   = gTrue;
  wi8.g.x      = wi7.g.x + wi7.g.width + 1;
  wi8.g.y      = wi5.g.y;
  wi8.g.width  = wi5.g.width;
  wi8.g.height = wi5.g.height;
  wi8.text     = "B.adcV";
  ghAdcCurB    = gwinLabelCreate(NULL, &wi8);

}
