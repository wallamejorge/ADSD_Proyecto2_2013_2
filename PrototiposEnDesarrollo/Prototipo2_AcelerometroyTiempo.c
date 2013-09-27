//----------------------------------------------------------------------------------------------------------------------------------------------//
//Prototipo NO.2 Proyecto 2 Cohete Agua. Codigo que lee 3 señales analogas de 0-5v y calcula los angulos de inclinacion.Tomando tambien los tiempos
// es capaz de predecir la distancia maxima
// Autores: Juan Felipe Martinez, Monica Tuta Fajardo, Jorge Luis Mayorga
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
double initial_anglexz(double ax, double ay, double az); // Calcula el angulo xz inicial de lanzamiento
double initial_anglexy(double ax, double ay, double az); // Calcula el angulo xy inicial de lanzamiento
double initial_angleyz(double ax, double ay, double az); // Calcula el angulo yz inicial de lanzamiento
double final_distance(double angle0xz,double tf); // Calcula la distancia del cohete en mts usando el angulo y el tiempo
int time_counter(int Reset,int count,int count0); // Contador mientras que este activo count
void init_ports(void); //Inicializa los puertos
void select_ADC_port(int i);// Seleccion cual puerto de ADC leer
void print_Angle_Binary(int NoPortCD,double angle);// Imprime en PORTD el angulo medido en binario
//------------------------------------------------------//


//------------------------------------------------------//
//---------------------Main.c---------------------------//
//------------------------------------------------------//
void main(void)
{
	init_ports(); // Inicializa puertos A=>ADC, B=Inputs ,C=Outputs , D=Outputs
	int i=0;
	double angle0xy=0.0;
	double angle0yz=0.0;
	double angle0xz=0.0;
	double x_total=0.0;
	double a[3];
	int time=0.0;
	int Count=0;
	int Reset=0;

	while (1)
	{  for(i=0;i<4;i++){select_ADC_port(i); /*Seleccionar puerto de entrada ADC*/ a[i]=ADCH-81;} // Cargar en el vector a[i] los valores ax ay az

		angle0xy=initial_anglexy(a[0],a[1],a[2]); // Genero angulo entre x y y en grados
		angle0xz=initial_anglexz(a[0],a[1],a[2]); // Genero angulo entre x y z en grados
		angle0yz=initial_angleyz(a[0],a[1],a[2]); // Genero angulo entre y y z en grados
		print_Angle_Binary(1,angle0xz); // Imprime el angulo en binario en el puerto PORTD
		time=time+1;
		_delay_ms(1);
		x_total=final_distance(angle0xz,time); // Calcula la distancia final.

		

	}
}
//------------------------------------------------------//





//------------------------------------------------------//
//-------------------Init_Ports-------------------------//
//------------------------------------------------------//
void init_ports(void){

	DDRA=0x00;		//Defino Puerto A como Inputs
	PORTA=0x00;     //Habilito los puertos de A
	DDRB = 0x00;    //Defino Puerto B como Inputs
	PORTB=0xff;     //Habilito los puertos de B

	DDRC=0xff;		//Defino Puerto C como Outputs
	PORTC=0x00;     //Habilito los puertos de C
	DDRD = 0xff;    //Defino Puerto D como Outputs
	PORTD=0x00;     //Habilito los puertos de D

	ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0); // Habilito preescaler para tener una buena resolucion de datos de ADC

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
//----------------Initial_Angle_XY----------------------//
//------------------------------------------------------//
double initial_anglexy(double ax, double ay, double az){
	return atan(ax/ay)*(57.2958);
}
//------------------------------------------------------//


//------------------------------------------------------//
//----------------Initial_Angle_XZ----------------------//
//------------------------------------------------------//
double initial_anglexz(double ax, double ay, double az){
	return atan(az/ax)*(57.2958);
}
//------------------------------------------------------//


//------------------------------------------------------//
//----------------Initial_Angle_YZ----------------------//
//------------------------------------------------------//
double initial_angleyz(double ax, double ay, double az){
	return atan(ay/az)*(57.2958);
}
//------------------------------------------------------//


//------------------------------------------------------//
//---------------------Time_Counter---------------------//
//------------------------------------------------------//
int time_counter(int Reset,int count,int count0){
	int salida=0;
	if(Reset==0)
	{
		if(count==1){salida=count0+count;}else{salida=count0;}
	}
	else
	{
		salida=0;
	}
	return salida;
}
//------------------------------------------------------//


//------------------------------------------------------//
//----------------------Print Angle---------------------//
//------------------------------------------------------//
void print_Angle_Binary(int NoPortCD,double angle){
	double angle_segment_1=10;
	double angle_segment_2=20;
	double angle_segment_3=30;
	double angle_segment_4=40;
	double angle_segment_5=50;
	double angle_segment_6=60;
	double angle_segment_7=70;
	double angle_segment_8=80;
	double angle_segment_9=90;
	
if(angle>angle_segment_9){     sbi(PORTD,PD3);cbi(PORTD,PD2);sbi(PORTD,PD1);cbi(PORTD,PD0);} //PORD=1010//
else if(angle>angle_segment_8){sbi(PORTD,PD3);cbi(PORTD,PD2);cbi(PORTD,PD1);sbi(PORTD,PD0);} //PORD=1001//
else if(angle>angle_segment_7){sbi(PORTD,PD3);cbi(PORTD,PD2);cbi(PORTD,PD1);cbi(PORTD,PD0);} //PORD=1000//
else if(angle>angle_segment_6){cbi(PORTD,PD3);sbi(PORTD,PD2);sbi(PORTD,PD1);sbi(PORTD,PD0);} //PORD=0111//
else if(angle>angle_segment_5){cbi(PORTD,PD3);sbi(PORTD,PD2);sbi(PORTD,PD1);cbi(PORTD,PD0);} //PORD=0110//
else if(angle>angle_segment_4){cbi(PORTD,PD3);sbi(PORTD,PD2);cbi(PORTD,PD1);sbi(PORTD,PD0);} //PORD=0101//
else if(angle>angle_segment_3){cbi(PORTD,PD3);sbi(PORTD,PD2);cbi(PORTD,PD1);cbi(PORTD,PD0);} //PORD=0100//
else if(angle>angle_segment_2){cbi(PORTD,PD3);cbi(PORTD,PD2);sbi(PORTD,PD1);sbi(PORTD,PD0);} //PORD=0011//
else if(angle>angle_segment_1){cbi(PORTD,PD3);cbi(PORTD,PD2);sbi(PORTD,PD1);cbi(PORTD,PD0);} //PORD=0010//
else if(angle>0){              cbi(PORTD,PD3);cbi(PORTD,PD2);cbi(PORTD,PD1);sbi(PORTD,PD0);} //PORD=0001//
	
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
	return r;
}
//------------------------------------------------------//
