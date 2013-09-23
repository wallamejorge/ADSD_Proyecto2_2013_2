//----------------------------------------------------------------------------------------------------------------------------------------------//
//Prototipo NO.1 Proyecto 2 Cohete Agua. Codigo que lee 3 señales analogas de 0-5v y calcula los angulos de inclinacion y con las aceleracion
//determina velocidades para predecir la trayectoria del cohete.
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
double initial_velocityY(int ax, int ay, int az); // Calcula la velocidad inicial de lanzamiento en y
double initial_velocityX(int ax, int ay, int az); // Calcula la velocidad inicial de lanzamiento en x
double final_distance(int angle0xz,int vy0,int v0x,int t);
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
    unsigned char compare_value = 230;
    unsigned char x[3]; 
    int i=0;
	double g=9.86;
	double angle0xy=0.0;
	double angle0yz=0.0;
	double angle0xz=0.0;
	double vx0=0.0;
	double vy0=0.0;
	double x_total=0.0;
	int time_tmp=0.0;
	int a[3];
	int tao_ms=1;
	int j=0;
	int Start=0;
	int Reset=0;
	int End=0;
	int a_data[1000*tao_ms*60*100];
	
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
		vy0=initial_velocityY(a[0],a[1],a[2]);
		vx0=initial_velocityX(a[0],a[1],a[2]);
		
		j=j+1;
		
		if(is_high(PINB,PB0)){Start=1;}else{Start=0;}
		if(is_high(PINB,PB1)){End=1;}else{End=0;}
		if(is_high(PINB,PB2)){Reset=1;}else{Reset=0;}
			
	    time_counter(Start,End,Reset,j,time_tmp);
	
		x_total=final_distance(angle0xz,vy0,vx0,j);
		
		// Pines de prueba, enciende PC0,PC1,PC2 si voltaje en PA0,PA1,PA2 esta por encima del limite de comparacion //
		test_ADC(x[0],x[1],x[2]);
		
		//Dermina si angulo entre xz es mas o menos 0.75 rad.
		if(angle0xz<(0.75) && angle0xz>(0.85)){sbi(PORTC,PC3);}else{cbi(PORTC,PC3);}

    }
}
//------------------------------------------------------//
	
	
	


	
//------------------------------------------------------//
//-------------------Init_Ports-------------------------//
//------------------------------------------------------//
void init_ports(void){
	
DDRA=0x00;		//Defino Puerto B como Inputs
PORTA=0xff;     //Habilito los puertos de A
DDRB = 0x00;    //Defino Puerto B como Inputs
PORTB=0xff;     //Habilito los puertos de B
	
DDRC=0xff;		//Defino Puerto C como Inputs
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
double final_distance(angle0xz,vy0,vx0,t){
	int g=9.8;
	return (((sqrt(vy0*vy0+vx0*vx0))*sin(angle0xz))/(g));
}
//------------------------------------------------------//






