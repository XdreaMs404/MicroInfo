// BENCHIHA SALENGRO

// Lire le note avec le clavier
// Auteur(s), Date

#include <p18C452.h>//inclus la librairie contenant les définitions des bits et des registres

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
    TRISB=0b11000111;
    PORTB=0;
    TRISD=0xFF;
    TRISA=0xF0;
   ADCON1=0x07;
	INTCON3bits.INT1IE=1;
	INTCONbits.GIE=1;

	CCP1CON = 0b00001100; //PWM Mode

	T2CON = 0b00000110 ; // enable Timer2
}

void note(char n)
{

	switch(n)
	{
		case '1': //do

		PR2 = 238;
			CCPR1L = 0b01110111; //TH -> PR2/2
;
			break; 
		case '2': //re

			PR2 = 212;
			CCPR1L = 0b01101010;
			break;
		case '3': //mi

			PR2 = 189;
			CCPR1L = 0b01011110;
			break; 
		case '4':

			PR2 = 176; //fa
			CCPR1L = 0b01011000; //TH -> PR2/2
			break;
		case '5': //sol

		PR2 = 159;
			CCPR1L = 0b01001111; //TH -> PR2/2
			break; 
		case '6': //la

			PR2 = 142;
			CCPR1L = 0b01000111;
			break;
		case '7': //si

			PR2 = 126;
			CCPR1L = 0b00111111;
			break; 
		case '8': //do

			PR2 = 119; //la
			CCPR1L = 0b00111011; //TH -> PR2/2
			break;
		case '9': //Ré#

		PR2 = 200;
			CCPR1L = 0b001100100; //TH -> PR2/2
			break; 
		case '0': //fa#

			PR2 = 169;
			CCPR1L = 0b001010100;
			break;
		case '#': //sol#

			PR2 = 150;
			CCPR1L = 0b001001011;
			break; 
		case '*': //si bémol

			PR2 = 134; //la
			CCPR1L = 0b001000011; //TH -> PR2/2
			break;
		
	}

}



//programme qui traite les interruptions
#pragma interrupt traiter_it

void  traiter_it(void)
{       
if (INTCON3bits.INT1IF)
	{
	INTCON3bits.INT1IF=0;
	}
}




#pragma code
// programme principal
 void main(void)
{
int i;
 init();
write_lcd(0x38,2); //initialiser,8bits
write_lcd(0x38,0); //on est prê a ecrire
write_lcd(0x0E,0);
write_lcd(0x01,0);




for(;;) //effectue le programme suivant en boucle
{TRISC = 0x00;
        lettre=GetChar();
		note(lettre);


    }

}