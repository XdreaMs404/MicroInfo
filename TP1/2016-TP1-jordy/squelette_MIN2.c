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
int temp=0,tempo=0;

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
	TRISB=0x03;
	TRISC=0;
	TRISD=0;
	INTCON=0xF0;
	T0CON=0x98;
	TMR0H=0xD8;
	TMR0L=0xF0;
}



//programme qui traite les interruptions
#pragma interrupt traiter_it

void  traiter_it(void)
{		
	if(INTCONbits.TMR0IF)
		{
			if(etat==1)
			{temp++;tempo++;}
			if(etat==3)
			{tempo++;}
			INTCONbits.TMR0IF=0;
			TMR0H=0xD8 ; // Voir doc technique Timer0, octet de gauche
			TMR0L=0xF0 ; // Point de départ, octet de droit
		}
	if(INTCONbits.INT0IF)
		{
		if(etat==0)
		{
		etat=1;
		}
		else if(etat==1)
		{
		etat=2;
		}
		else if(etat==2)
		{
		etat=1;
		}
	/*etat=2; // if(etat =2 , etat=1) puis arreter timer ....*/
	INTCONbits.INT0IF=0;
		}
	if(INTCON3bits.INT1IF)
		{
		if(etat==1)
		{
		etat=3;
		}
		else if(etat==2)
		{
		etat=0;
		temp=0;
		}
		else if(etat==3)
		{
		etat=1;
		temp=tempo;
		}
	/*etat=2; // if(etat =2 , etat=1) puis arreter timer ....*/
	INTCON3bits.INT1IF=0;
		}
	
}




#pragma code
// programme principal
 void main(void) 
{
 init();
for(;;) //effectue le programme suivant en boucle
	{	
	decomposition(temp);
if(temp>9998)
	temp=0;

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

