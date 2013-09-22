/* Nombre del Programa :Blinking Led Secuencial
 * Creador :Jorge Luis Mayorga
 * Fecha :22/sep/2013
 * Microcontrolador:ATMEGA88
 * Descripcion:t
   Este programa imprime pin a pin un 1 logico y los demas en 0, secuencialmente.
 */

#include <avr/io.h> //standard include for ATMega
#include <util/delay.h> // standard include for delays
#define sbi(x,y) x |= _BV(y) //set bit
#define cbi(x,y) x &= ~(_BV(y)) //clear bit
#define tbi(x,y) x ^= _BV(y) //toggle bit
#define is_high(x,y) ((x & _BV(y)) == _BV(y)) //check if the input pin is high
#define is_low(x,y) ((x & _BV(y)) == 0) //check if the input pin is low

int main(void)
{
    unsigned int i;
    DDRB=0xff; //PORTB as OUTPUT
    PORTB=0x00;
    DDRD=0x00; //PORTD as INPUT
    PORTD=0xff; //Enable Pull-up on the input portD

    while(1) //Infinite loop
    {
			sbi(PORTB,0);
            _delay_ms(500);
			cbi(PORTB,0);
            _delay_ms(500);
			sbi(PORTB,1);
            _delay_ms(500);
			cbi(PORTB,1);
            _delay_ms(500);
			sbi(PORTB,2);
            _delay_ms(500);
			cbi(PORTB,2);
            _delay_ms(500);
			sbi(PORTB,3);
            _delay_ms(500);
			cbi(PORTB,3);
            _delay_ms(500);
			sbi(PORTB,4);
            _delay_ms(500);
			cbi(PORTB,4);
            _delay_ms(500);
			sbi(PORTB,5);
            _delay_ms(500);
			cbi(PORTB,5);
            _delay_ms(500);
			sbi(PORTB,6);
            _delay_ms(500);
			cbi(PORTB,7);
            _delay_ms(500);
			sbi(PORTB,7);

    return 0;
}
}
