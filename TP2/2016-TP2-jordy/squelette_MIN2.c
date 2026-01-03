// Descriptif général du programme
// Auteur(s), Date

#include <p18C452.h>//inclus la librairie contenant les définitions des bits et des registres

#pragma config OSC = XT // oscillatateur extérieur
#pragma config WDT = OFF // pas de chien de garde

void traiter_it(void);// declaration programme gestion interruption
void init (void);
// déclarer vos variables et constantes
char mille=0,centaine=0,dizaine=0,unite=0;
int etat=0;
const char T[10]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
int i;
int valeur,valeur1,valeur2,valeur3;
int volt=0;

#pragma  code it=0x08  //adresse interruption haute
       
void saut_sur_spit(void)
{_asm
   	goto traiter_it
 _endasm
}


#pragma code
// Sous programmes 

void decomposition(int temp)
{

for(mille=0;temp>=1000;mille++)
{	temp-=1000;
	
}
for(centaine=0;temp>=100;centaine++)
{	temp-=100;
	
}
for(dizaine=0;temp>=10;dizaine++)
{	temp-=10;
	
}
for(unite=0;temp>=1;unite++)
{	temp-=1;
	
}

}






//pour chaque sous programme commenter fonction


//programme d'initialisation
void init(void)
{
	volt=ADRESH;
	volt*=19;
	TRISB=0x03;
	TRISC=0;
	TRISD=0;
	INTCON=0xF0;
	T0CON=0x92; 
	TMR0H=0x0B;
	TMR0L=0xDC;
//can // A REGLER !!!!!!
	ADCON0bits.ADCS1=1;
	ADCON0bits.CHS1=0;
	ADCON0bits.CHS0=1;
	ADCON0bits.ADON=1;
	// Voir doc technique 17.
	ADCON1bits.ADFM=1;
	ADCON1bits.ADCS2=0;
	ADCON1bits.PCFG3=1;
	ADCON1bits.PCFG2=0;
	ADCON1bits.PCFG1=0;
	ADCON1bits.PCFG0=1;
}



//programme qui traite les interruptions
#pragma interrupt traiter_it

void  traiter_it(void)
{		
	if(INTCONbits.TMR0IF)
		{
			ADCON0bits.GO=1;
			while(ADCON0bits.GO==1)
			{}
			volt=ADRESH;// Interuption toutes les 0.5sec.
			volt*=19;
			INTCONbits.TMR0IF=0;
			TMR0H=0x0B ; // Voir doc technique Timer0, octet de gauche
			TMR0L=0xDC ; // Point de départ, octet de droit
		}	
	
}




#pragma code
// programme principal
 void main(void) 
{
 init();
for(;;) //effectue le programme suivant en boucle
	{	
	decomposition(volt);

for(i=0;i<10;i++)
	if(mille==i)
		valeur=T[i];
PORTC=valeur;
PORTD=0x70; //bit 3

for(i=0;i<100;i++)
{;}
PORTD=0xFF;
for(i=0;i<100;i++)
{;}

for(i=0;i<10;i++)
	if(centaine==i)
		valeur1=T[i];
PORTC=valeur1;
PORTD=0xB0; //bit 2

for(i=0;i<100;i++)
{;}
PORTD=0xFF;
for(i=0;i<100;i++)
{;}

for(i=0;i<10;i++)
	if(dizaine==i)
		valeur2=T[i];
PORTC=valeur2;
PORTD=0xD0; // bit 1

for(i=0;i<100;i++)
{;}
PORTD=0xFF;
for(i=0;i<100;i++)
{;}

for(i=0;i<10;i++)
	if(unite==i)
		valeur3=T[i];
PORTC=valeur3;
PORTD=0xE0;// bit 0

for(i=0;i<100;i++)
{;}
PORTD=0xFF;
for(i=0;i<100;i++)
{;}
	}
}

