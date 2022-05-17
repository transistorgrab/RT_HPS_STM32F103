/** this file contains buttons related code */

#include "ch.h"
#include "hal.h"
#include "board.h"

#define LONGPRESSEDMS 1000   // 1000 ms threshold before declaring long press

static systime_t startTimeA, startTimeB, startTimeR, startTimeL, startTimeRot;

typedef enum {
  CLEARED,
  CLICKED,
  LONGPRESSED,
  RISINGEDGE
}buttonstate;

typedef struct buttons_ {
  buttonstate A;    // input button for output A
  buttonstate B;    // input button for output B
  buttonstate L;    // Left input button
  buttonstate R;    // Right input button
  buttonstate ROT;  // rotary encoded button
} buttons;

buttons active;

int16_t rot_up_down; //holds the identified number of clicks for the rotary input

void button_cb(void *arg){
  (void)arg;
  if(palReadLine(LINE_BUTTON_A) == PAL_HIGH){ // rising edge starts timer
    startTimeA = chVTGetSystemTimeX();
    active.A = RISINGEDGE;
  }
  // falling edge reads timer and desides if clicked or long press
  // evaluate only if A was pressed before
  else if(active.A == RISINGEDGE) {
    if(chVTIsSystemTimeWithinX(startTimeA, startTimeA + TIME_MS2I(LONGPRESSEDMS))){
      // short pressed
      active.A = CLICKED;
    }
    else { // long press
      active.A = LONGPRESSED;
    }
  }
  if(palReadLine(LINE_BUTTON_B) == PAL_HIGH){ // rising edge starts timer
    startTimeB = chVTGetSystemTimeX();
    active.B = RISINGEDGE;

  }
  else if(active.B == RISINGEDGE)  { // falling edge reads timer and desides if clicked or long press
    if(chVTIsSystemTimeWithinX(startTimeB, startTimeB + TIME_MS2I(LONGPRESSEDMS))){
      // short pressed
      active.B = CLICKED;
    }
    else { // long press
      active.B = LONGPRESSED;
    }
  }
  if(palReadLine(LINE_BUTTON_L) == PAL_HIGH){ // rising edge starts timer
    startTimeL = chVTGetSystemTimeX();
  }
  else { // falling edge reads timer and desides if clicked or long press
    if(chVTIsSystemTimeWithinX(startTimeL, startTimeL + TIME_MS2I(LONGPRESSEDMS))){
      // short pressed
      active.L = CLICKED;
    }
    else { // long press
      active.L = LONGPRESSED;
    }
  }
  if(palReadLine(LINE_BUTTON_R) == PAL_HIGH){ // rising edge starts timer
    startTimeR = chVTGetSystemTimeX();
  }
  else { // falling edge reads timer and desides if clicked or long press
    if(chVTIsSystemTimeWithinX(startTimeR, startTimeR + TIME_MS2I(LONGPRESSEDMS))){
      // short pressed
      active.R = CLICKED;
    }
    else { // long press
      active.R = LONGPRESSED;
    }
  }
  if(palReadLine(LINE_BUTTON_ROT) == PAL_HIGH){ // rising edge starts timer
    startTimeRot = chVTGetSystemTimeX();
  }
  else { // falling edge reads timer and desides if clicked or long press
    if(chVTIsSystemTimeWithinX(startTimeRot, startTimeRot + TIME_MS2I(LONGPRESSEDMS))){
      // short pressed
      active.ROT = CLICKED;
    }
    else { // long press
      active.ROT = LONGPRESSED;
    }
  }
}

void rotary_cb(void *arg){
  (void)arg;
  if( (palReadLine(LINE_ROTARY_B) == PAL_HIGH) & (palReadLine(LINE_ROTARY_A) == PAL_HIGH)
  //   |(palReadLine(LINE_ROTARY_B) == PAL_LOW ) & (palReadLine(LINE_ROTARY_A) == PAL_LOW )
     ){
    rot_up_down += 1;   // rotated clockwise: increment counter
  }
  else{
    rot_up_down -= 1;   // rotated counter-clockwise: decrement counter
  }
}

/** set up the buttons */
void buttonInit(void){
  palEnableLineEvent(LINE_BUTTON_A,   PAL_EVENT_MODE_BOTH_EDGES);
  palEnableLineEvent(LINE_BUTTON_B,   PAL_EVENT_MODE_BOTH_EDGES);
  palEnableLineEvent(LINE_BUTTON_L,   PAL_EVENT_MODE_BOTH_EDGES);
  palEnableLineEvent(LINE_BUTTON_R,   PAL_EVENT_MODE_BOTH_EDGES);
  palEnableLineEvent(LINE_BUTTON_ROT, PAL_EVENT_MODE_BOTH_EDGES);
  // must listen on ROTARY_B because ROTARY_A is on pin 11 port C and button R is on pin 11 port B
  // STM32 EXTI cannot listen on the same pin for different ports :(
  palEnableLineEvent(LINE_ROTARY_B,   PAL_EVENT_MODE_RISING_EDGE);

  palSetLineCallback(LINE_BUTTON_A,   button_cb, NULL);
  palSetLineCallback(LINE_BUTTON_B,   button_cb, NULL);
  palSetLineCallback(LINE_BUTTON_L,   button_cb, NULL);
  palSetLineCallback(LINE_BUTTON_R,   button_cb, NULL);
  palSetLineCallback(LINE_BUTTON_ROT, button_cb, NULL);
  palSetLineCallback(LINE_ROTARY_B,   rotary_cb, NULL);
}
