/**
 *	\file		EX2.c
 *	\brief		Mod?le de programme source
 *				pour cible PIC18F452
  *	\date		19 sept 2025
 *	\author		Alexandra Quettier et Alexis Hecquet 
 *	\version	0.1
 *
 *	\Note		
 * objectifs : timer 1/100 s
 * fonctionnel : 
 * tests : 
 * limite : 
 * \ cablage : Port D sur les lettres du 7 segment et C0 pour Dis 0
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
void initLED(void);


/****************************************************************************/
/*		V A R I A B L E S    G L O B A L E S			                    */
/****************************************************************************/
	
unsigned char l;
char unite,dizaine,centaine,millier,dmillier;
char etat,flag,test;
unsigned int temps,affich,i,j;
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
    
    initLED();
    TRISD = 0; //on ecrit sur le 7 segment
    TRISC = 0; //on ecrit sur l'un des 4 7 segments
    PORTCbits.RC0 = 0; //on met dis 0 en 0 donc pour allumer les led il faut qu'elles aient la valeur 0
    
while(1) 
{
    PORTD = 0b10011111; //on dit qu'elle lettre allumer et eteindre -> affichage de 1
    
 // La suite du programme s'effectue en boucle

}
    return;
}
	
  /* Fin de la fonction main() */


/****************************************************************************/
/*		Z O N E   D E   S O U S   P R O G R A M M E S 							*/
/****************************************************************************/

	/*Mettre ici les sous programmes*/



/* Fin de la zone de sous programmes */
void initLED()
{	

		
}
