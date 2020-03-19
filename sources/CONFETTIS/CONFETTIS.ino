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
#define MAX_TEMPO_LED	50	// en nb de 10ms
#define PIN_BT_RESET 	32


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
//! init tableau des producteurs cabl� pour un hard V2 	
//#define MODE_OLQ
#ifdef _MODE_OLQ
ST_CANON tabCanon[NB_CANON] = 	{	{CANON_OFF, CANON_LED_OFF, 33, 53, 2, MAX_TEMPO_LED, MAX_TEMPO_RLY},
									{CANON_OFF, CANON_LED_OFF, 34, 46, 3, MAX_TEMPO_LED, MAX_TEMPO_RLY},
									{CANON_OFF, CANON_LED_OFF, 35, 45, 4, MAX_TEMPO_LED, MAX_TEMPO_RLY},
									{CANON_OFF, CANON_LED_OFF, 36, 44, 5, MAX_TEMPO_LED, MAX_TEMPO_RLY},
									{CANON_OFF, CANON_LED_OFF, 37, 10, 6, MAX_TEMPO_LED, MAX_TEMPO_RLY},
									{CANON_OFF, CANON_LED_OFF, 38, 11, 7, MAX_TEMPO_LED, MAX_TEMPO_RLY},
									{CANON_OFF, CANON_LED_OFF, 39, 12, 8, MAX_TEMPO_LED, MAX_TEMPO_RLY},
									{CANON_OFF, CANON_LED_OFF, 40, 13, 9, MAX_TEMPO_LED, MAX_TEMPO_RLY}		// pro->offsetK ne doit pas d�passer ni egal � ENTREENUMERIQUE 19
								} ;  
#else
ST_CANON tabCanon[NB_CANON] = 	{	{CANON_OFF, CANON_LED_OFF, 33, 6, 22, MAX_TEMPO_LED, MAX_TEMPO_RLY},
									{CANON_OFF, CANON_LED_OFF, 34, 7, 24, MAX_TEMPO_LED, MAX_TEMPO_RLY},
									{CANON_OFF, CANON_LED_OFF, 35, 8, 26, MAX_TEMPO_LED, MAX_TEMPO_RLY},
									{CANON_OFF, CANON_LED_OFF, 36, 9, 28, MAX_TEMPO_LED, MAX_TEMPO_RLY},
									{CANON_OFF, CANON_LED_OFF, 37, 10, 30, MAX_TEMPO_LED, MAX_TEMPO_RLY},
									{CANON_OFF, CANON_LED_OFF, 38, 11, 32, MAX_TEMPO_LED, MAX_TEMPO_RLY},
									{CANON_OFF, CANON_LED_OFF, 39, 12, 34, MAX_TEMPO_LED, MAX_TEMPO_RLY},
									{CANON_OFF, CANON_LED_OFF, 40, 14, 36, MAX_TEMPO_LED, MAX_TEMPO_RLY}		// pro->offsetK ne doit pas d�passer ni egal � ENTREENUMERIQUE 19
								} ;
#endif


// ========================================================================
// PROTOTYPAGES DES FONCTIONS
// ========================================================================
void fnct_read_bt(int i) ;
void fnct_canon_on(int i) ;
void fnct_canon_off(int i) ;
void fnct_canon_led_off(int i);
void fnct_canon_led_on(int i);
void fnct_canon_led_off_cli(int i);
void fnct_canon_led_oon_cli(int i);

void PGM_NORMAL();
void TEST_ENTREES() ;
void TEST_LED_ET_RELAYS() ;
void TEST_LED_ET_RELAYS_CHENILLARD();



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
    tabCanon[i].eEtatLed = CANON_LED_CLI_OFF ;  // Prochain état
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
  TEST_LED_ET_RELAYS_CHENILLARD();
  // TEST_LED_ET_RELAYS(); //==> OK
  // TEST_ENTREES();       //==> OK
  // PGM_NORMAL();         //==> OK
}

/* ************************************************** */
// RECOPIE ENTRESS SUR SORTIES
/* ************************************************** */
void PGM_NORMAL()
{
      bool val;
      for (uint8_t i=0; i<NB_CANON; i++)		
      {
        switch ( tabCanon[i].eEtatCanon) {
          case CANON_READY :  fnct_read_bt(i);   break ;
          case CANON_ON  :    fnct_canon_on(i);  break ;
          case CANON_OFF :    fnct_canon_off(i);  break ;
          default :           break ;                                 
        }
      }

      // Lecture du Bt RESET
      if (!digitalRead(PIN_BT_RESET))
      {
        for (uint8_t i=0; i<NB_CANON; i++)    
        {
          // tous les connons passe en ready
          digitalWrite(tabCanon[i].pinRelayCanon,LOW);
          tabCanon[i].eEtatCanon = CANON_READY;
          
          digitalWrite(tabCanon[i].pinLeD,HIGH);
          tabCanon[i].eEtatLed = CANON_LED_ON;
        }
    }
}

/* ************************************************** */
// RECOPIE ENTRESS SUR SORTIES
/* ************************************************** */
void fnct_read_bt(int i)
{
  bool val; 
  val = digitalRead(tabCanon[i].bpPin) ;
  if (!val) // detection appuie
  {
    digitalWrite(tabCanon[i].pinRelayCanon,HIGH); // on active les cannons
    tabCanon[i].eEtatCanon = CANON_ON;

    digitalWrite(tabCanon[i].pinLeD,HIGH); // on active les cannons
    tabCanon[i].eEtatLed = CANON_LED_CLI_ON;
  }
}

/* ************************************************** */
// RECOPIE ENTRESS SUR SORTIES
/* ************************************************** */
void fnct_canon_on(int i)
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
                tabCanon[i].eEtatLed   = CANON_LED_OFF ;
            }
            // on fait clignoter les led
            // c'est la fin du clignotement de la led qui fait arreter le canon
            switch( tabCanon[i].eEtatLed)
            {
              case CANON_LED_OFF :      fnct_canon_led_off(i);      break;
              case CANON_LED_ON  :      fnct_canon_led_on(i);       break;
              case CANON_LED_CLI_ON :   fnct_canon_led_on_cli(i);  break;
              case CANON_LED_CLI_OFF :  fnct_canon_led_off_cli(i);   break;
              default:                  break;
            }
}

/* ************************************************** */
/* ************************************************** */
void fnct_canon_led_off(int i)
{
  digitalWrite(tabCanon[i].pinLeD,LOW);
}

/* ************************************************** */
/* ************************************************** */
void fnct_canon_led_on(int i)
{
  digitalWrite(tabCanon[i].pinLeD,HIGH);
}

/* ************************************************** */
/* ************************************************** */
void fnct_canon_led_off_cli(int i)
{
    if ( tabCanon[i].tempoLed > 0) {
        tabCanon[i].tempoLed --;
    } else {
      digitalWrite(tabCanon[i].pinLeD,HIGH);
      tabCanon[i].eEtatLed = CANON_LED_CLI_ON ;
      tabCanon[i].tempoLed = MAX_TEMPO_LED;
    }
    
}

/* ************************************************** */
/* ************************************************** */
void fnct_canon_led_on_cli(int i)
{
    if ( tabCanon[i].tempoLed > 0) {
        tabCanon[i].tempoLed --;
    } else {
      digitalWrite(tabCanon[i].pinLeD,LOW);
      tabCanon[i].eEtatLed = CANON_LED_CLI_OFF ;
      tabCanon[i].tempoLed = MAX_TEMPO_LED;
    }
    
}

/* ************************************************** */
// CANON OFF
/* ************************************************** */
void fnct_canon_off(int i)
{
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

/* ************************************************** */
/* ************************************************** */
void TEST_LED_ET_RELAYS_CHENILLARD()
{
	static int cmp = TIMER_500MS; // 50x 10ms = 0.5s
	static int etat = 1;
	static int pos = 0;
  if (cmp-- == 0)
  {
	  for (uint8_t i=0; i<NB_CANON; i++)   {
		  digitalWrite(tabCanon[i].pinLeD,LOW);
		  digitalWrite(tabCanon[i].pinRelayCanon,LOW);
	  }
		digitalWrite(tabCanon[pos].pinLeD,HIGH);  digitalWrite(tabCanon[pos].pinRelayCanon,HIGH); 
		pos++;
		if (pos == 8)
		{
			pos = 0;
		}
		// etat = 1-etat;
		cmp = TIMER_500MS;
	}
}
