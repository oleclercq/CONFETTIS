/*  CANON A CONFETIS 
    CARTE ATMEGA 1656
*/
//#define _MODE_OLQ
// #define DEBUG_MODE
#define TIMER_050MS 10
#define TIMER_100MS 20
#define TIMER_150MS 30
#define TIMER_200MS 40	
#define TIMER_250MS 50
#define TIMER_500MS 100
#define TIMER_CHENILARD_MS	TIMER_050MS


// ========================================================================
// DESFINIIONS
// ========================================================================
#define NB_CANON		8
#define MAX_TEMPO_RLY	900 // en nb de 10ms
#define MAX_TEMPO_LED	50	// en nb de 10ms

// C'est define sont utiliés pour le chenilard du début
#define MODE_ALLER		0
#define MODE_RETOUR		1
#define MODE_REMPLIR	2


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
bool gChenillardDeDebut = true ;
#ifdef _MODE_OLQ
 #define PIN_BT_RESET 	32
 #define LED_ON			HIGH
 #define LED_OFF		LOW
 #define RELAY_ON		HIGH
 #define RELAY_OFF		LOW
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
 #define PIN_BT_RESET 	2
 #define LED_ON		HIGH
 #define LED_OFF	LOW
 #define RELAY_ON	LOW
 #define RELAY_OFF	HIGH
//                                                              /----------- Colonne A affectation Num des pin pour les Bt poussoirs    
//                                                              |   /------- Colonne B affectation Num des pin pour les LED    
//                                                              |   |   /--  Colonne C affectation Num des pin pour le relay
//                                                              A   B   C
ST_CANON tabCanon[NB_CANON] = 	{
                                    {CANON_OFF, CANON_LED_OFF, 23,  4, 22, MAX_TEMPO_LED, MAX_TEMPO_RLY},	// Channel 1
                                    {CANON_OFF, CANON_LED_OFF, 25,  5, 24, MAX_TEMPO_LED, MAX_TEMPO_RLY},   // Channel 2
                                    {CANON_OFF, CANON_LED_OFF, 27,  6, 26, MAX_TEMPO_LED, MAX_TEMPO_RLY},	// Channel 3
                                    {CANON_OFF, CANON_LED_OFF, 29,  7, 28, MAX_TEMPO_LED, MAX_TEMPO_RLY},	// Channel 4
                                    {CANON_OFF, CANON_LED_OFF, 31,  8, 30, MAX_TEMPO_LED, MAX_TEMPO_RLY},	// Channel 5
                                    {CANON_OFF, CANON_LED_OFF, 33,  9, 32, MAX_TEMPO_LED, MAX_TEMPO_RLY},	// Channel 6
                                    {CANON_OFF, CANON_LED_OFF, 35, 10, 34, MAX_TEMPO_LED, MAX_TEMPO_RLY},	// Channel 7
                                    {CANON_OFF, CANON_LED_OFF, 37, 11, 36, MAX_TEMPO_LED, MAX_TEMPO_RLY}	// Channel 8
								} ;
#endif


// ========================================================================
// PROTOTYPAGES DES FONCTIONS
// ========================================================================
void fnct_read_bt(int i) ;
void fnct_canon_on(int i) ;
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
		pinMode(tabCanon[i].bpPin, 			INPUT_PULLUP);
		pinMode(tabCanon[i].pinLeD, 		OUTPUT);
		pinMode(tabCanon[i].pinRelayCanon,	OUTPUT);

    digitalWrite(tabCanon[i].pinRelayCanon,RELAY_OFF);
    tabCanon[i].eEtatCanon = CANON_READY ;
    
    digitalWrite(tabCanon[i].pinLeD,LED_OFF);
    tabCanon[i].eEtatLed = CANON_LED_OFF ;  // Prochain état
	}
	pinMode(PIN_BT_RESET,INPUT_PULLUP); 

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
/* PGM Principal, lecture en permanance les entrees
/* ************************************************************************ */
void loop()
{
}

/* ************************************************************************ */
/* ON RENTRE DANS CE TIMER TOUTES LES 10ms
/* ************************************************************************ */
ISR(TIMER1_COMPA_vect) // 16 bit timer 1 compare 1A match
{
// SEULEMENT UNE SEULE FONCTION DOIT ETRE APPELE ICI
// LES TROIS PREMIERES, C EST POUR DU TEST
//	TEST_LED_ET_RELAYS_CHENILLARD();
//  TEST_LED_ET_RELAYS(); //==> OK
//  TEST_ENTREES();       //==> OK
	if (gChenillardDeDebut){
		PGM_CHENILLARD();
	}else{
		PGM_NORMAL();         //==> OK
	}
}




/* ************************************************** */
// PROGRAMME PRINCIPAL
/* ************************************************** */
void PGM_NORMAL()
{
      bool val;
      for (uint8_t i=0; i<NB_CANON; i++)		
      {
        switch ( tabCanon[i].eEtatCanon) {
          case CANON_READY :  fnct_read_bt(i);   break ;
          case CANON_ON  :    fnct_canon_on(i);  break ;
          case CANON_OFF :    break ;
          default :           break ;                                 
        }
      }

      // Lecture du Bt RESET
      if (!digitalRead(PIN_BT_RESET))
      {
        for (uint8_t i=0; i<NB_CANON; i++)    
        {
          // tous les connons passe en ready
          digitalWrite(tabCanon[i].pinRelayCanon,RELAY_OFF);
          tabCanon[i].eEtatCanon = CANON_READY;
          gChenillardDeDebut = true;
          //digitalWrite(tabCanon[i].pinLeD,LED_ON);
          //tabCanon[i].eEtatLed = CANON_LED_ON;
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
    digitalWrite(tabCanon[i].pinRelayCanon,RELAY_ON); // on active les cannons
    tabCanon[i].eEtatCanon = CANON_ON;

    digitalWrite(tabCanon[i].pinLeD,LED_ON); // on active les cannons
    tabCanon[i].eEtatLed = CANON_LED_CLI_ON;
  }
}

/* ************************************************** */
// LE CANON EST ON, on y passe plusieur fois avant de le passer a OFF
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
                digitalWrite(tabCanon[i].pinRelayCanon,RELAY_OFF); // on FERME.
                tabCanon[i].eEtatCanon = CANON_OFF; // Le passage a READY ne se fera que sur app du RESET
                tabCanon[i].tempoRelay = MAX_TEMPO_RLY ;
                tabCanon[i].eEtatLed   = CANON_LED_OFF ;
            }
            // on fait clignoter les led
            switch( tabCanon[i].eEtatLed)
            {
              case CANON_LED_OFF :      fnct_canon_led_off(i);      break;
              case CANON_LED_ON  :      fnct_canon_led_on(i);       break;
              case CANON_LED_CLI_ON :   fnct_canon_led_on_cli(i);  break;   // tempo pour la duree de la led on
              case CANON_LED_CLI_OFF :  fnct_canon_led_off_cli(i);   break; // tempo pour la duree de la led OFF
              default:                  break;
            }
}

/* ************************************************** */
/* ************************************************** */
void fnct_canon_led_off(int i)
{
  digitalWrite(tabCanon[i].pinLeD,LED_OFF);
}

/* ************************************************** */
/* ************************************************** */
void fnct_canon_led_on(int i)
{
  digitalWrite(tabCanon[i].pinLeD,LED_ON);
}

/* ************************************************** */
/* ************************************************** */
void fnct_canon_led_off_cli(int i)
{
    if ( tabCanon[i].tempoLed > 0) {
        tabCanon[i].tempoLed --;
    } else {
      digitalWrite(tabCanon[i].pinLeD,LED_ON);
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
	Serial.print("bp_");
	Serial.print(i);
	Serial.print("(");
	Serial.print(tabCanon[i].bpPin);
	Serial.print(")=");
	Serial.print(val);
	Serial.print("   ");
  }
  Serial.println("");
}

/* ************************************************** */
/* ************************************************** */
void TEST_LED_ET_RELAYS()
{
// /* TEST DES LEDS et DES  RELAYS ==> OK 
  static int cmp = TIMER_500MS; // 50x 10ms = 0.5s
  static int etat = 4;
	if (cmp-- == 0)
	{ 
		for (uint8_t i=0; i<NB_CANON; i++)   
		{
			//Serial.print(tabCanon[i].pinRelayCanon);
			//Serial.print(" ");
			if(etat <= 0)
			{
				digitalWrite(tabCanon[i].pinLeD,LED_OFF);
				digitalWrite(tabCanon[i].pinRelayCanon,RELAY_ON);
				
			} else {
				digitalWrite(tabCanon[i].pinLeD,LED_ON);
				digitalWrite(tabCanon[i].pinRelayCanon,RELAY_OFF);
			}
		}
		
		if(etat <= 0) {
 			etat = 4 ;
		} else { 
			etat--; 
		}
		cmp = TIMER_500MS;
		//Serial.println("");
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
		  digitalWrite(tabCanon[i].pinLeD,LED_OFF);
		  digitalWrite(tabCanon[i].pinRelayCanon,RELAY_OFF);
	  }
		digitalWrite(tabCanon[pos].pinLeD,LED_ON);  digitalWrite(tabCanon[pos].pinRelayCanon,RELAY_ON); 
		pos++;
		if (pos == 8)
		{
			pos = 0;
		}
		// etat = 1-etat;
		cmp = TIMER_500MS;
	}
}

/* ************************************************** */
// FAIT UN CHENILARD de Décoration pour remplir les LED 
// cette fonction est appelée par le timer toutes les 10ms
// on temporise pour faire un changement de led toutes les 250ms
/* ************************************************** */
void PGM_CHENILLARD()
{
	static int cmp = TIMER_CHENILARD_MS; // 50x 10ms = 0.5s
	static int pos = 0;
	static int mode = MODE_ALLER;
	if (cmp-- == 0)
	{
		// MODE_ALLER
		if (mode == MODE_ALLER)
		{
			for (uint8_t i=0; i<NB_CANON; i++)   {
				digitalWrite(tabCanon[i].pinLeD,LED_OFF);
			}
			digitalWrite(tabCanon[pos].pinLeD,LED_ON); 
			pos++;
			if (pos == 8)
			{
				pos	= 6;
				mode = MODE_RETOUR;
			}
		}
		
		// MODE_RETOUR
		else if (mode == MODE_RETOUR)
		{
			for (uint8_t i=0; i<NB_CANON; i++)   {
				digitalWrite(tabCanon[i].pinLeD,LED_OFF);
			}
			digitalWrite(tabCanon[pos].pinLeD,LED_ON); 
			if (pos-- == 0)
			{
				pos	= 1;
				mode = MODE_REMPLIR;
			}
		}
	
		// MODE_REMPLIR
		else //  if (mode == MODE_REMPLIR)
		{
			digitalWrite(tabCanon[pos].pinLeD,LED_ON); 
			
			pos++;
			if(pos == 8)
			{
				for (uint8_t i=0; i<NB_CANON; i++)   {
					tabCanon[pos].eEtatLed = CANON_LED_ON;
				}
				pos	= 0;
				mode = MODE_ALLER;
				gChenillardDeDebut = false ;		// le chenillard de début est terminé
			}
		}
		cmp = TIMER_CHENILARD_MS;
	}
	
	
}

