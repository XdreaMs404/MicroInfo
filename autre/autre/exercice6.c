// Descriptif general du programme
// Auteur(s), Date

#include <p18C452.h>//inclus la librairie contenant les d?finitions des bits et des registres

#pragma config OSC = XT // oscillatateur ext?rieur
#pragma config WDT = OFF // pas de chien de garde

void traiter_it(void);// declaration programme gestion interruption
void init (void);
void separation(unsigned char a);
void affiche(unsigned char c,unsigned char d,unsigned char u);
void ecrire_LCD(char donnee, char param);
char lire_LCD();
void busy();

// d?clarer vos variables et constantes
unsigned char vitesse=0, unite=0,dizaine=0,centaine=0;
int i;

//afficheur
#define E PORTBbits.RB3
#define RW PORTBbits.RB4
#define RS PORTBbits.RB5


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

void affiche(unsigned char c,unsigned char d,unsigned char u)
{
	ecrire_LCD(centaine+48,1);
	ecrire_LCD(dizaine+48,1);
	ecrire_LCD(unite+48,1);
	ecrire_LCD(' ',1);
	ecrire_LCD('K',1);
	ecrire_LCD('m',1);
	ecrire_LCD('/',1);
	ecrire_LCD('h',1);
}

void ecrire_LCD(char donnee, char param)
{
	if(param != 2)
	{
		busy();
	}

	if(param !=1)
	{
		RS=0;
	}
	else
	{
		RS=1;
	}

	RW=0;
	//2 commandes avant E= pour attendre environ 140ns
	TRISD=0;
	PORTD=donnee;

	E=1;
	E=1;
	E=1;
	E=0;	
}

char lire_LCD()
{ 
	char valeur;
	RS=0;
	RW=1;
	TRISD=0b11111111;
	E=1;
	E=1;
	valeur=PORTD;
	E=1;
	E=0;
	return valeur;
}

void busy()
{ 
	char valeur;
	valeur=0b10000000;
	while(valeur==0b10000000)
	{
		valeur=lire_LCD();
		valeur = valeur & 0b10000000;
	}
}

//pour chaque sous programme commenter fonction


//programme d'initialisation
void init(void)
{
	TRISD=0;//choix de l'afficheur
	TRISC=0;//valeur de l'afficheur
	ADCON1=0x06;
	TRISB=1;	

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

	//sur 8 bits et 2 lignes validées
	ecrire_LCD(0x38,2);
	//efface l'afficheur
	ecrire_LCD(0x01,0);

	ecrire_LCD(0x0E,0);

	ecrire_LCD(0x06,0);

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
			ecrire_LCD(0x01,0);
			PR2=0x00;
			CCPR1L=0;
			CCP1CONbits.DC1B1 = 0; //bits du duty cycle (CCPR1L(00))
			CCP1CONbits.DC1B0 = 0;
		}
		affiche(centaine,dizaine,unite);
		for(i=0;i<5000;i++);
		ecrire_LCD(0x01,0);
	}
}

