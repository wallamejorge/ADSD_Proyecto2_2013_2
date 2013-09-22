
#include <avr/io.h> //standard include for ATMega
#include <util/delay.h> // standard include for delays
#define sbi(x,y) x |= _BV(y) //set bit of X  in the yth position
#define cbi(x,y) x &= ~(_BV(y)) //clear bit of X  in the yth position
#define tbi(x,y) x ^= _BV(y) //toggle bit of X  in the yth position
#define is_high(x,y) ((x & _BV(y)) == _BV(y)) //check if the input pin is high
#define is_low(x,y) ((x & _BV(y)) == 0) //check if the input pin is low

int main(void)
{
    DDRB=0xff; //PORTB as OUTPUT
    PORTB=0x00;
    DDRD=0x00; //PORTD as INPUT
    PORTD=0xff; //Enable Pull-up on the input port
	int tao=100;
    while(1) //Infinite loop
    {
		if(is_low(PIND,PD0)){
			sbi(PORTB,0);
            _delay_ms(tao);
			cbi(PORTB,0);
            _delay_ms(tao);
			sbi(PORTB,1);
            _delay_ms(tao);
			cbi(PORTB,1);
            _delay_ms(tao);
			sbi(PORTB,2);
            _delay_ms(tao);
			cbi(PORTB,2);
            _delay_ms(tao);
			sbi(PORTB,3);
            _delay_ms(tao);
			cbi(PORTB,3);
            _delay_ms(tao);
		    sbi(PORTB,4);
            _delay_ms(tao);
			cbi(PORTB,4);
            _delay_ms(tao);
		    sbi(PORTB,5);
            _delay_ms(tao);
			cbi(PORTB,5);
            _delay_ms(tao);
			sbi(PORTB,6);
            _delay_ms(tao);
			cbi(PORTB,6);
            _delay_ms(tao);
			sbi(PORTB,7);
            _delay_ms(tao);
			cbi(PORTB,7);
            _delay_ms(tao);}
			else {cbi(PORTB,PB0);cbi(PORTB,PB1);cbi(PORTB,PB2);cbi(PORTB,PB3);cbi(PORTB,PB4);cbi(PORTB,PB5);cbi(PORTB,PB6);cbi(PORTB,PB7);}
	}
    return 0;

}