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
double final_distance(double angle0xz,double tf); // Calcula la distancia del cohete en mts usando el angulo y el tiempo
int time_counter(int Reset,int count,int count0); // Contador mientras que este activo count
void init_ports(void); //Inicializa los puertos
void select_ADC_port(int i);// Seleccion cual puerto de ADC leer
void print_Angle_Binary(int NoPortCD,double angle);// Imprime en PORTD el angulo medido en binario
void print_Decimes(float num_in);// Imprime en PORTD el angulo medido en binario
void print_Time_Binary(double time);// Imprime en PORTC el tiempo medido en binario

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
	int count=0;
	int aterrizo=0;
	double xmax=0.0;

	while (1)
	{ 
	sbi(PORTB,PB3);
	 for(i=0;i<6;i++){select_ADC_port(i); a[i]=ADCH-81;} // Cargar en el vector a[i] los valores ax ay az
		a[3]=a[3]+81;
		a[4]=a[4]+81;
	
		// Conversión de entrada análoga
		
		if(a[3]>132){count=1;}
		else{count=0;}

		if(a[4]>132){aterrizo=1;}
		else{aterrizo=0;}


	if(count==0 && aterrizo==0 ){

			angle0xz=initial_anglexz(a[0],a[1],a[2]); 
			time=time;
			print_Time_Binary(time);
			print_Decimes(time*10);
			print_Angle_Binary(1,angle0xz);}

	else if(count==0 && aterrizo==1 ){

			print_Time_Binary(time);
			print_Decimes(time*10);
			print_Angle_Binary(1,angle0xz);}	
	
	else if(count==1 && aterrizo==0 ){	
			angle0xz=angle0xz+0.0;
			time=time+0.1;
			_delay_ms(100);		

			print_Time_Binary(time);
			print_Decimes(time*10);
			print_Angle_Binary(1,angle0xz);}

	else if(count==1 && aterrizo==1 ){
			angle0xz=angle0xz+0.0;
			time=time+0.0;
			xmax=final_distance(angle0xz,time);
			print_Angle_Binary(1,angle0xz);
			print_Decimes(xmax);
}
	else{angle0xz=angle0xz;time=time;}		
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

//------------------------------------------------------//
//----------------Initial_Angle_XZ----------------------//
//------------------------------------------------------//
double initial_anglexz(double ax, double ay, double az){
	return atan(az/ax)*(57.2958);
}
//------------------------------------------------------//

//------------------------------------------------------//
//----------------------Print Angle---------------------//
//------------------------------------------------------//
void print_Angle_Binary(int NoPortCD,double angle){

	double angle_segment_0=5;
	double angle_segment_1=10;
	double angle_segment_2=15;
	double angle_segment_3=20;
	double angle_segment_4=25;
	double angle_segment_5=30;
	double angle_segment_6=35;
	double angle_segment_7=40;
	double angle_segment_8=45;
	double angle_segment_9=50;
	double angle_segment_10=55;
	double angle_segment_11=60;
	double angle_segment_12=65;
	double angle_segment_13=70;
	double angle_segment_14=75;
	double angle_segment_15=80;
	double angle_segment_16=85;
	double angle_segment_17=90;


	if(angle>angle_segment_17){PORTD=0x05;}        //0000//-----//0000// = 00
	else if(angle>angle_segment_16){PORTD=0x10;}   //0000//-----//0101// = 05
	else if(angle>angle_segment_15){PORTD=0x15;}   //0001//-----//0000// = 10  
	else if(angle>angle_segment_14){PORTD=0x20;}   //0001//-----//0101// = 15 
	else if(angle>angle_segment_13){PORTD=0x25;}   //0010//-----//0000// = 10  
	else if(angle>angle_segment_12){PORTD=0x30;}   //0010//-----//0101// = 25  
	else if(angle>angle_segment_11){PORTD=0x35;}   //0011//-----//0000// = 30  
	else if(angle>angle_segment_10){PORTD=0x40;}   //0011//-----//0101// = 35 
	else if(angle>angle_segment_9){PORTD=0x45;}    //0100//-----//0000// = 40  
	else if(angle>angle_segment_8){PORTD=0x50;}    //0100//-----//0101// = 45  
	else if(angle>angle_segment_7){PORTD=0x55;}    //0101//-----//0000// = 50   
	else if(angle>angle_segment_6){PORTD=0x60;}    //0101//-----//0101// = 55  
	else if(angle>angle_segment_5){PORTD=0x65;}    //0110//-----//0000// = 60   
	else if(angle>angle_segment_4){PORTD=0x70;}    //0110//-----//0101// = 65 
	else if(angle>angle_segment_3){PORTD=0x75;}    //0111//-----//0000// = 70 
	else if(angle>angle_segment_2){PORTD=0x80;}    //0111//-----//0101// = 75
	else if(angle>angle_segment_1){PORTD=0x85;}    //1000//-----//0000// = 80 
	else if(angle>angle_segment_0){PORTD=0x90;}    //1000//-----//0101// = 85  
	else {PORTD=0xAA;}  
}
//------------------------------------------------------//

//-------------------Print Distance---------------------//

void print_Decimes(float num_in){

double num0=0.0;

double num=0.0;

	num=num_in;

char decimas=0x00;

	if(num>=90){decimas=0x90;num0=num-90;}
	else if(num>=80){decimas=0x80;num0=num-80;}
	else if(num>=70){decimas=0x70;num0=num-70;}
	else if(num>=60){decimas=0x60;num0=num-60;}
	else if(num>=50){decimas=0x50;num0=num-50;}
	else if(num>=40){decimas=0x40;num0=num-40;}
	else if(num>=30){decimas=0x30;num0=num-30;}
	else if(num>=20){decimas=0x20;num0=num-20;}
	else if(num>=10){decimas=0x10;num0=num-10;}
	else {decimas=0x00; num0=num;}


	if(num0>=9){PORTC=decimas+0x09;}
	else if(num0>=8){PORTC=decimas+0x08;}
	else if(num0>=7){PORTC=decimas+0x07;}
	else if(num0>=6){PORTC=decimas+0x06;}
	else if(num0>=5){PORTC=decimas+0x05;}
	else if(num0>=4){PORTC=decimas+0x04;}
	else if(num0>=3){PORTC=decimas+0x03;}
	else if(num0>=2){PORTC=decimas+0x02;}
	else if(num0>=1){PORTC=decimas+0x01;}
	else if(num0>=0){PORTC=decimas+0x00;}
	else {PORTC=0xAA;}

}

//------------------------------------------------------//
//----------------------Print time---------------------//
//------------------------------------------------------//

void print_Time_Binary(double time){
int time_segment_1=1;
int time_segment_2=2;
int time_segment_3=3;
int time_segment_4=4;
int time_segment_5=5;
int time_segment_6=6;
int time_segment_7=7;
int times=time;

if(times>=time_segment_7){	sbi(PORTB,PB2);sbi(PORTB,PB1);sbi(PORTB,PB0);} //PORC=111//
else if(times>=time_segment_6){  sbi(PORTB,PB2);sbi(PORTB,PB1);cbi(PORTB,PB0);} //PORD=110//
else if(times>=time_segment_5){  sbi(PORTB,PB2);cbi(PORTB,PB1);sbi(PORTB,PB0);} //PORD=101//
else if(times>=time_segment_4){  sbi(PORTB,PB2);cbi(PORTB,PB1);cbi(PORTB,PB0);} //PORD=100//
else if(times>=time_segment_3){  cbi(PORTB,PB2);sbi(PORTB,PB1);sbi(PORTB,PB0);} //PORD=011//
else if(times>=time_segment_2){  cbi(PORTB,PB2);sbi(PORTB,PB1);cbi(PORTB,PB0);} //PORD=010//
else if(times>=time_segment_1){  cbi(PORTB,PB2);cbi(PORTB,PB1);sbi(PORTB,PB0);} //PORD=001//
else {			        cbi(PORTB,PB2);cbi(PORTB,PB1);cbi(PORTB,PB0);} //PORD=000//


}
//------------------------------------------------------//

//------------------------------------------------------//
//--------------------Final_Distance--------------------//
//------------------------------------------------------//

double final_distance(double angle0xz,double tf){
	float theta=(90-angle0xz)*0.017453;
	float t=tf;
	float t2=t*t;
	float g=9.8;
	float a=-sin(2.0*theta)/(t2); 
	float b=g;
	float g2=g*g;
	float c=(0.25)*(g2)*(t2)*(sin(2.0*theta));
	float r=(1.0/(2.0*a))*(-b+sqrt(b*b+4.0*a*c))/10;

	return r;
}

//------------------------------------------------------//





