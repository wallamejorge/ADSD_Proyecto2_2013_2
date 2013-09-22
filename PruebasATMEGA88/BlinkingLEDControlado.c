/* Nombre del Programa :Blinking LED controlado
 * Creador :NanN
 * Fecha :22/sep/2013
 * Microcontrolador:ATMEGA88
 * Descripcion:
   Este programa lee el puerto PORTD.0 y si esta en 0 parpadea el LED ed PORTB.0, si esta 1 LED de PORTB.0 se fija en 1 
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
    PORTD=0xff; //Enable Pull-up on the input port

    while(1) //Infinite loop
    {
        for(i=0;i<2;i++)
        {
            if(i==0)
            {
                sbi(PORTB,PB0);
            }
            else if(i==1)
            {
                if(is_low(PIND,PD0))
                {
                    cbi(PORTB,PB0);
                }
                else
                {
                    sbi(PORTB,PB0);
                }
            }
            _delay_ms(10);
        }
    }
    return 0;
}
