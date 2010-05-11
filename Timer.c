#include <avr/io.h>
#include <avr/interrupt.h>



const int DBVAL = 60; //debounce timer
const int CVAL = 2164; //Counter increments

char Led;
volatile int TMR; //global timer
int counter;

int dbtimer; //debounce timer
int cTimer; //counter timer.

char Switch;
char ID[30];

int i;






/**
*timer compare A interrupt handler
*
*/
ISR(TIM0_COMPA_vect){
TMR++;


}


char digitalRead(char pin){
	return PINB & (1<<pin);
}

void digitalWrite(char pin, char value){
	if( value){
		PORTB |= 1<<pin;
	}
	else{
		PORTB &= ~(1<<pin);
	}
}
//
//sets pin to input or output
// pin- pin
// mode- 0:input
//	 1:ouput
//
void pinMode(char pin, char mode){
        if( mode){
                DDRB |= 1<<pin;
        }
        else{
                DDRB &=~(1<<pin);
        }

}

void blinkTimer()
{
	int blinktimer=TMR;
	digitalWrite(2,1);
	while(TMR<blinktimer+1)
	{
	}
	digitalWrite(2,0);
	while(TMR<blinktimer+2)
	{
	}
}


int main(void) {
TMR=0;
Switch=0;
dbtimer=0;

//0001
DDRB = 0x01; // set PB0 to output (Relay)
//pinMode(0,1);
//pinMode(1,0);
pinMode(2,1);

TCCR0A = 0x00; //Normal operation
TCCR0B = 0x03; //clk (9.6 mHz)/64 (no prescaler)
OCR0A=130; //match at 126 (1/2 of 38khz period)


TIMSK0 = 0x1<<2;//enable compA match interrupts



sei();

while(1) {


if( digitalRead(1)){
	digitalWrite(0,0);
	if (dbtimer<=TMR-DBVAL){ //if debounce is older than 100 rollovers ago
	counter++;
	blinkTimer();
	dbtimer	=TMR;
	cTimer=TMR+CVAL;
	}
}
else {
	dbtimer=TMR; //set the debounce timer to current time.

	if (counter>15){
	counter=0;
	}

	if (counter>0){
		//PORTB=0xFF;
		digitalWrite(0,1); //turn on
		if (cTimer<=TMR){ //if counter is older than now-1000
			cTimer=TMR+CVAL;
			counter--;
		}
		else {
			if (counter==1)
			{
				blinkTimer();
			}
		}
	}
	else{
		//PORTB=0x00;
		digitalWrite(0,0); //shut off	
	}
}
}
return 0;
}


