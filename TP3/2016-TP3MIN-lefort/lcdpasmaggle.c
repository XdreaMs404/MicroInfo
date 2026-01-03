// Descriptif general du programme
// Auteur(s), Date

#include <p18C452.h>//inclus la librairie contenant les definitions des bits et des registres

#pragma config OSC = XT // oscillatateur exterieur
#pragma config WDT = OFF // pas de chien de garde

void traiter_it(void);// declaration programme gestion interruption
void init (void);// declarer vos variables et constantes
//afficheur LCD
#define E PORTBbits.RB3
#define RW PORTBbits.RB4
#define RS PORTBbits.RB5

//DIGICODE
#define lbis PORTAbits.RA0//*0#
#define	l1 PORTAbits.RA3//1.2.3
#define l2 PORTAbits.RA2//4.5.6
#define l3 PORTAbits.RA1//7.8.9

#define c1 PORTEbits.RE2//1.4.7
#define c2 PORTEbits.RE1//2.5.8
#define c3 PORTEbits.RE0//3.6.9

char chiffre1;
char chiffre2;
char chiffre3;
char chiffre4;

int i,flag=0;



#pragma  code it=0x08  //adresse interruption haute
       
void saut_sur_spit(void)
{_asm
   	goto traiter_it
 _endasm
}


#pragma code
// Sous programmes 


//pour chaque sous programme commenter fonction
void ecrire_LCD(char donnee, char param);
char lire_LCD();
void busy();
char digicode();

//programme d'initialisation
void init(void)
{	
	TRISB=0;

//digicode
	ADCON1=0x06;
	TRISAbits.TRISA0=0;
	TRISAbits.TRISA1=0;
	TRISAbits.TRISA2=0;
	TRISAbits.TRISA3=0;
	TRISEbits.TRISE0=1;
	TRISEbits.TRISE1=1;
	TRISEbits.TRISE2=1;
	
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
	
}


#pragma code
// programme principal
 void main(void) 
{
	char code[4];
	code[0]='4';
	code[1]='2';
	code[2]='4';
	code[3]='7';
	init();
	
	for(;;) //effectue le programme suivant en boucle
	{
		//ecrire_LCD(0x01,0);
		flag=0;
		do{
			chiffre1=digicode();
			for(i=0;i<=10000;i++);
				if(chiffre1=='#')
				{
					ecrire_LCD(0x01,0);
					chiffre1='A';
					chiffre2='A';
					chiffre3='A';
					chiffre4='A';
					flag=1;
				}
				if(chiffre1=='*')
				{
					ecrire_LCD(0x01,0);
					chiffre1='A';
					flag=1;
				}
		}while(chiffre1=='A');
		ecrire_LCD(chiffre1,1);
		

		if(flag==0)
		{
			do{
			chiffre2=digicode();
			for(i=0;i<=10000;i++);
				if(chiffre2=='#')
				{
					ecrire_LCD(0x01,0);
					chiffre1='A';
					chiffre2='A';
					chiffre3='A';
					chiffre4='A';
					flag=1;
				}
				if(chiffre2=='*')
				{
					ecrire_LCD(0x01,0);
					chiffre1='A';
					chiffre2='A';
					flag=1;
				}
			}while(chiffre1==chiffre2 || chiffre2=='A');
			ecrire_LCD(chiffre2,1);
		}

		if(flag==0)
		{
			do{
			chiffre3=digicode();
			for(i=0;i<=10000;i++);
				if(chiffre3=='#')
				{
					ecrire_LCD(0x01,0);
					chiffre1='A';
					chiffre2='A';
					chiffre3='A';
					chiffre4='A';
					flag=1;
				}
				if(chiffre3=='*')
				{
					ecrire_LCD(0x01,0);
					chiffre2='A';
					chiffre3='A';
					flag=1;
				}
			}while(chiffre2==chiffre3 || chiffre3=='A');
			ecrire_LCD(chiffre3,1);
		}

		if(flag==0)
		{
			do{
			chiffre4=digicode();
			for(i=0;i<=10000;i++);
				if(chiffre4=='#')
				{
					ecrire_LCD(0x01,0);
					chiffre1='A';
					chiffre2='A';
					chiffre3='A';
					chiffre4='A';
					flag=1;
				}
				if(chiffre4=='*')
				{
					ecrire_LCD(0x01,0);
					chiffre3='A';
					chiffre4='A';
					flag=1;
				}
			}while(chiffre3==chiffre4 || chiffre4=='A');
			ecrire_LCD(chiffre4,1);
		}

		if((chiffre1!='A')&&(chiffre2!='A')&&(chiffre3!='A')&&(chiffre4!='A'))
		{
			if((code[0]==chiffre1)&&(code[1]==chiffre2)&&(code[2]==chiffre3)&&(code[3]==chiffre4))
			{
				ecrire_LCD(0x01,0);
				ecrire_LCD('C',1);
				ecrire_LCD('O',1);
				ecrire_LCD('D',1);
				ecrire_LCD('E',1);
				ecrire_LCD(' ',1);
				ecrire_LCD('B',1);
				ecrire_LCD('O',1);
				ecrire_LCD('N',1);
				for(;;);
			}
			else
			{
				ecrire_LCD(0x01,0);
				ecrire_LCD('C',1);
				ecrire_LCD('O',1);
				ecrire_LCD('D',1);
				ecrire_LCD('E',1);
				ecrire_LCD(' ',1);
				ecrire_LCD('F',1);
				ecrire_LCD('A',1);
				ecrire_LCD('U',1);
				ecrire_LCD('X',1);
				for(i=0;i<=30000;i++);
				for(i=0;i<=10000;i++);
				ecrire_LCD(0x01,0);
			}
		}
	}
}

char digicode()
{
	char chiffre;
	chiffre='A';
	l1=1;
	l2=0;
	l3=0;
	lbis=0;
	for(i=0;i<=100;i++);
	if(c1==1) chiffre='1';
	if(c2==1) chiffre='2';
	if(c3==1) chiffre='3';


	l1=0;
	l2=1;
	for(i=0;i<=100;i++);
	if(c1==1) chiffre='4';
	if(c2==1) chiffre='5';
	if(c3==1) chiffre='6';

	l2=0;
	l3=1;
	for(i=0;i<=100;i++);
	if(c1==1) chiffre='7';
	if(c2==1) chiffre='8';
	if(c3==1) chiffre='9';

	l3=0;
	lbis=1;
	for(i=0;i<=100;i++);
	if(c1==1) chiffre='*';
	if(c2==1) chiffre='0';
	if(c3==1) chiffre='#';

	return chiffre;
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

