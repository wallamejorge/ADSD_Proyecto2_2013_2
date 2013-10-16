//----------------------------------------------------------------------------------------------------------------------------------------------//
//Prototipo NO.2 Proyecto 2 Cohete Agua. Codigo que lee 3 señales analogas de 0-5v y calcula los angulos de inclinacion.Tomando tambien los tiempos
// es capaz de predecir la distancia maxima
// Autores: Juan Felipe Martinez, Monica Tuta Fajardo, Jorge Luis Mayorga
// Universidad de los Andes
//----------------------------------------------------------------------------------------------------------------------------------------------//

//-------------------------------------------------//
//-----------------LIBRERIAS-----------------------//
//------------------------------------------------//
//#include<mega16.h>
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
#define is_high(x,y) ((x & _BV(y)) == _BV(y))
#define is_low(x,y) ((x & (_BV(y))) == 0) //check if the input pin is low

//--------------------------------------------------------//
double initial_anglexz(double ax, double ay, double az); // Calcula el angulo xz inicial de lanzamiento
void init_ports(void); //Inicializa los puertos
void select_ADC_port(int i);// Seleccion cual puerto de ADC leer
void print_Number(double num_in);// Imprime en PORTD el angulo medido en binario
double calculate_angle(double ax, double az); // calcula en ángulo
//------------------------------------------------------//


//------------------------------------------------------//
//---------------------Main.c---------------------------//
//------------------------------------------------------//
void main(void)
{

	init_ports(); // Inicializa puertos A=>ADC, B=Inputs ,C=Outputs , D=Outputs
	
	double time=0.0;
	double ms_time=0.0;
	
	int i=0;
	double angle0xz=0.0;

	double a[5];
	double despegue;
	int aterrizo=0;
	double xmax=0.0;
	double angle=0.0;

	while (1)
	{ 
	sbi(PORTB,PB3);
	 for(i=0;i<6;i++){select_ADC_port(i); a[i]=ADCH;} // Cargar en el vector a[i] los valores ax ay az
		a[3]=a[3]+81;
		a[4]=a[4]+81;
	
		// Conversión de entrada análoga
		
		if(a[3]>132){despegue=1;}
		else{despegue=0;}

		if(a[4]>132){aterrizo=1;}
		else{aterrizo=0;}

		// Código de funcionamiento:

		if(despegue==1){
			angle=calculate_angle(a[0],a[2]);
			print_Number(angle);}
		else{angle=angle;
			print_Number(angle);}
		
		
}			
}

//------------------------------------------------------//
//------------------------------------------------------//
//-------------------Init_Ports-------------------------//
//------------------------------------------------------//
void init_ports(void){

	MCUCSR|=(1<<JTD);
	MCUCSR|=(1<<JTD);
	DDRA=0x00;	//Defino Puerto A como Inputs
	PORTA=0x00;     //Habilito los puertos de A
	DDRB = 0x1f;    //Defino Puerto B como Inputs
	PORTB=0x00;     //Habilito los puertos de B

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
//----------------Initial_Angle_XZ----------------------//
//------------------------------------------------------//
double initial_anglexz(double ax, double ay, double az){
	return atan(az/ax)*(57.2958);
}
//------------------------------------------------------//
//---------------PRINT NUMBER---------------------------//
//------------------------------------------------------//

void print_Number(double num_in){

double num2=0.0;
double num1=0.0;
double num0=0.0;
double num=0.0;

	num=num_in;

char decimas=0x00;
char centesimas=0x00;
char unidades=0x00;
char decimales=0x00;

char numero0=0xFC; //1111 1100
char numero1=0x60; //0110 0000
char numero2=0xDA; //1101 1010
char numero3=0xF2; //1111 0010
char numero4=0x66; //0110 0110
char numero5=0xB6; //1011 0110
char numero6=0xBE; //1011 1110
char numero7=0xE0; //1110 0000
char numero8=0xFE; //1111 1110
char numero9=0xF6; //1111 0110

// PD4 -> primer display //11101111
// PD5 -> segundo display //11011111
// PD6 -> tercer display //10111111

char Disp1=0xEF;
char Disp2=0xDF;
char Disp3=0xBF;

if(num>100){

	if(num>=900){centesimas=numero9; num2=num-900;}
	else if(num>=800){centesimas=numero8; num2=num-800;}
	else if(num>=700){centesimas=numero7; num2=num-700;}
	else if(num>=600){centesimas=numero6; num2=num-600;}
	else if(num>=500){centesimas=numero5; num2=num-500;}
	else if(num>=400){centesimas=numero4; num2=num-400;}
	else if(num>=300){centesimas=numero3; num2=num-300;}
	else if(num>=200){centesimas=numero2; num2=num-200;}
	else if(num>=100){centesimas=numero1; num2=num-100;}
	else {decimas=numero0; num2=num;}

	PORTC=centesimas;
	PORTD=Disp1;
	_delay_ms(5);

	if(num2>=90){decimas=numero9; num1=num2-90;}
	else if(num2>=80){decimas=numero8; num1=num2-80;}
	else if(num2>=70){decimas=numero7; num1=num2-70;}
	else if(num2>=60){decimas=numero6; num1=num2-60;}
	else if(num2>=50){decimas=numero5; num1=num2-50;}
	else if(num2>=40){decimas=numero4; num1=num2-40;}
	else if(num2>=30){decimas=numero3; num1=num2-30;}
	else if(num2>=20){decimas=numero2; num1=num2-20;}
	else if(num2>=10){decimas=numero1; num1=num-10;}
	else {decimas=numero0; num1=num;}
	
	PORTC=decimas;
	PORTD=Disp2;
	_delay_ms(5);

	if(num1>=9){unidades=numero9+0x01;num0=num1-9;}
	else if(num1>=8){unidades=numero8+0x01;num0=num1-8;}
	else if(num1>=7){unidades=numero7+0x01;num0=num1-7;}
	else if(num1>=6){unidades=numero6+0x01;num0=num1-6;}
	else if(num1>=5){unidades=numero5+0x01;num0=num1-5;}
	else if(num1>=4){unidades=numero4+0x01;num0=num1-4;}
	else if(num1>=3){unidades=numero3+0x01;num0=num1-3;}
	else if(num1>=2){unidades=numero2+0x01;num0=num1-2;}
	else if(num1>=1){unidades=numero1+0x01;num0=num1-1;}
	else {unidades=numero0;num0=num;}

	PORTC=unidades;
	PORTD=Disp3;}

else{

    if(num>=90){decimas=numero9; num1=num-90;}
	else if(num>=80){decimas=numero8; num1=num-80;}
	else if(num>=70){decimas=numero7; num1=num-70;}
	else if(num>=60){decimas=numero6; num1=num-60;}
	else if(num>=50){decimas=numero5; num1=num-50;}
	else if(num>=40){decimas=numero4; num1=num-40;}
	else if(num>=30){decimas=numero3; num1=num-30;}
	else if(num>=20){decimas=numero2; num1=num-20;}
	else if(num>=10){decimas=numero1; num1=num-10;}
	else {decimas=numero0; num1=num;}
	
	PORTC=decimas;
	PORTD=Disp1;
	_delay_ms(5);

	if(num1>=9){unidades=numero9+0x01;num0=num1-9;}
	else if(num1>=8){unidades=numero8+0x01;num0=num1-8;}
	else if(num1>=7){unidades=numero7+0x01;num0=num1-7;}
	else if(num1>=6){unidades=numero6+0x01;num0=num1-6;}
	else if(num1>=5){unidades=numero5+0x01;num0=num1-5;}
	else if(num1>=4){unidades=numero4+0x01;num0=num1-4;}
	else if(num1>=3){unidades=numero3+0x01;num0=num1-3;}
	else if(num1>=2){unidades=numero2+0x01;num0=num1-2;}
	else if(num1>=1){unidades=numero1+0x01;num0=num1-1;}
	else {unidades=numero0;num0=num;}

	PORTC=unidades;
	PORTD=Disp2;
	_delay_ms(5);
	
	if(num0>=0.9){decimales=numero9;}
	else if(num0>=0.8){decimales=numero8;}
	else if(num0>=0.7){decimales=numero7;}
	else if(num0>=0.6){decimales=numero6;}
	else if(num0>=0.5){decimales=numero5;}
	else if(num0>=0.4){decimales=numero4;}
	else if(num0>=0.3){decimales=numero3;}
	else if(num0>=0.2){decimales=numero2;}
	else if(num0>=0.1){decimales=numero1;}
	else if(num0>=0.0){decimales=numero0;}
	else {decimales=0x6E;}

	PORTC=decimales;
	PORTD=Disp3;
	_delay_us(100);

}
}

//---------------------------------------------------------------------------------//
//--------------------------Calcular Angulo----------------------------------------//
//---------------------------------------------------------------------------------//

double calculate_angle(double ax, double az){

//regz=-8,1926*z+755,19
//regX=0,6586*x^2-106,73*x+4325

//SUMA=regX+regZ;

//ANGULO=0,00003*SUMA^3-0,0073*SUMA^2+1,0438*SUMA-8,4633
//ERROR=(0,001*SUMA*xSUMA)-0,067*SUMA+0,9513

double angulo=0.0;
double error=0.0;
double func=0.0;
double regz=0.0;
double regx=0.0;
double suma=0.0;
double error_f=0.0;

//regz
regz=-8.1926*az+755.19;

//regx
regx=0.6586*(ax*ax)-106.73*(ax)+4325;

//Suma
suma=regx+regz;

//Angulo
angulo=0.00003*(suma*suma*suma)-0.0073*(suma*suma)+1.0438*(suma)-8.4633;

//Error
error_f=0.001*(suma*suma)-0.067*(suma)+0.9513;

func=angulo-error_f;

return func;
}










