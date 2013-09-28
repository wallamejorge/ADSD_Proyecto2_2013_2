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
double initial_anglexz(double ax, double ay, double az); // Calcula el angulo xz inicial de lanzamiento
double initial_anglexy(double ax, double ay, double az); // Calcula el angulo xy inicial de lanzamiento
double initial_angleyz(double ax, double ay, double az); // Calcula el angulo yz inicial de lanzamiento
double final_distance(double angle0xz,double tf); // Calcula la distancia del cohete en mts usando el angulo y el tiempo
int time_counter(int Reset,int count,int count0); // Contador mientras que este activo count
void init_ports(void); //Inicializa los puertos
void select_ADC_port(int i);// Seleccion cual puerto de ADC leer
void print_Angle_Binary(int NoPortCD,double angle);// Imprime en PORTD el angulo medido en binario
void print_Time_Binary(int time);// Imprime en PORTC el tiempo medido en binario
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
	double a[5];
	int time=0;
	int rst;
	double ms_time=0.0;
	int count=0;
	int rstanalogo=0;

	while (1)
	{  for(i=0;i<6;i++){select_ADC_port(i); /*Seleccionar puerto de entrada ADC*/ a[i]=ADCH-81;} // Cargar en el vector a[i] los valores ax ay az
	a[3]=a[3]+81;
	a[4]=a[4]+81;
				
		//angle0xy=initial_anglexy(a[0],a[1],a[2]); // Genero angulo entre x y y en grados
		//angle0xz=initial_anglexz(a[0],a[1],a[2]); // Genero angulo entre x y z en grados
		//angle0yz=initial_angleyz(a[0],a[1],a[2]); // Genero angulo entre y y z en grados
		//print_Angle_Binary(1,angle0xz); // Imprime el angulo en binario en el puerto PORTD
		
		// Conversión de entrada análoga

		
		if(a[3]>132){count=1;}
		else{count=0;}

		if(a[4]>132){rstanalogo=1;}
		else{rstanalogo=0;}

		// Si reset (análogo) todas la señales a 0, sino los contadores de milisegundos y segundos funcionan normal	
		
			
			if (count==0)	{
				if(ms_time==10){time=time+1;ms_time=0;}else{time=time+0;}
				_delay_ms(100);
				ms_time=ms_time+1;	
				print_Time_Binary(time);
				
				angle0xy=angle0xy; angle0xz=angle0xz; angle0yz=angle0yz; //los ángulos se mantienen con el último dato
				print_Angle_Binary(1,angle0xz); // Imprime el angulo en binario en el puerto PORTD 
				}
				

			else { ms_time=ms_time;time=time;
				angle0xy=initial_anglexy(a[0],a[1],a[2]); // Genero angulo entre x y y en grados
				angle0xz=initial_anglexz(a[0],a[1],a[2]); // Genero angulo entre x y z en grados
				angle0yz=initial_angleyz(a[0],a[1],a[2]); // Genero angulo entre y y z en grados
				print_Angle_Binary(1,angle0xz); // Imprime el angulo en binario en el puerto PORTD  
			     }
			
		

		x_total=final_distance(angle0xz,(time+(ms_time/10))); // Calcula la distancia final.

		

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
	
if(angle>angle_segment_9){     cbi(PORTD,PD3);cbi(PORTD,PD2);cbi(PORTD,PD1);sbi(PORTD,PD0);} //PORD=0001//
else if(angle>angle_segment_8){cbi(PORTD,PD3);cbi(PORTD,PD2);sbi(PORTD,PD1);cbi(PORTD,PD0);} //PORD=0010//
else if(angle>angle_segment_7){cbi(PORTD,PD3);cbi(PORTD,PD2);sbi(PORTD,PD1);sbi(PORTD,PD0);} //PORD=0011//
else if(angle>angle_segment_6){cbi(PORTD,PD3);sbi(PORTD,PD2);cbi(PORTD,PD1);cbi(PORTD,PD0);} //PORD=0100//
else if(angle>angle_segment_5){cbi(PORTD,PD3);sbi(PORTD,PD2);cbi(PORTD,PD1);sbi(PORTD,PD0);} //PORD=0101//
else if(angle>angle_segment_4){cbi(PORTD,PD3);sbi(PORTD,PD2);sbi(PORTD,PD1);cbi(PORTD,PD0);} //PORD=0110//
else if(angle>angle_segment_3){cbi(PORTD,PD3);sbi(PORTD,PD2);sbi(PORTD,PD1);sbi(PORTD,PD0);} //PORD=0111// 
else if(angle>angle_segment_2){sbi(PORTD,PD3);cbi(PORTD,PD2);cbi(PORTD,PD1);cbi(PORTD,PD0);} //PORD=1000// 
else if(angle>angle_segment_1){sbi(PORTD,PD3);cbi(PORTD,PD2);cbi(PORTD,PD1);sbi(PORTD,PD0);} //PORD=1001// 
else if(angle>0){              sbi(PORTD,PD3);cbi(PORTD,PD2);sbi(PORTD,PD1);cbi(PORTD,PD0);} //PORD=1010//
		
}
//------------------------------------------------------//



//------------------------------------------------------//
//----------------------Print Angle---------------------//
//------------------------------------------------------//
void print_Time_Binary(int time){
int time_segment_1=1;
int time_segment_2=2;
int time_segment_3=3;
int time_segment_4=4;
int time_segment_5=5;
int time_segment_6=6;
int time_segment_7=7;
int times=time;
if(times>time_segment_7){	sbi(PORTC,PC2);sbi(PORTC,PC1);sbi(PORTC,PC0);} //PORC=111//
else if(times>time_segment_6){  sbi(PORTC,PC2);sbi(PORTC,PC1);cbi(PORTC,PC0);} //PORD=110//
else if(times>time_segment_5){  sbi(PORTC,PC2);cbi(PORTC,PC1);sbi(PORTC,PC0);} //PORD=101//
else if(times>time_segment_4){  sbi(PORTC,PC2);cbi(PORTC,PC1);cbi(PORTC,PC0);} //PORD=100//
else if(times>time_segment_3){  cbi(PORTC,PC2);sbi(PORTC,PC1);sbi(PORTC,PC0);} //PORD=011//
else if(times>time_segment_2){  cbi(PORTC,PC2);sbi(PORTC,PC1);cbi(PORTC,PC0);} //PORD=010//
else if(times>time_segment_1){  cbi(PORTC,PC2);cbi(PORTC,PC1);sbi(PORTC,PC0);} //PORD=001//
else {			        cbi(PORTC,PC2);cbi(PORTC,PC1);cbi(PORTC,PC0);} //PORD=000//


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




