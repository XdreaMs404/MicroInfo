
/**
 *	\file		chap1.c
 *	\brief		Mod?le de programme source
 *				pour cible PIC18F4685
  *	\date		17/10/2025
 *	\author		Vous
 *	\version	0.1
 *
 *	\Note		A compl?ter
 * 
 * \ cablage
 * RS:B5
 * B4:RW
 * B3:E
 * PortD:afficheur LCD port
 * 
 * Test : Affichage de BO sur l'�cran puis affichage de BO et NJ en passant 40 cases pour un retour � la ligne
 * w
 * ///////////////////////Fonctionnel ///////////////////////////////
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
char digit();



/****************************************************************************/
/*		V A R I A B L E S    G L O B A L E S			                    */
/****************************************************************************/
	
unsigned char i;
char unite,dizaine,centaine,millier,dmillier;
char etat,flag,test,var3;
unsigned int temps,affich;
char matrix[4][4] = {
{'1', '2', '3', 'A'},
{'4', '5', '6', 'B'},
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
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
    
    
    
    init();//initialisation de l'�cran et du pic

    
while(1) 
{
    var3 = digit();
    ecrire(var3, 1);
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
ADCON1=0x06;
    ecrire(0x38, 2);//init de fonction N=1 (pour 2 lignes) 8bits, matrice de 5*8 (F=0)
    ecrire(0x38, 2);
    ecrire(0x1, 0);//effacer l'affichage
    ecrire(0x0E, 0);//on affiche le curseur et absence de clignotement
    ecrire(0x06, 0);//deplacement vers la droite et affichage qui ne bouge pas
    
    
}

void ecrire(char donne, char param){
    if (param != 2){
        busy();//verifie que l'ecran a termin� sa tache precedente
    }
    if ((param==0)||(param==2)){//verifie qu'il est en config
        RS=0;// mode commande (pour configurer)
        RW=0;//miode ecriture
    }
    if (param == 1){
        RS=1;//mode donn�e (pour ecrire sur l'ecran)
        RW=0;
    }
    TRISD=0;
    PORTD=donne;//ecrit dans le port
    E=1;//trois fois cette commande pour attendre450ms, se place en position haute
    E=1;
    E=1;
    E=0;//position basse provoque front descendant
}

void busy(){//verifie si l'ecran a ter�in� la demande precedente et atte,d que ce soit le cas
    char valeur =0x80;
    while(valeur==0x80){
        valeur=lire_commande();
        valeur=(valeur & 0x80);
    }
}

char lire_commande(){//vas lire le flag (le BL))
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

char digit(){
    TRISA=0;
    TRISEbits.TRISE0=1;
    TRISEbits.TRISE1=1;
    TRISEbits.TRISE2=1;
    TRISC=255;
    while(1){
        for(i=0;i<4;i++){
            PORTAbits.RA0 = 0;
            PORTAbits.RA1 = 0;
            PORTAbits.RA2 = 0;
            PORTAbits.RA3 = 0;
            
            
            
            if (i==0){
                PORTAbits.RA0 = 1;
            }
            if (i==1){
                PORTAbits.RA1 = 1;
            }
            if (i==2){
                PORTAbits.RA2 = 1;
            }
            if (i==3){
                PORTAbits.RA3 = 1;
            }
            
            if (PORTCbits.RC0 == 1){
                while(PORTCbits.RC0 == 1);
                return matrix[0][i];
            }
            if (PORTEbits.RE0 == 1){
                while(PORTEbits.RE0 == 1);
                return matrix[1][i];
            }
            if (PORTEbits.RE1 == 1){
                while(PORTEbits.RE1 == 1);
                return matrix[2][i];
            }
            if (PORTEbits.RE2 == 1){
                while(PORTEbits.RE2 == 1);
                return matrix[3][i];
            }

            
        }
    }
}