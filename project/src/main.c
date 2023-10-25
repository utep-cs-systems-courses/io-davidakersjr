
   #include <msp430.h>

   #include "libTimer.h"

   #include "led.h"

   #include "buzzer.h"

   #include "switch.h"


   int main(void) {

     P1DIR |= LEDS;
     P1OUT |= LED_GREEN;
     P1OUT |= LED_RED;
     
     

     P2REN |= SWITCHES;/* enables resistors for switches */

     P2IE |= SWITCHES;/* enable interrupts from switches */

     P2OUT |= SWITCHES;/* pull-ups for switches */

     P2DIR &= ~SWITCHES;/* set switches' bits for input */
     

     configureClocks();/* setup master oscillator, CPU & peripheral clocks */

     enableWDTInterrupts();/* enable periodic interrupt */

     
     

     or_sr(0x18);/* CPU off, GIE on */

   }

void buzzer(){
  buzzer_init();
  static int note = 0;

  switch(note){
  case 1: buzzer_set_period(350);note++;break;
  case 2: buzzer_set_period(250);note++;break;
  case 3: buzzer_set_period(430);note++;break;
  case 4: buzzer_set_period(600);note = 0;break;
  default: note++;
  }


}
static int on = 0;
void flicker(){
  if (on){
  P1OUT ^= LED_GREEN;
  P1OUT ^= LED_RED;
  }
}
static int listNotes[] = {400,500,600,700,800,900,1000,1100,1200,1300};

void ultra(){
  buzzer_init();
  
  
  for(int i = 0;i <= 9; i++){
    P1OUT ^= LED_RED;
    for(int j = 0; j <= 10000;j++){
      buzzer_set_period(listNotes[i]);
    }
    
  }
  buzzer_set_period(0);

}


// blink state machine

static int blinkLimit = 5;   //  state var representing reciprocal of duty cycle

void blinkUpdate() // called every 1/250s to blink with duty cycle 1/blinkLimit
{

  static int blinkCount = 0; // state var representing blink state

  blinkCount ++;

  if (blinkCount >= blinkLimit) {

    blinkCount = 0;
   
    // greenControl(1);
    // redControl(1);

  } else {

    // greenControl(0);
    // redControl(0);
  }
}


void oncePerSecond() // repeatedly start bright and gradually lower duty cycle, one step/sec
{
  
  blinkLimit ++;  // reduce duty cycle
  
  if (blinkLimit >= 8){  // but don't let duty cycle go below 1/7.
    blinkLimit = 0;
  }
}



void secondUpdate()
{
  static int secondCount = 0;
  secondCount++;
  if (secondCount >= 250){
    secondCount = 0;
    flicker();
    oncePerSecond();
    
  }
}



void timeAdvStateMachines() // called every 1/250 sec
{

  blinkUpdate();

  secondUpdate();

  

}

void switch_interrupt_handler() {
  char p2val = P2IN;/* switch is in P2 */



  /* update switch interrupt sense to detect changes from current buttons */

  P2IES |= (p2val & SWITCHES);/* if switch up, sense down */

  P2IES &= (p2val | ~SWITCHES);

  
  if(!(p2val & SW1)){
    buzzer();
  }
  else if(!(p2val & SW2)){
    redControl(0);
  }
  else if(!(p2val & SW3)){
    on = 1 - on ;
  }
  else if(!(p2val & SW4)){
    ultra();
  }

}
 



void __interrupt_vec(WDT_VECTOR) WDT()/* 250 interrupts/sec */
{
  
  // handle blinking

  timeAdvStateMachines();

}
void __interrupt_vec(PORT2_VECTOR) Port_2(){

  if (P2IFG & SWITCHES) {      /* did a button cause this interrupt? */

    P2IFG &= ~SWITCHES;      /* clear pending sw interrupts */

    switch_interrupt_handler();/* single handler for all switches */

  }

}

