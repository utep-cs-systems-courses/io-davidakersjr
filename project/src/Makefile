# makfile configuration

COMMON_OBJECTS  = main.o

CPU             = msp430g2553

CFLAGS          = -mmcu=${CPU} -I../h

LDFLAGS		= -L/opt/ti/msp430_gcc/include



#switch the compiler (for the internal make rules)

CC              = msp430-elf-gcc

AS              = msp430-elf-as



all: led-s.elf



#additional rules for files



led-s.elf: ${COMMON_OBJECTS} greenControl.o redControl.o buzzer.o

	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ ../lib/libTimer.a





load: led-s.elf

	msp430loader.sh $^



clean:

	rm -f *.o *.elf



main.o: led.h

greenControl.o: led.h

redControl.o:led.h

wdInterruptHandler.o:  led.h

buzzer.o: buzzer.h


