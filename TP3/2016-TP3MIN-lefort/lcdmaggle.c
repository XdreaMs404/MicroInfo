// Descriptif general du programme
// Auteur(s), Date

#include <p18C452.h>//inclus la librairie contenant les d?finitions des bits et des registres

#pragma config OSC = XT // oscillatateur ext?rieur
#pragma config WDT = OFF // pas de chien de garde

void traiter_it(void);// declaration programme gestion interruption
void init (void);

// d?clarer vos variables et constantes


#pragma  code it=0x08  //adresse interruption haute
       
void saut_sur_spit(void)
{_asm
   	goto traiter_it
 _endasm
}


#pragma code
// Sous programmes 
void lire_LCD()
{
	E
	RS
	Rw
	PORTD


}

//pour chaque sous programme commenter fonction
int conversion(int a)
{

}

//programme d'initialisation
void init(void)
{

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
 init();
for(;;) //effectue le programme suivant en boucle
	{
		afficher(conversion(teninter));
	}
}

