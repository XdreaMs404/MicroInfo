
/**
 *	\file		ex5.c
 *	\brief		Mod?le de programme source
 *				pour cible PIC18F4685
  *	\date		19 sept 2025
 *	\author		Alexandra Q et Alexis H
 *	\version	0.1
 *
 *	\Note		
 * objectifs : tout avec bouton 0
 * fonctionnel : à moitié -> il manque le temps intermediaire
 * tests : -
 * limite : -
 * \ cablage : Port D sur les lettres du 7 segment et C0, C1, C6 et C7 pour Dis 0,1,2,3 et B0 avec BP0 pour bouton 1 et B1 avec BP1 pour bouton 0
 */

/****************************************************************************/
/*		I N C L U D E S   S T A N D A R D S        							*/
/****************************************************************************/
#include <xc.h>



/****************************************************************************/
/*		C O N F I G U R A T I O N	                                        */
/****************************************************************************/
#pragma config OSC = XT         // Oscillator Selection bits (XT oscillator)
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled #pragma config OSC = ECIO



/****************************************************************************/
/*		C O N S T A N T E S  S Y M B O L I Q U E S		                   */
/****************************************************************************/
 #define pot0		PORTAbits.RA0





/****************************************************************************/
/*		P R O T O T Y P E S   D E   F O N C T I O N S						*/
/****************************************************************************/
void init(void); //Fonction d'initialisation
void setNumber(int x, char afficheur);
void wait(); //Fonction d'attente d'environ 1 s
void initTIMER0();
void initINT0();
void initINT1();


/****************************************************************************/
/*		V A R I A B L E S    G L O B A L E S			                    */
/****************************************************************************/
	
char etat,flag;
unsigned int i, temps, attente=200, mili =0,centi = 0, unite = 0,dizaine = 0,stop=0;

char number[]={
    0b00000011, //0 l'ancien affichage de 0 ne marcher pas maintenant si
    0b10011111, //1
    0b00100101, //2
    0b00001101, //3
    0b10011001, //4
    0b01001001, //5
    0b01000001, //6
    0b00011111, //7
    0b00000001, //8
    0b000001001 //9
    };
/****************************************************************************/
/*		T R A I T E M E N T S   D' I N T E R R U P T I O N                  */
/****************************************************************************/

void  __interrupt(high_priority) myHighint(void)             // High priority interrupt
{
   /*  if(Flag1==1)
   * { */
        if (INTCONbits.TMR0IF == 1)
    {
        TMR0H = 0xD8;
        TMR0L = 0xEF;
        /*if (PORTBbits.RB0==1)
        {
            PORTBbits.RB0=0;
        }
        else
        {
            PORTBbits.RB0=1;
        }*/
        if (stop == 0) //pour faire marcher
        {
            mili += 1;  //le 7 segment de tout a droite 00.0mili
            if (mili >= 10){ //si au dessus de 10 ou egal on retourne a 1 et incrémente les centi
                mili = mili % 10;
                centi +=1; //le 7 segment avant dernier par la droite 00.centi0
            }
            if (centi >= 10){
                centi = centi % 10;
                unite +=1; //le deuxieme 7 segment en partant de la gauche 0unite.00
            }
            if (unite >= 10){
                unite = unite % 10;
                dizaine +=1; //le premier 7 segment en partant de la gauche dizaine0.00
            }
            if (dizaine >= 10){
                dizaine = dizaine % 10;
            }
        }
       
                
        INTCONbits.TMR0IF = 0;
    }
        if (INTCONbits.INT0IF == 1)
    {
        if (stop==1) //si est a 1 on met en route
        {
            stop=0;
        }
        else //sinon on met en arret
        {
            stop=1;
        }
        INTCONbits.INT0IF = 0;
    }
        
        if (INTCON3bits.INT1IF == 1)
    {
        if (stop==1) //si est a 1 on reinitialise
        {
            unite=0;
            centi=0;
            mili=0;
            dizaine=0;
           
        }
        INTCON3bits.INT1IF = 0;
    }
        
  /* }
   */
}



/****************************************************************************/
/*		F O N C T I O N     P R I N C I P A L E 							*/
/****************************************************************************/
void main(void) {
    
    init();
    initTIMER0();
    initINT0();
    initINT1();
    
while(1) 
{
    setNumber(dizaine,0b11111110);
    wait();
        
    setNumber(unite,0b11111101);
    PORTDbits.RD0=0; //pour avoir le point
    wait();
    
    setNumber(centi,0b10111111);
    wait();
        
    setNumber(mili,0b01111111);
    wait();


}
    return;
}
	
  /* Fin de la fonction main() */


/****************************************************************************/
/*		Z O N E   D E   S O U S   P R O G R A M M E S 							*/
/****************************************************************************/

	/*Mettre ici les sous programmes*/


/* Fin de la zone de sous programmes */
void init()
{	
    TRISC=0; //sortie C
    TRISD=0; //sortie D
    TRISB =3;
    
    
    PORTC=0;
    PORTD=0;
}

void setNumber(int x, char afficheur){
    
    PORTC=0b11111111; //reinitialisation -> on eteint tout
    PORTD=number[x]; //quel nombre afficher
    PORTC=afficheur; //lequel segment
}

void wait(){
    for(temps=0; temps<attente ; temps++){
        etat=etat;
    }
}

void initTIMER0()
    {
        T0CONbits.TMR0ON=1;
        T0CONbits.T08BIT=0; //sur 16 bits
        T0CONbits.T0CS=0;
        T0CONbits.T0SE=0;
        T0CONbits.PSA=1;
        
        INTCONbits.GIE=1;/*autorise les interruptions globales*/
        INTCONbits.TMR0IE=1; //interruption spécifique
        INTCONbits.TMR0IF = 0;
        TMR0H = 0xD8;
        TMR0L = 0xEF;
        
    }

void initINT0()
    {
        PORTBbits.RB0=1;
        INTCONbits.GIE=1;/*autorise les interruptions globales*/
        INTCONbits.INT0IE=1;
        
    }

void initINT1()
{
    PORTBbits.RB1=1;
    INTCONbits.GIE=1;
    INTCON3bits.INT1IE=1;
}
