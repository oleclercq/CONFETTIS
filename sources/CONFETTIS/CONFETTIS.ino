/*  CANON A CONFETIS 
    CARTE ATMEGA 1656
	loma.fr
*/


#define TIMER_500MS 100

// ========================================================================
// DESFINIIONS
// ========================================================================
#define NB_CANON		8
#define MAX_TEMPO_RLY	900 // en nb de 10ms
#define MAX_TEMPO_LED	900	// en nb de 10ms

// ========================================================================
// DESFINIIONS DES TYPES      
// ========================================================================
typedef enum { CANON_READY, CANON_ON, CANON_OFF  } ENUM_ETAT_CANON;
typedef enum { CANON_LED_OFF, CANON_LED_ON, CANON_LED_CLI_ON, CANON_LED_CLI_OFF  } ENUM_ETAT_LED;

typedef struct 	{	ENUM_ETAT_CANON  eEtatCanon;      // Etat en cours.
                  ENUM_ETAT_LED    eEtatLed;        // Etat en cours.
					        uint8_t bpPin;			// Pin de l'arduino ou est branch� le bouton poussoir	
					        uint8_t pinLeD;			// pin de l'arduiuino ou est brancher la led du bouton
					        uint8_t pinRelayCanon;
					        uint16_t tempoLed;	// Nb de pas encore a effectuer pour �teindre la led
					        uint16_t tempoRelay;
				} ST_CANON;
				
// ========================================================================
// VARIABLES GLOBALES
// ========================================================================
// ************************************************************
// DEFINITIONS DES VARAIBLES
//! init tableau des producteurs cabl� pour un hard V2 	
ST_CANON tabCanon[NB_CANON] = 	{	{CANON_OFF, CANON_LED_OFF,33, 53, 2, MAX_TEMPO_LED, MAX_TEMPO_RLY},
									                {CANON_OFF, CANON_LED_OFF, 34, 46, 3, MAX_TEMPO_LED, MAX_TEMPO_RLY},
									                {CANON_OFF, CANON_LED_OFF, 35, 45, 4, MAX_TEMPO_LED, MAX_TEMPO_RLY},
									                {CANON_OFF, CANON_LED_OFF, 36, 44, 5, MAX_TEMPO_LED, MAX_TEMPO_RLY},
									                {CANON_OFF, CANON_LED_OFF, 37, 10, 6, MAX_TEMPO_LED, MAX_TEMPO_RLY},
									                {CANON_OFF, CANON_LED_OFF, 38, 11, 7, MAX_TEMPO_LED, MAX_TEMPO_RLY},
									                {CANON_OFF, CANON_LED_OFF, 39, 12, 8, MAX_TEMPO_LED, MAX_TEMPO_RLY},
									                {CANON_OFF, CANON_LED_OFF, 40, 13, 9, MAX_TEMPO_LED, MAX_TEMPO_RLY}		// pro->offsetK ne doit pas d�passer ni egal � ENTREENUMERIQUE 19
								} ;  

/* ************************************************************************ */
// /* configuration initial,                                                */
/* ************************************************************************ */
void setup()
{
#ifdef DEBUG_MODE
	Serial.begin(9600);
	Serial.println("CONFETTIS_V1.00");
#endif	

//  ================================================================================
//  configuration des Pinouts de l'arduino
	for (uint8_t i=0; i<NB_CANON; i++)		{
		pinMode(tabCanon[i].bpPin, 			INPUT);
		pinMode(tabCanon[i].pinLeD, 		OUTPUT);
		pinMode(tabCanon[i].pinRelayCanon,	OUTPUT);

    digitalWrite(tabCanon[i].pinRelayCanon,LOW);
    tabCanon[i].eEtatCanon = CANON_READY ;
    
    digitalWrite(tabCanon[i].pinLeD,HIGH);
    tabCanon[i].eEtatLed = CANON_LED_ON ;
	}
 

//  ================================================================================
//  PARAMETRAGE DU TIMER 1 16BIT
	cli();
	TCCR1A = 0;
	TCCR1B = ( 1<<WGM12) | (0<<CS12) | (1<<CS11) | (1<<CS10) ; 	// Prescaler / 64 ==> des PAS de 5ms
	TCCR1C = 0; 												// not forcing output compare
	TCNT1 = 0; 													// set timer counter initial value (16 bit value)
	
	OCR1A = 2500;												// Closer to one second than value above, 2500 pas de 4�s ca fait 10ms 
	TIMSK1 = 1 << OCIE1A; 										// Enable timer compare match 1A interrupt
	sei(); 														// Enable interrupts
}

/* ************************************************************************ */
/* PGM Principal, lecture en permanance les entr�es
/* ************************************************************************ */
void loop()
{
}

/* ************************************************************************ */
/* ON RENTRE DANS CE TIMER TOUTES LES 10ms
/* ************************************************************************ */
ISR(TIMER1_COMPA_vect) // 16 bit timer 1 compare 1A match
{
  
  // TEST_LED_ET_RELAYS(); //==> OK
  // TEST_ENTREES();       //==> OK
  
  // Lecture des Inters
      bool val;
      for (uint8_t i=0; i<NB_CANON; i++)		
      {
        if ( tabCanon[i].eEtatCanon == CANON_READY)
        {
          val = digitalRead(tabCanon[i].bpPin) ;
          if (val) // detection appuie
          {
            digitalWrite(tabCanon[i].pinRelayCanon,HIGH); // on active les cannons
            tabCanon[i].eEtatCanon = CANON_ON;
          }
        }
        else if ( tabCanon[i].eEtatCanon == CANON_ON)
        {
            // on décremente la temporisation du cannon
            if ( tabCanon[i].tempoRelay >0 )
            {
              tabCanon[i].tempoRelay--;
            }
            else
            {
                digitalWrite(tabCanon[i].pinRelayCanon,LOW); // on active les cannons
                tabCanon[i].eEtatCanon = CANON_OFF; // Le passage a READY ne se fera que sur app du RESET
                tabCanon[i].tempoRelay = MAX_TEMPO_RLY ;
            }
            // on fait clignoter les led
        }
      }

      /*
      if (tabCanon[i].eEtat == CANON_FEUX)
      {
        if (tabCanon[i].tempoRestanteRelay > 0)
        {
          tabCanon[i].tempoRestanteRelay--;
        } 
        else
        {
          tabCanon[i].eEtat == CANON_WAIT;
          tabCanon[i].tempoRestanteRelay = MAX_TEMPO_RLY ;
          digitalWrite(tabCanon[i].pinRelayCanon,LOW);
        }

        if (tabCanon[i].tempoRestanteLed > 0)
        {
            tabCanon[i].tempoRestanteLed--;
        }
        else
        {
           tabCanon[i].tempoRestanteLed = MAX_TEMPO_LED;
           digitalWrite(tabCanon[i].pinLeD,LOW);
        }
      } 
      else if ( !digitalRead(tabCanon[i].bpPin) == LOW)	// Lecture de toutes les entr�es
		  {
			// BT_Appuy�, on active le relay et la led
			//tabCanon[i].eEtat = CANON_FEUX;
			digitalWrite(tabCanon[i].pinRelayCanon,HIGH);
		} else
    {}
	}
 */
}

/* ************************************************** */
// RECOPIE ENTRESS SUR SORTIES
/* ************************************************** */
void TEST_ENTREES()
{
  bool val;
  for (uint8_t i=0; i<NB_CANON; i++)   
  {
    val = digitalRead(tabCanon[i].bpPin) ;
    digitalWrite(tabCanon[i].pinLeD,val);
    digitalWrite(tabCanon[i].pinRelayCanon,val);
  }
}

/* ************************************************** */
/* ************************************************** */
void TEST_LED_ET_RELAYS()
{
// /* TEST DES LEDS et DES  RELAYS ==> OK 
  static int cmp = TIMER_500MS; // 50x 10ms = 0.5s
  static int etat = 1;
  if (cmp-- == 0)
  { for (uint8_t i=0; i<NB_CANON; i++)   {
    if(etat){
      digitalWrite(tabCanon[i].pinLeD,HIGH);
      digitalWrite(tabCanon[i].pinRelayCanon,LOW);
    } else {
      digitalWrite(tabCanon[i].pinLeD,LOW);
      digitalWrite(tabCanon[i].pinRelayCanon,HIGH);
        }
    }
    etat = 1-etat;
    cmp = TIMER_500MS;
  }
}
