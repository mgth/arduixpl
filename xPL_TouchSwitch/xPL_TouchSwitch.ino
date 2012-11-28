//#define XPL_DEBUG
#define XPL_CONFIG		//  1372 Kb
//#define XPL_GROUP		//  1880 Kb
//#define XPL_FILTER		//  2170 Kb

#include <EtherCard.h>

#include <ps2.h>
#include <Trackpad.h>

#include <xPL.h>
#include <xPL_AdapterENC28J60.h>

#include <Arduino.h>


unsigned int CLICLEVEL = 10; // click level detect
int TCLIC = 300;    // clic max time
float NOISE = 0.1 ; // click error
int ORIENTATION = 1;// pad orientation

int SlowDimmer = 128; // Variation pour la hauteur du pad, avec un doigt
int FastDimmer = 512; // ... avec deux doigts.

// initialize the library with the numbers of the interface pins
Trackpad t(11,10);

// Limites initiales du pad (valeurs auto-ajustées à l'utilisation)
// (valeurs absolues avant orientation)
unsigned int xMin = 1126; 
unsigned int xMax = 5797;
unsigned int yMin = 818;
unsigned int yMax = 4959;

float x,y;      // position

float xStart=0; // position au début du clic
float yStart=0;
int tStart;     // heure au début du clic

float xMove=0;  // déplacement depuis le début du clic
float yMove=0;

boolean hFix=true; // Détection du respect des axes lors des mouvements
boolean vFix=true;

int fingers = 0;   // Nombre de doigts sur le pad
int oldFingers = 0;

int maxProg = 3;   // Nombre total de programmes
int prog = 0;      // programme en cours
int dimmer = 255;   // Variation générale du programme
int newDimmer=255;  // Variation pendant
int power = false; // Allumé/Eteind


int main(void)
{
	#include <xPL.h>

	while(1)
	{
		xPL.loopAll();

		packet_t * packet;
		packet = t.getNewPacket();

			if (packet->z > CLICLEVEL) 
			{
			  if (packet->w >=4 )
			  {
				if (fingers<2) // si on a déjà détecté deux doigts, on garde : au relachement on passe régulièrement par un doigt
				  fingers = 1; // un seul doigt;
			  }
			  else if (packet->w == 0) fingers = 2; // deux doigts;
			  else fingers=0;
			}
			else fingers=0;

			if (packet->x > xMax) xMax = packet->x;
			if (packet->x < xMin && packet->x > 100 ) xMin = packet->x;
			if (packet->y > yMax) yMax = packet->y;
			if (packet->y < yMin && packet->y > 100 ) yMin = packet->y;
        
			switch(ORIENTATION)
			{
			  case(0):
			  {
				x = (float)(packet->x - xMin) / (xMax-xMin) ;
				y = (float)(packet->y - yMin) / (yMax-yMin) ;
				break;
			  }
			  case(1):
			  {
				y = (float)(xMax - packet->x) / (xMax-xMin) ;
				x = (float)(packet->y - yMin) / (yMax-yMin) ;
				break; 
			  }
			  case(2):
			  {
				x = (float)(xMax - packet->x) / (xMax-xMin) ;
				y = (float)(yMax - packet->y) / (yMax-yMin) ;
				break;
			  }
			  case(3):
			  {
				y = (float)(packet->x - xMin) / (xMax-xMin) ;
				x = (float)(yMax - packet->y) / (yMax-yMin) ;
				break; 
			  }
		   }
       
        
			if ( oldFingers == 0 )
			{
			  if ( fingers > 0 )      ///////////////////////////////////// début de clic
			  {
				tStart = millis();
            
				xStart = x;    yStart = y;
				xMove  = 0;    yMove  = 0;
				hFix   = true; vFix = true;
            
				oldFingers = fingers;
				newDimmer  = dimmer;
			  }
			}
			else
			{
			  if ( fingers == 0) /////////////////////////////////////////// fin de clic
			  {
				int tClic = millis() - tStart;  // durée du clic
            
				if ( hFix && vFix && tClic<=TCLIC )      power  = !power;    // CLIC
            
				if ( (hFix && tClic > TCLIC) || !vFix  ) dimmer = newDimmer; // VARIATION
				else newDimmer=dimmer;
            
				if ( !hFix && vFix )                                         // PROGRAMME
				{
				  if (xMove>0)
				  {
					if (prog<maxProg) prog++; else prog=0;
				  }
				  else 
				  {
					if (prog>0) prog--; else prog=maxProg;
				  }
				}
             
				oldFingers = 0;
			  }
        
			  else  //////////////////////////////////////////////////////// Pendant le Clic
			  {
				int tClic = millis() - tStart;  // durée du clic
            
				xMove= x-xStart;
				hFix = ( abs(xMove) < NOISE );
				yMove= y-yStart;
				vFix = ( abs(yMove) < NOISE );
        
				if ( (hFix && tClic > TCLIC) || !vFix  )
				  newDimmer = dimmer + (yMove * (fingers==1 ? SlowDimmer : FastDimmer) );
				else
				  newDimmer = dimmer;
              
				if (newDimmer<0)   newDimmer = 0;
				if (newDimmer>255) newDimmer = 255;
			  }
			}



	}
}