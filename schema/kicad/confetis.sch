EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:arduino
LIBS:valves
LIBS:Switch
LIBS:confetis-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Arduino_Mega2560_Shield XA?
U 1 1 5E6D5F2F
P 4250 3800
F 0 "XA?" V 4350 3800 60  0000 C CNN
F 1 "Arduino_Mega2560_Shield" V 4150 3800 60  0000 C CNN
F 2 "" H 4950 6550 60  0001 C CNN
F 3 "" H 4950 6550 60  0001 C CNN
	1    4250 3800
	1    0    0    -1  
$EndComp
$Comp
L +9V #PWR?
U 1 1 5E6D5FD5
P 2200 5900
F 0 "#PWR?" H 2200 5750 50  0001 C CNN
F 1 "+9V" H 2200 6040 50  0000 C CNN
F 2 "" H 2200 5900 50  0000 C CNN
F 3 "" H 2200 5900 50  0000 C CNN
	1    2200 5900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5E6D6015
P 2400 5500
F 0 "#PWR?" H 2400 5250 50  0001 C CNN
F 1 "GND" H 2400 5350 50  0000 C CNN
F 2 "" H 2400 5500 50  0000 C CNN
F 3 "" H 2400 5500 50  0000 C CNN
	1    2400 5500
	1    0    0    -1  
$EndComp
Text Notes 1850 6150 0    60   ~ 0
Alimentation 9V par la prise \nd'almimentation de la carte
$Comp
L SW_Push_LED SW?
U 1 1 5E6D631A
P 7200 4050
F 0 "SW?" H 7225 4275 50  0000 L CNN
F 1 "SW_Push_LED" H 7200 3925 50  0000 C CNN
F 2 "" H 7200 4350 50  0001 C CNN
F 3 "" H 7200 4350 50  0001 C CNN
	1    7200 4050
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 5E6D6375
P 6700 3750
F 0 "R?" V 6780 3750 50  0000 C CNN
F 1 "10k" V 6700 3750 50  0000 C CNN
F 2 "" V 6630 3750 50  0001 C CNN
F 3 "" H 6700 3750 50  0001 C CNN
	1    6700 3750
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 5E6D63DE
P 6700 3500
F 0 "#PWR?" H 6700 3350 50  0001 C CNN
F 1 "+5V" H 6700 3640 50  0000 C CNN
F 2 "" H 6700 3500 50  0001 C CNN
F 3 "" H 6700 3500 50  0001 C CNN
	1    6700 3500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5E6D641B
P 7700 4000
F 0 "#PWR?" H 7700 3750 50  0001 C CNN
F 1 "GND" H 7700 3850 50  0000 C CNN
F 2 "" H 7700 4000 50  0001 C CNN
F 3 "" H 7700 4000 50  0001 C CNN
	1    7700 4000
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 5E6D6459
P 7500 3500
F 0 "#PWR?" H 7500 3350 50  0001 C CNN
F 1 "+5V" H 7500 3640 50  0000 C CNN
F 2 "" H 7500 3500 50  0001 C CNN
F 3 "" H 7500 3500 50  0001 C CNN
	1    7500 3500
	1    0    0    -1  
$EndComp
Text Notes 6500 3250 0    60   ~ 0
x8
$Comp
L R R?
U 1 1 5E6D66A3
P 6700 4300
F 0 "R?" V 6780 4300 50  0000 C CNN
F 1 "470" V 6700 4300 50  0000 C CNN
F 2 "" V 6630 4300 50  0001 C CNN
F 3 "" H 6700 4300 50  0001 C CNN
	1    6700 4300
	1    0    0    -1  
$EndComp
Text GLabel 7000 1650 2    60   Output ~ 0
RELAIS_1
Text Notes 6950 1550 0    60   ~ 0
Vers la carte Relais x8
Text GLabel 7000 1750 2    60   Output ~ 0
RELAIS_2
Text GLabel 7000 1850 2    60   Output ~ 0
RELAIS_3
Text GLabel 7000 1950 2    60   Output ~ 0
RELAIS_4
Text GLabel 7000 2050 2    60   Output ~ 0
RELAIS_5
Text GLabel 7000 2150 2    60   Output ~ 0
RELAIS_6
Text GLabel 7000 2250 2    60   Output ~ 0
RELAIS_7
$Comp
L GND #PWR?
U 1 1 5E6D6A0B
P 6950 2600
F 0 "#PWR?" H 6950 2350 50  0001 C CNN
F 1 "GND" H 6950 2450 50  0000 C CNN
F 2 "" H 6950 2600 50  0000 C CNN
F 3 "" H 6950 2600 50  0000 C CNN
	1    6950 2600
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 5E6D6A6D
P 6950 2500
F 0 "#PWR?" H 6950 2350 50  0001 C CNN
F 1 "+5V" H 6950 2640 50  0000 C CNN
F 2 "" H 6950 2500 50  0001 C CNN
F 3 "" H 6950 2500 50  0001 C CNN
	1    6950 2500
	1    0    0    -1  
$EndComp
Text Notes 7600 2600 0    60   ~ 0
Alimentation de\n la carte Relais
Entry Wire Line
	6250 4650 6350 4750
Entry Wire Line
	6250 4550 6350 4650
Entry Wire Line
	6250 4450 6350 4550
Entry Wire Line
	6250 4350 6350 4450
Entry Wire Line
	6250 4250 6350 4350
Entry Wire Line
	6250 4150 6350 4250
Entry Wire Line
	6250 4050 6350 4150
Entry Wire Line
	6250 3950 6350 4050
Wire Wire Line
	2400 5500 2400 5450
Wire Wire Line
	2400 5450 2950 5450
Wire Wire Line
	2950 5950 2200 5950
Wire Wire Line
	2200 5950 2200 5900
Wire Wire Line
	6700 3600 6700 3500
Wire Wire Line
	6700 3900 6700 3950
Connection ~ 6700 3950
Wire Wire Line
	7400 3950 7700 3950
Wire Wire Line
	7700 3950 7700 4000
Wire Wire Line
	7400 4050 7500 4050
Wire Wire Line
	7500 4050 7500 3500
Wire Wire Line
	5550 5950 6700 5950
Wire Wire Line
	6700 4050 7000 4050
Wire Wire Line
	5550 1650 7000 1650
Wire Notes Line
	6500 3250 6500 4450
Wire Notes Line
	6500 4450 7950 4450
Wire Notes Line
	7950 4450 7950 3250
Wire Notes Line
	7950 3250 6500 3250
Wire Wire Line
	6700 4150 6700 4050
Wire Wire Line
	6700 5950 6700 4450
Wire Wire Line
	5550 1750 7000 1750
Wire Wire Line
	7000 1850 5550 1850
Wire Wire Line
	7000 1950 5550 1950
Wire Wire Line
	7000 2050 5550 2050
Wire Wire Line
	7000 2150 5550 2150
Wire Wire Line
	7000 2250 5550 2250
Wire Wire Line
	6950 2500 7450 2500
Wire Wire Line
	6950 2600 7450 2600
Wire Notes Line
	6750 1450 6750 2850
Wire Notes Line
	6750 2850 8300 2850
Wire Notes Line
	8300 2850 8300 1450
Wire Notes Line
	8300 1450 6750 1450
Wire Wire Line
	5550 4050 6250 4050
Wire Wire Line
	5550 4150 6250 4150
Wire Wire Line
	5550 4250 6250 4250
Wire Wire Line
	5550 4350 6250 4350
Wire Wire Line
	6250 4450 5550 4450
Wire Wire Line
	5550 4550 6250 4550
Wire Wire Line
	5550 4650 6250 4650
Wire Bus Line
	6350 4750 6350 4050
Wire Wire Line
	5550 3950 6250 3950
Wire Wire Line
	6350 4050 6350 3950
Wire Wire Line
	6350 3950 7000 3950
Wire Wire Line
	5550 2450 5850 2450
Wire Wire Line
	5550 2550 5850 2550
Wire Wire Line
	5550 2650 5850 2650
Wire Wire Line
	5550 2750 5850 2750
Entry Wire Line
	5850 2450 5950 2550
Entry Wire Line
	5850 2550 5950 2650
Entry Wire Line
	5850 2650 5950 2750
Entry Wire Line
	5850 2750 5950 2850
Wire Bus Line
	5950 2550 5950 6050
Entry Wire Line
	5850 5950 5950 6050
Entry Wire Line
	5850 5050 5950 5150
Entry Wire Line
	5850 5150 5950 5250
Entry Wire Line
	5850 5250 5950 5350
Wire Wire Line
	5850 5050 5550 5050
Wire Wire Line
	5550 5150 5850 5150
Wire Wire Line
	5850 5250 5550 5250
Text Notes 6750 3650 0    60   ~ 0
R facultatif
Text Notes 5800 1350 0    60   ~ 0
Pilotage des relais
Text Label 5650 1650 0    60   ~ 0
RL1
Text Label 5650 1750 0    60   ~ 0
RL2
Text Label 5650 1850 0    60   ~ 0
RL3
Text Label 5650 1950 0    60   ~ 0
RL4
Text Label 5650 2050 0    60   ~ 0
RL5
Text Label 5650 2150 0    60   ~ 0
RL6
Text Label 5650 2250 0    60   ~ 0
RL7
Text Label 5650 3950 0    60   ~ 0
IN_1
Text Label 5650 4050 0    60   ~ 0
IN_2
Text Label 5650 4150 0    60   ~ 0
IN_3
Text Label 5650 4250 0    60   ~ 0
IN_4
Text Label 5650 4350 0    60   ~ 0
IN_5
Text Label 5650 4450 0    60   ~ 0
IN_6
Text Label 5650 4550 0    60   ~ 0
IN_7
Text Label 5650 4650 0    60   ~ 0
IN_8
Text Label 5600 5950 0    60   ~ 0
LED_1
Text Label 5600 5250 0    60   ~ 0
LED_2
Text Label 5600 5150 0    60   ~ 0
LED_3
Text Label 5600 5050 0    60   ~ 0
LED_4
Text Label 5650 2450 0    60   ~ 0
LED_5
Text Label 5650 2550 0    60   ~ 0
LED_6
Text Label 5650 2650 0    60   ~ 0
LED_7
Text Label 5650 2750 0    60   ~ 0
LED_8
$EndSCHEMATC
