// Descriptif général du programme
// Auteur(s), Date

#include <p18C452.h>//inclus la librairie contenant les définitions des bits et des registres

#pragma config OSC = XT // oscillatateur extérieur
#pragma config WDT = OFF // pas de chien de garde
#pragma config LVP = OFF

void traiter_it(void);// declaration programme gestion interruption
void init (void);
void afficher_freq();
void afficher_periode();


// déclarer vos variables et constantes
void decomposition(int);
char read_lcd(void);
char mille=0,centaine=0,dizaine=0,unite=0,dixmille=0;
char mode=1;
unsigned int freq,freqp,compteur=0,periodq,period;

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
for(dixmille=0;temp>=10000;dixmille++)
{
	temp-=10000;
}
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

char read_lcd(void)
{
	unsigned char etat;
	TRISD=0xFF;
	PORTBbits.RB5=0;
	PORTBbits.RB4=1;
	PORTBbits.RB3=1;
	PORTBbits.RB3=1;
	PORTBbits.RB3=1;
	etat=PORTD;
	PORTBbits.RB3=0;
	PORTBbits.RB3=0; 
	PORTBbits.RB3=0;
	PORTBbits.RB4=0;
	return etat;
}

void write_lcd(unsigned char data,int param)
{
	if(param!=2)
		while((read_lcd()&0x80)!= 0); //Et logique..
	if(param==1)
		PORTBbits.RB5=1;
	else
		PORTBbits.RB5=0;
	
	PORTBbits.RB4=0;
	TRISD = 0;	
	PORTD = data;
	PORTBbits.RB3=1;
	PORTBbits.RB3=1;
	PORTBbits.RB3=1;

	PORTBbits.RB3=0; 
	PORTBbits.RB3=0;
	PORTBbits.RB3=0;
	PORTBbits.RB4=0;
	TRISD=0xFF;
}
//pour chaque sous programme commenter fonction


//programme d'initialisation
void init(void)
{
//Durée: 0,1s
//0,1 * 1 000 000 = 100 000
//Tu divises par 2 jusqu'à ce que 100 000 soit < 65 536
//100 000 / 2 = 50 000
//==> Préscale = 1/2
//Ensuite, tu fais 65536 - 50 000 = 15 536
//15 536 en hexa = 3C B0
//
//TMR0H = 3C
//TMR0L = B0
//Prescale = 1/2
//TMR0H = 3C
//TMR0L = B0
//Prescale = 1/2
	TRISB=0b11000111;
	PORTB=0;
	TRISD=0xFF;
	TRISA=0xF0;
	ADCON1=0x07;
	TMR0H=0x3C;
	TMR0L=0xB0;
	T0CON=0b10000000; //presca de 1/2
	INTCONbits.INT0E=1;
	INTCONbits.TMR0IE=1;
	
	write_lcd(0x38,2);
	write_lcd(0x38,0);
	write_lcd(0x0E,0);
	write_lcd(0x01,0);
}




//programme qui traite les interruptions
#pragma interrupt traiter_it

void  traiter_it(void)
{		
	if(INTCONbits.INT0IF)
		{
			if(mode==1)
			{
			compteur++;
			}
			else
			{
			period=compteur;
			compteur=0;
			}
			INTCONbits.INT0IF=0;
		}
	if (INTCONbits.TMR0IF)
		{
		if(mode==1)
		{
		TMR0H=0x0B;
		TMR0L=0xDC;
		freq=compteur;
		compteur=0;
		
		}
		else
		{
			TMR0H=0xFC;
			TMR0L=0x18;
			compteur++;
		}
		INTCONbits.TMR0IF=0;
		}
	if(INTCON3bits.INT1IF)
		{
		if(mode==1)
		{
		mode=0;
		TMR0H=0xFC;
		TMR0L=0x18;
		T0CON=0b10001000;
		}
		else
		{
		mode=1;
		TMR0H=0x0B;
		TMR0L=0xDC;
		T0CON=0b10000011;
		}
		INTCON3bits.INT1IF=0;
		}
}

void afficher_freq()
{
	write_lcd(0x01,0);
	decomposition(freq);
	dixmille+=0x30;
	mille+=0x30;
	centaine+=0x30;
	dizaine+=0x30;
	unite+=0x30;
	write_lcd(dixmille,1);
	write_lcd(mille,1);
	write_lcd(centaine,1);
	write_lcd(dizaine,1);
	write_lcd(unite,1);
	write_lcd(' ',1);
	write_lcd('H',1);
	write_lcd('z',1);
	
}

void afficher_periode()
{
	write_lcd(0x01,0);
	decomposition(period);
	dixmille+=0x30;
	mille+=0x30;
	centaine+=0x30;
	dizaine+=0x30;
	unite+=0x30;
	write_lcd(dixmille,1);
	write_lcd(mille,1);
	write_lcd(centaine,1);
	write_lcd(dizaine,1);
	write_lcd(unite,1);
	
	write_lcd(' ',1);
	write_lcd('m',1);
	write_lcd('s',1);

	
}


#pragma code
// programme principal
 void main(void) 
{
init();
INTCONbits.GIE=1;
for(;;)
{
		if(mode==1)
		{
			if (freq!=freqp)
			{
			afficher_freq();
			freqp=freq;
			}
		}
		else
		{
			if(periodq!=period)
			{
			afficher_periode();
			periodq=period;
			}
		}

} //effectue le programme suivant en boucle

}