#include<avr/io.h>
#define cte 1.2778
#define seg_initial 170
#define seg_1 seg_initial-1*cte
#define seg_2 seg_initial-2*cte
#define seg_3 seg_initial-3*cte
#define seg_4 seg_initial-4*cte
#define seg_5 seg_initial-5*cte
#define seg_6 seg_initial-6*cte
#define seg_7 seg_initial-7*cte
#define seg_8 seg_initial-8*cte
#define seg_9 seg_initial-9*cte
#define seg_10 seg_initial-10*cte
#define seg_11 seg_initial-11*cte
#define seg_12 seg_initial-12*cte
#define seg_13 seg_initial-13*cte
#define seg_14 seg_initial-14*cte
#define seg_15 seg_initial-15*cte
#define seg_16 seg_initial-16*cte
#define seg_17 seg_initial-17*cte
#define seg_18 seg_initial-18*cte
#define sbi(x,y) x |= _BV(y)
#define cbi(x,y) x &=~_BV(y)

int main(void)
{
	DDRD=0xff; //PORTB as output
	PORTD=0x00;
	DDRA=0x00; //PORTA set as input (since we want to take inputs for the ADC)
	PORTA=0xff; //Enable the pull-ups on PORTA

	ADMUX =_BV(ADLAR); //Enable the Left aligning of the ADC result

	ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0); //Enable the ADC with prescaler=128

	ADCSRA |=_BV(ADSC); //Set the bit to start the conversion
	while(1)
	{
		while(ADCSRA &_BV(ADSC)) {} //Waits for ADC conversion to complete
		
		if(ADCH>=170)
		{
			sbi(PORTD,PD0); 
			sbi(PORTD,PD1);
			sbi(PORTD,PD2); 
			sbi(PORTD,PD3); 
		}
		else if(ADCH>=165)
		{
			cbi(PORTD,PD0); 
			sbi(PORTD,PD1);
			sbi(PORTD,PD2); 
			sbi(PORTD,PD3); 
		}
		else if(ADCH>=160)
		{
			sbi(PORTD,PD0); 
			cbi(PORTD,PD1);
			sbi(PORTD,PD2); 
			sbi(PORTD,PD3); 
		}
		else if(ADCH>=155)
		{
			sbi(PORTD,PD0); 
			sbi(PORTD,PD1);
			cbi(PORTD,PD2); 
			sbi(PORTD,PD3); 
		}
		else if(ADCH>=147)
		{
			sbi(PORTD,PD0); 
			sbi(PORTD,PD1);
			sbi(PORTD,PD2); 
			cbi(PORTD,PD3); 
		}
		else if(ADCH>=140)
		{
			cbi(PORTD,PD0); 
			cbi(PORTD,PD1);
			cbi(PORTD,PD2); 
			cbi(PORTD,PD3); 
		}
		else if(ADCH>=80)
		{
			sbi(PORTD,PD0); 
			cbi(PORTD,PD1);
			cbi(PORTD,PD2); 
			sbi(PORTD,PD3); 
		}
		else if(ADCH>=70)
		{
			cbi(PORTD,PD0); 
			cbi(PORTD,PD1);
			cbi(PORTD,PD2); 
			sbi(PORTD,PD3); 
		}
		else if(ADCH>=60)
		{
			sbi(PORTD,PD0); 
			sbi(PORTD,PD1);
			sbi(PORTD,PD2); 
			cbi(PORTD,PD3); 
		}
		else if(ADCH>=50)
		{
			cbi(PORTD,PD0); 
			sbi(PORTD,PD1);
			sbi(PORTD,PD2); 
			cbi(PORTD,PD3); 
		}
		else if(ADCH>=40)
		{
			sbi(PORTD,PD0); 
			cbi(PORTD,PD1);
			sbi(PORTD,PD2); 
			cbi(PORTD,PD3); 
		}
		else if(ADCH>=30)
		{
			cbi(PORTD,PD0); 
			cbi(PORTD,PD1);
			sbi(PORTD,PD2); 
			cbi(PORTD,PD3); 
		}
		else if(ADCH>=20)
		{
			sbi(PORTD,PD0); 
			sbi(PORTD,PD1);
			cbi(PORTD,PD2); 
			cbi(PORTD,PD3); 
		}
		else if(ADCH>=10)
		{
			cbi(PORTD,PD0); 
			sbi(PORTD,PD1);
			cbi(PORTD,PD2); 
			cbi(PORTD,PD3); 
		}
		else if(ADCH>=9)
		{
			sbi(PORTD,PD0); 
			cbi(PORTD,PD1);
			cbi(PORTD,PD2); 
			cbi(PORTD,PD3); 
		}
		else /* if(ADCH>=8)*/
		{
			cbi(PORTD,PD0); 
			cbi(PORTD,PD1);
			cbi(PORTD,PD2); 
			cbi(PORTD,PD3); 
		}/*
		else if(ADCH>=seg_4)
		{
			cbi(PORTD,PD0);
			cbi(PORTD,PD1);
			cbi(PORTD,PD2);
			cbi(PORTD,PD3);
			sbi(PORTD,PD4);
		}
		else if(ADCH>=seg_3)
		{
			sbi(PORTD,PD0);
			cbi(PORTD,PD1);
			cbi(PORTD,PD2);
			cbi(PORTD,PD3);
			sbi(PORTD,PD4);
		}
		else if(ADCH>=seg_2)
		{
			cbi(PORTD,PD0);
			sbi(PORTD,PD1);
			cbi(PORTD,PD2);
			cbi(PORTD,PD3);
			sbi(PORTD,PD4);
		}
		else if(ADCH>=seg_1)
		{
			sbi(PORTD,PD0);
			sbi(PORTD,PD1);
			cbi(PORTD,PD2);
			cbi(PORTD,PD3);
			sbi(PORTD,PD4);
		}
		else {
			sbi(PORTD,PD0);
			sbi(PORTD,PD1);
			sbi(PORTD,PD2);
			sbi(PORTD,PD3);
			sbi(PORTD,PD4);
		}*/
		ADCSRA |=_BV(ADSC); //Set the bit to start conversion again
	}
	return 0;
}
