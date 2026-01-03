
/**
 *	\file		squel.c
 *	\brief		Mod?le de programme source
 *				pour cible PIC18F4685
  *	\date		1er sept 2010
 *	\author		Vous
 *	\version	0.1
 *
 *	\Note		A compl?ter
 * 
 * \ cablage
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
#define RS PORTBbits.RB5
#define RW PORTBbits.RB4
#define E PORTBbits.RB3






/****************************************************************************/
/*		P R O T O T Y P E S   D E   F O N C T I O N S						*/
/****************************************************************************/
void init(void);
void ecrire(char donne, char param);
void busy();
char lire_commande();



/****************************************************************************/
/*		V A R I A B L E S    G L O B A L E S			                    */
/****************************************************************************/
	
unsigned char i;
char unite,dizaine,centaine,millier,dmillier;
char etat,flag,test;
unsigned int temps,affich;
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
    ecrire('B', 1);
    ecrire('O', 1);
    for(i=0;i<40;i++){
        ecrire(0x14, 0);
    }
    ecrire('N', 1);
    ecrire('J', 1);
    
while(1) 
{
    
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
void init()
{	TRISB=0;
    ecrire(0x38, 2);
    ecrire(0x38, 2);
    ecrire(0x1, 0);
    ecrire(0x0E, 0);
    ecrire(0x06, 0);
}

void ecrire(char donne, char param){
    if (param != 2){
        busy();
    }
    if ((param==0)||(param==2)){
        RS=0;
        RW=0;
    }
    if (param == 1){
        RS=1;
        RW=0;
    }
    TRISD=0;
    PORTD=donne;
    E=1;
    E=1;
    E=1;
    E=0;
}

void busy(){
    char valeur =0x80;
    while(valeur==0x80){
        valeur=lire_commande();
        valeur=(valeur & 0x80);
    }
}

char lire_commande(){
    char valeur;
    RS= 0;
    RW=1;
    TRISD=0b11111111;
    E=1;
    E=1;
    valeur = PORTD;
    E=0;
    return valeur;
}