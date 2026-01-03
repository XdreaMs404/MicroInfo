// Descriptif general du programme
// Auteur(s), Date

#include <p18C452.h>//inclus la librairie contenant les d?finitions des bits et des registres

#pragma config OSC = XT // oscillatateur ext?rieur
#pragma config WDT = OFF // pas de chien de garde

void traiter_it(void);// declaration programme gestion interruption
void init (void);
void separation(unsigned char a);

// d?clarer vos variables et constantes
unsigned char vitesse=0, unite=0,dizaine=0,centaine=0;

#pragma  code it=0x08  //adresse interruption haute
       
void saut_sur_spit(void)
{_asm
   	goto traiter_it
 _endasm
}


#pragma code
// Sous programmes 

void separation(unsigned char a)
{
	unite=0;
	dizaine=0;
	centaine=0;

	while(a>=100)
	{
		centaine++;
		a=a-100;
	}

	while(a>=10)
	{
		dizaine++;
		a=a-10;
	}

	unite=a;
}

//pour chaque sous programme commenter fonction


//programme d'initialisation
void init(void)
{
	TRISC = 0x00; // PORTC2 : speaker
	
	CCP1CONbits.CCP1M3=1; // réglage du CCP en mode PWM (11xx)
	CCP1CONbits.CCP1M2=1;	


	T2CONbits.T2CKPS1=1;
	T2CONbits.T2CKPS0=1; // prescale du timer2 à 16
	T2CONbits.TMR2ON=1;
	
//valeur de départ du timer
	TMR0H=0x3C;//poid fort
	TMR0L=0xB0;//poid faible
	INTCONbits.GIE=1;//autorise toutes les interruptions
	INTCONbits.TMR0IE=1;//autorise les interruptions timer
	INTCONbits.INT0IE=0;//autorise les interruptions bouton 0
	INTCON3bits.INT1IE=0;//autorise les interruptions bouton 1
	T0CONbits.TMR0ON=1;//autorise les interruptions de depassememnt du timer
	T0CONbits.T08BIT=0;//compte sur 2 octets
	T0CONbits.T0CS=0;//lance le chrono

	T0CONbits.PSA=0;//a 1, pas de prescale pour compter tout les 2 ou 4 ticks
	T0CONbits.T0PS2=0;//prescaler, ici a 4
	T0CONbits.T0PS1=0;
	T0CONbits.T0PS0=1;

	//convertisseur
	ADCON1=0x06;
	ADCON1bits.ADFM=0;//decalage a droite de la conversion
	ADCON0bits.ADCS1=0;//clock pour le convertisseur
	ADCON0bits.ADCS0=1;//ici fosc/8
	ADCON1bits.ADCS2=0;
	ADCON1bits.PCFG3=0;//configuration des ports
	ADCON1bits.PCFG2=0;
	ADCON1bits.PCFG1=0;
	ADCON1bits.PCFG0=0;
	ADCON0bits.ADON=1;//allume le convertisseur
	ADCON0bits.CHS2=1;//choix du channel(AN4)
	ADCON0bits.CHS1=0;
	ADCON0bits.CHS0=0;
	PIR1bits.ADIF=0;//flag du convertisseur a zero
	PIE1bits.ADIE=1;//autorise les interruption du convertisseur
}


//programme qui traite les interruptions
#pragma interrupt traiter_it
void  traiter_it(void)
{
	if(INTCONbits.TMR0IF)//interuption du au timer
	{
		INTCONbits.TMR0IF=0;
		TMR0H=0x3C;//poid fort
		TMR0L=0xB0;//poid faible
		ADCON0bits.GO=1;
	}
	
	if(PIR1bits.ADIF)
	{
		PIR1bits.ADIF=0;
		vitesse=ADRESH;
		separation(vitesse);
	}
}

#pragma code
// programme principal
 void main(void) 
{
 init();
for(;;) //effectue le programme suivant en boucle
	{
		if(vitesse>=130)
		{
			PR2=0x3E;
			CCPR1L =31;
			CCP1CONbits.DC1B1 = 0; //bits du duty cycle (CCPR1L(00))
			CCP1CONbits.DC1B0 = 1;
		}
		else
		{
			PR2=0x00;
			CCPR1L=0;
			CCP1CONbits.DC1B1 = 0; //bits du duty cycle (CCPR1L(00))
			CCP1CONbits.DC1B0 = 0;
		}
	}
}

