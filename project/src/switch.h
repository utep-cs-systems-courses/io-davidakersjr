#include <msp430.h>

#include "libTimer.h"

#ifndef switch_included

#define switch_included

#define SW1 BIT0               // P1.0

#define SW2 BIT1             // P1.6

#define SW3 BIT2

#define SW4 BIT3

#define SWITCHES (BIT0 | BIT1 | BIT2 | BIT3)


#endif
