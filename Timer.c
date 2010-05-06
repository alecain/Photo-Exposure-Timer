nclude <avr/io.h>
#include <avr/interrupt.h>



const int DBVAL = 15;	//debounce timer
const int CVAL = 450;	//Counter increments

char Led;
volatile int TMR;	//global timer
int counter;

int dbtimer;	//debounce timer
int cTimer;		//counter timer.

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
	return PORTB & (1<<pin);	
}

int main(void) {
	TMR=0;
	Switch=0;
	dbtimer=0;
	
	DDRB |= 1<<PB0; 	// set PB0 to output
	DDRB &= ~(0<<PB1);	//set PB1 to input

	TCCR0A = 0x00; //Normal operation
			TCCR0B = 0x05;//clk (9.6 mHz)/8 (no prescaler)

	OCR0A=130; //match at 126 (1/2 of 38khz period)
	
	//PORTB &= ~(1<<PB0);

	TIMSK0 = 0x1<<2;//enable compA match interrupts
	
	

	sei();
	
	while(1) {
		if( digitalRead(1)){
			if (dbtimer<=TMR-DBVAL){	//if debounce is older than 100 rollovers ago
				counter++;
				dbtimer=TMR;
				cTimer=TMR+CVAL;
			}
		}
 		else {
			dbtimer=TMR; //set the debounce timer to current time.
		
			if (counter>0){
				PORTB |= (1<<PB0); //turn on output
				if (cTimer<=TMR){ //if counter is older than now-1000
					cTimer=TMR+CVAL;
					counter--;
				}
				else {
					//wait
				}
			} 
			else{
				PORTB &= ~(1<<PB0);//shut off output
			}

		}
	}
	  return 0;
}




