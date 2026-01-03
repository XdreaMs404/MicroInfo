// BENCHIcHA SALENGRO

// Descriptif général du programme ; Jouer une Note de musique
// Auteur(s), Date

#include <p18f452.h>//inclus la librairie contenant les définitions des bits et des registres
#include<delays.h>
#pragma config OSC = XT // oscillatateur extérieur
#pragma config WDT = OFF // pas de chien de garde

void traiter_it(void);// declaration programme gestion interruption
void init (void);
char read_lcd(void);
void codeFaux();
void effacer();
void codeBon();
void code();
char valider();
void ReadClavier(char);
unsigned char GetChar(void);

// déclarer vos variables et constantes
unsigned char lettre;
unsigned char lettrep=0;
unsigned char mdp[4]={'0','0','0','0'}; // Initialise la table mot de passe
unsigned char codebon2[4]={'2','5','8','0'}; // Notre code
int temp=0;
#pragma  code it=0x08  //adresse interruption haute
      
void saut_sur_spit(void)
{_asm
       goto traiter_it
 _endasm
}

#pragma code
// Sous programmes

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





void effacer()
{
    write_lcd(0x01,0); //On rénitialise
}


void ReadClavier(char lettre)
{
	write_lcd(lettre,1);
}



unsigned char GetChar(void)
{
    unsigned char tempo=0;
    unsigned char i,j;
    const char tab[12]={'#','9','6','3','0','8','5','2','*','7','4','1'};
    PORTA=0x01;
    i=0;
    while ((i<4) && ((PORTE&0x07)==0))
    {
    PORTA=PORTA*2;
    i++;
    }
    if ((PORTE&0x07)!=0)
    {
    if ((PORTE&0x07)==1) j=i;
    if ((PORTE&0x07)==2) j=4+i;
    if ((PORTE&0x07)==4) j=8+i;
    tempo=tab[j];
    }
    return tempo;
}


//pour chaque sous programme commenter fonction


//programme d'initialisation
void init(void)
{

/*Jouer une note TP5*/
 /*On prend une fréquence de 262 -> Do.
	1/262 = 3816 µs.On divise par 16. (Il ne faut pas dépasser les 255)
	PR2 Do-> 238,5
	PR2 Do# -> 255,625
	PR2 Ré -> 212,5625
	PR2 Ré# ->200,9375
	PR2 MI -> 189,375
	PR2 Fa -> 176,0625
	PR2 Fa# -> 168,875
	PR2 Sol -> 159,4375
	PR2 sol# -> 150,5625
	PR2 La -> 142
	PR2 Si bemol -> 134,0125
	PR2 Si -> 126,5
	PR2 do -> 119,25
*/


	CCP1CON = 0b00001100; //PWM Mode
	TRISC = 0x00; //mise en sortie
	T2CON = 0b00000110 ; // enable Timer2

/*
do si do re si la do si la do do la do si  
*/
} 




//programme qui traite les interruptions
#pragma interrupt traiter_it

void  traiter_it(void)
{       

}


void note(int n)
{
TRISC = 0x00;
	switch(n)
	{
		/*	case 1:
		PR2 = 238;
			CCPR1L = 0b01110111; //TH -> PR2/2
			break; */
		case 1: //si
			PR2 = 126;
			CCPR1L = 0b00111111;
			break;
	/*	case 1: //re
			PR2 = 212;
			CCPR1L = 0b01101010;
			break; */
		case 2:
			PR2 = 142; //la
			CCPR1L = 0b01000111; //TH -> PR2/2
			break;
	}
}


#pragma code
// programme principal
 void main(void)
{
	int t;
 init();
//PR2 = 0b10001110; //142 -> La
//PR2 = 238;
//PR2 = 126;
//PR2 = 238;
//PR2 = 212;
//PR2 = 126;
//PR2 = 142;
//PR2 = 238;
//PR2 = 126;
//PR2 = 142;
//PR2 = 238;
//PR2 = 238;
//PR2 = 142;
//PR2 = 238;
//PR2 = 126;
CCPR1L = 0b01000111; //TH -> PR2/2
for(;;) //effectue le programme suivant en boucle
{
	for(t=1;t<=2;t++){
		Delay10KTCYx(50);
		note(t);
	}
	t=1;
}

}
