
/**
 *	\file		exl.c
 *	\brief		Mod?le de programme source
 *				pour cible PIC18F4685
  *	\date		19 sept 2025
 *	\author		Alexandra Q et Alexis H
 *	\version	0.1
 *
 *	\Note		
 * objectifs : afficher un chiffre
 * fonctionnel : oui
 * tests : affichage de 1234 
 * limite : peu pas faire plus qu'un nombre à quatre chiffre 
 * \ cablage : Port D sur les lettres du 7 segment et C0, C1, C6 et C7 pour Dis 0,1,2,3
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


/****************************************************************************/
/*		V A R I A B L E S    G L O B A L E S			                    */
/****************************************************************************/
	
char etat;
unsigned int i, temps, attente=200;

char number[]={
    0b11111111, //0
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
   
}



/****************************************************************************/
/*		F O N C T I O N     P R I N C I P A L E 							*/
/****************************************************************************/
void main(void) {
    
    init();
    
while(1) 
{
    setNumber(1,0b11111110);
    wait();

    setNumber(2,0b11111101);
    wait();

    setNumber(3,0b10111111);
    wait();

    setNumber(4,0b01111111);
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