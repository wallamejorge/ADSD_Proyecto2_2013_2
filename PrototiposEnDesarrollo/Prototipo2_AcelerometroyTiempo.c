//----------------------------------------------------------------------------------------------------------------------------------------------//
//Prototipo NO.2 Proyecto 2 Cohete Agua. Codigo que lee 3 se?ales analogas de 0-5v y calcula los angulos de inclinacion.Tomando tambien lso tiempos
// es capaz de predecir la distancia maxima
// Autores: Juan Felipe Martinez, Monica Tuta Farjado, Jorge Luis Mayorga
// Universidad de los Andes
//----------------------------------------------------------------------------------------------------------------------------------------------//

//-------------------------------------------------//
//-----------------LIBRERIAS-----------------------//
//------------------------------------------------//
#include<avr/io.h>
#include<math.h>
#include<util/delay.h>
#include<stdbool.h>
//------------------------------------------------//



//------------------------------------------------//
//---------------DEFINIR FUNCIONES----------------//
//------------------------------------------------//
#define sbi(x,y) x |= _BV(y) //Hace set al yth bit del puerto X
#define cbi(x,y) x &= ~(_BV(y)) //Hace reset al yth bit del puerto X
#define tbi(x,y) x ^= _BV(y) //Cambia el estado logico del yth bit del puerto X
#define is_high(x,y) ((x & _BV(y)) == _BV(y)) //Verifica si el yth bit del puerto X es 1
#define is_low(x,y) ((x & _BV(y)) == 0) //Verifica si el yth bit del puerto X es 0
double initial_anglexz(int ax, int ay, int az); // Calcula el angulo xz inicial de lanzamiento
double initial_anglexy(int ax, int ay, int az); // Calcula el angulo xy inicial de lanzamiento
double initial_angleyz(int ax, int ay, int az); // Calcula el angulo yz inicial de lanzamiento
double final_distance(double angle0xz,double tf);
void time_counter(int Start,int End,int Reset,int i,int time_tmp);
void init_ports(void);
void select_ADC_port(int i);
void test_ADC(int x0,int x1,int x2);
//------------------------------------------------------//


//------------------------------------------------------//
//---------------------Main.c---------------------------//
//------------------------------------------------------//
void main(void)
{
	init_ports();
	unsigned char compare_value = 180;
	unsigned char x[3];
	int i=0;
	double angle0xy=0.0;
	double angle0yz=0.0;
	double angle0xz=0.0;
	double x_total=0.0;
	int time_tmp=0.0;
	int a[3];
	int j=0;
	int Start=0;
	int Reset=0;
	int End=0;

	while (1)
	{
		for(i=0;i<4;i++)
		{
			select_ADC_port(i); // Seleccionar puerto de entrada ADC
			a[i]=ADCH;
			if(ADCH > compare_value) x[i]=1; // Cargar X[i] para el tes de pines
			else x[i]=0;
		}
		
		angle0xy=initial_anglexy(a[0],a[1],a[2]);
		angle0xz=initial_anglexz(a[0],a[1],a[2]);
		angle0yz=initial_angleyz(a[0],a[1],a[2]);

		x_total=final_distance(angle0xz,time_tmp);
		test_ADC(x[0],x[1],x[2]);
		

	}
}
//------------------------------------------------------//






//------------------------------------------------------//
//-------------------Init_Ports-------------------------//
//------------------------------------------------------//
void init_ports(void){

	DDRA=0x00;	//Defino Puerto A como Inputs
	PORTA=0xff;     //Habilito los puertos de A
	DDRB = 0x00;    //Defino Puerto B como Inputs
	PORTB=0xff;     //Habilito los puertos de B

	DDRC=0xff;	//Defino Puerto C como Inputs
	PORTC=0x00;     //Habilito los puertos de C
	DDRD = 0xff;    //Defino Puerto D como Inputs
	PORTD=0x00;     //Habilito los puertos de D

	ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);

}
//------------------------------------------------------//






//------------------------------------------------------//
//----------------Select_ADC_port-----------------------//
//------------------------------------------------------//
void select_ADC_port(int i){
	ADMUX = _BV(ADLAR) + i; // Selecciona un pin de las entradas analogas ADC[i]
	ADCSRA |= _BV(ADSC); //Comienza conversion
	while (ADCSRA & _BV(ADSC) ) {}  // Esperar la conversion

}
//------------------------------------------------------//





//------------------------------------------------------//
//----------------------Test_ADC------------------------//
//------------------------------------------------------//
void test_ADC(int x0,int x1,int x2){
	int x[3];
	x[0]=x0;
	x[1]=x1;
	x[2]=x2;

	if(x[0] == 1){sbi(PORTC,PC0);}else{cbi(PORTC,PC0);}
	if(x[1] == 1){sbi(PORTC,PC1);}else{cbi(PORTC,PC1);}
	if(x[2] == 1){sbi(PORTC,PC2);}else{cbi(PORTC,PC2);}

}
//------------------------------------------------------//

//------------------------------------------------------//
//----------------Initial_Angle_XY----------------------//
//------------------------------------------------------//
double initial_anglexy(int ax, int ay, int az){
	return atan(ax/ay);
}
//------------------------------------------------------//



//------------------------------------------------------//
//----------------Initial_Angle_XZ----------------------//
//------------------------------------------------------//
double initial_anglexz(int ax, int ay, int az){
	return atan(ax/az);
}
//------------------------------------------------------//




//------------------------------------------------------//
//----------------Initial_Angle_YZ----------------------//
//------------------------------------------------------//
double initial_angleyz(int ax, int ay, int az){
	return atan(ay/az);
}
//------------------------------------------------------//


//------------------------------------------------------//
//----------------Initial_velocityY---------------------//
//------------------------------------------------------//
double initial_velocityY(int ax, int ay, int az){
	return 0;
}
//------------------------------------------------------//



//------------------------------------------------------//
//----------------Initial_velocityX---------------------//
//------------------------------------------------------//
double initial_velocityX(int ax, int ay, int az){
	return 0;
}
//------------------------------------------------------//




//------------------------------------------------------//
//---------------------Time_Counter---------------------//
//------------------------------------------------------//
void time_counter(int Start,int End,int Reset,int i,int time_tmp){
	if(Reset==0){
		if(Start==1){time_tmp=time_tmp+1;}
		else if(End==1){time_tmp=time_tmp;}
	else time_tmp=0;}
	else time_tmp=0;
}
//------------------------------------------------------//



//------------------------------------------------------//
//--------------------Final_Distance--------------------//
//------------------------------------------------------//
double final_distance(double angle0xz,double tf){
	float theta=0.7156;
	float t=2.544;
	float g=9.8;
	float a=-sin(2*theta)/(t*t); 
	float b=g;
	float c=(-1/4)*g*g*t*t*sin(2*theta);
	float r=(1/(2*a))*(-b+sqrt(b*b-4*a*c));
	
	if(c<-154){sbi(PORTD,PD0);}else{cbi(PORTD,PD0);}
	if(c>-152){sbi(PORTD,PD1);}else{cbi(PORTD,PD1);}
	
		
	return r;
}
//------------------------------------------------------//
