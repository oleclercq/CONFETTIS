/*  CANON A CONFETIS 
    CARTE ATMEGA 1656
	loma.fr
*/

// ========================================================================
// DESFINIIONS
// ========================================================================
#define NB_CANON		8
#define MAX_TEMPO_RLY	900 // en nb de 10ms
#define MAX_TEMPO_LED	900	// en nb de 10ms

// ========================================================================
// DESFINIIONS DES TYPES      
// ========================================================================
typedef enum { CANON_WAIT, CANON_FEUX, CANON_DECOMPTE, CANON_VIDE  } ENUM_ETAT;
typedef struct 	{	ENUM_ETAT  eEtat;			// Etat en cours.
					uint8_t bpPin;			// Pin de l'arduino ou est branché le bouton poussoir	
					uint8_t pinLeD;			// pin de l'arduiuino ou est brancher la led du bouton
					uint8_t pinRelayCanon;
					uint16_t tempoRestanteLed;	// Nb de pas encore a effectuer pour éteindre la led
					uint16_t tempoRestanteRelay;
				} ST_CANON;
				
// ========================================================================
// VARIABLES GLOBALES
// ========================================================================
// ************************************************************
// DEFINITIONS DES VARAIBLES
//! init tableau des producteurs cablé pour un hard V2 	
ST_CANON tabCanon[NB_CANON] = 	{	{CANON_WAIT, 20, 50, 58, MAX_TEMPO_LED, MAX_TEMPO_RLY},
									{CANON_WAIT, 21, 51, 59, MAX_TEMPO_LED, MAX_TEMPO_RLY},
									{CANON_WAIT, 22, 52, 60, MAX_TEMPO_LED, MAX_TEMPO_RLY},
									{CANON_WAIT, 23, 53, 61, MAX_TEMPO_LED, MAX_TEMPO_RLY},
									{CANON_WAIT, 24, 54, 62, MAX_TEMPO_LED, MAX_TEMPO_RLY},
									{CANON_WAIT, 25, 55, 63, MAX_TEMPO_LED, MAX_TEMPO_RLY},
									{CANON_WAIT, 26, 56, 64, MAX_TEMPO_LED, MAX_TEMPO_RLY},
									{CANON_WAIT, 27, 57, 65, MAX_TEMPO_LED, MAX_TEMPO_RLY}		// pro->offsetK ne doit pas dépasser ni egal à ENTREENUMERIQUE 19
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
		pinMode(tabCanon[i].bpPin, 			INPUT_PULLUP);
		pinMode(tabCanon[i].pinLeD, 		OUTPUT);
		pinMode(tabCanon[i].pinRelayCanon,	OUTPUT);
		
	}

//  ================================================================================
//  PARAMETRAGE DU TIMER 1 16BIT
	cli();
	TCCR1A = 0;
	TCCR1B = ( 1<<WGM12) | (0<<CS12) | (1<<CS11) | (1<<CS10) ; 	// Prescaler / 64 ==> des PAS de 5ms
	TCCR1C = 0; 												// not forcing output compare
	TCNT1 = 0; 													// set timer counter initial value (16 bit value)
	
	OCR1A = 2500;												// Closer to one second than value above, 2500 pas de 4µs ca fait 10ms 
	TIMSK1 = 1 << OCIE1A; 										// Enable timer compare match 1A interrupt
	sei(); 														// Enable interrupts
}

/* ************************************************************************ */
/* PGM Principal, lecture en permanance les entrées
/* ************************************************************************ */
void loop()
{
}


/* ************************************************************************ */
/* ON RENTRE DANS CE TIMER TOUTES LES 10ms
/* ************************************************************************ */
ISR(TIMER1_COMPA_vect) // 16 bit timer 1 compare 1A match
{
	//Lecture des Inters
	for (uint8_t i=0; i<NB_CANON; i++)		{
		if ( !digitalRead(tabCanon[i].bpPin) == LOW)	// Lecture de toutes les entrées
		{
			// BT_Appuyé, on active le relay et la led
			tabCanon[i].eEtat = CANON_FEUX;
			digitalWrite(tabCanon[i].pinRelayCanon),HIGH)
		}
		
		
		
		
	}
	
	
	
}
	
