EESchema Schematic File Version 4
LIBS:SX1503-Breakout-cache
EELAYER 26 0
EELAYER END
$Descr User 6299 5512
encoding utf-8
Sheet 1 1
Title "SX1503 Breakout"
Date "2019-11-02"
Rev "1"
Comp "Manuvr, inc"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Label 1600 2750 2    39   ~ 0
SCL
Text Label 1600 2650 2    39   ~ 0
SDA
$Comp
L Device:C C2
U 1 1 5D1C4E71
P 3550 2700
F 0 "C2" H 3550 2800 39  0000 L CNN
F 1 "1uF" H 3550 2600 39  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3588 2550 50  0001 C CNN
F 3 "~" H 3550 2700 50  0001 C CNN
F 4 "490-1320-1-ND" H 3550 2700 50  0001 C CNN "Digikey_Num"
	1    3550 2700
	1    0    0    -1  
$EndComp
Text Label 2150 1200 1    39   ~ 0
Vcc
Text Label 2550 3000 0    39   ~ 0
GND
Text Label 3550 2900 3    39   ~ 0
GND
Wire Wire Line
	3550 2900 3550 2850
Wire Wire Line
	3550 2550 3550 2500
Text Label 1600 2550 2    39   ~ 0
~RESET
$Comp
L sx1503:SX1503 U1
U 1 1 5DBD2C36
P 2150 2000
F 0 "U1" H 2250 1550 50  0000 C CNN
F 1 "SX1503" V 2150 2000 50  0000 C CNN
F 2 "Package_DFN_QFN:QFN-28-1EP_4x4mm_P0.4mm_EP2.4x2.4mm" H 2150 2000 50  0001 C CNN
F 3 "" H 2150 2000 50  0001 C CNN
	1    2150 2000
	1    0    0    -1  
$EndComp
Text Label 1600 2450 2    39   ~ 0
~INT
Wire Wire Line
	2200 2900 2200 3000
Wire Wire Line
	2200 3000 2300 3000
Wire Wire Line
	2300 2900 2300 3000
Connection ~ 2300 3000
Wire Wire Line
	2300 3000 2400 3000
Wire Wire Line
	2400 2900 2400 3000
Connection ~ 2400 3000
Wire Wire Line
	2400 3000 2500 3000
Wire Wire Line
	2500 2900 2500 3000
Connection ~ 2500 3000
Wire Wire Line
	2500 3000 2550 3000
Wire Wire Line
	2150 1250 2150 1200
Wire Wire Line
	1600 2650 1650 2650
Wire Wire Line
	1600 2750 1650 2750
Wire Wire Line
	1600 2450 1650 2450
Wire Wire Line
	1600 2550 1650 2550
$Comp
L Device:C C1
U 1 1 5DBD36F1
P 3050 2700
F 0 "C1" H 3050 2800 39  0000 L CNN
F 1 "10uF" H 3050 2600 39  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3088 2550 50  0001 C CNN
F 3 "~" H 3050 2700 50  0001 C CNN
	1    3050 2700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C3
U 1 1 5DBD3759
P 3300 2700
F 0 "C3" H 3300 2800 39  0000 L CNN
F 1 "10uF" H 3300 2600 39  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3338 2550 50  0001 C CNN
F 3 "~" H 3300 2700 50  0001 C CNN
	1    3300 2700
	1    0    0    -1  
$EndComp
Text Label 3550 2500 1    39   ~ 0
Vcc
Text Label 3300 2900 3    39   ~ 0
GND
Text Label 3050 2900 3    39   ~ 0
GND
Wire Wire Line
	3050 2850 3050 2900
Wire Wire Line
	3300 2900 3300 2850
Text Label 2700 1450 0    39   ~ 0
Vcc2
Text Label 1600 1450 2    39   ~ 0
Vcc1
Text Label 3900 2450 2    39   ~ 0
SCL
Text Label 3900 2350 2    39   ~ 0
SDA
Text Label 4650 2350 0    39   ~ 0
~RESET
Text Label 4650 2450 0    39   ~ 0
~INT
Text Label 3300 2500 1    39   ~ 0
Vcc2
Text Label 3050 2500 1    39   ~ 0
Vcc1
Wire Wire Line
	3050 2500 3050 2550
Wire Wire Line
	3300 2500 3300 2550
Wire Wire Line
	2100 2900 2100 3000
Wire Wire Line
	2100 3000 2200 3000
Connection ~ 2200 3000
Text Label 1600 1550 2    39   ~ 0
IO-0
Text Label 1600 1650 2    39   ~ 0
IO-1
Text Label 1600 1750 2    39   ~ 0
IO-2
Text Label 1600 1850 2    39   ~ 0
IO-3
Text Label 1600 1950 2    39   ~ 0
IO-4
Text Label 1600 2050 2    39   ~ 0
IO-5
Text Label 1600 2150 2    39   ~ 0
IO-6
Text Label 1600 2250 2    39   ~ 0
IO-7
Text Label 2700 1550 0    39   ~ 0
IO-8
Text Label 2700 1650 0    39   ~ 0
IO-9
Text Label 2700 1750 0    39   ~ 0
IO-10
Text Label 2700 1850 0    39   ~ 0
IO-11
Text Label 2700 1950 0    39   ~ 0
IO-12
Text Label 2700 2050 0    39   ~ 0
IO-13
Text Label 2700 2150 0    39   ~ 0
IO-14
Text Label 2700 2250 0    39   ~ 0
IO-15
Text Label 4650 2250 0    39   ~ 0
Vcc2
Text Label 4650 1450 0    39   ~ 0
IO-8
Text Label 4650 1550 0    39   ~ 0
IO-9
Text Label 4650 1650 0    39   ~ 0
IO-10
Text Label 4650 1750 0    39   ~ 0
IO-11
Text Label 4650 1850 0    39   ~ 0
IO-12
Text Label 4650 1950 0    39   ~ 0
IO-13
Text Label 4650 2050 0    39   ~ 0
IO-14
Text Label 4650 2150 0    39   ~ 0
IO-15
Text Label 3900 2250 2    39   ~ 0
Vcc1
Text Label 3900 1450 2    39   ~ 0
IO-0
Text Label 3900 1550 2    39   ~ 0
IO-1
Text Label 3900 1650 2    39   ~ 0
IO-2
Text Label 3900 1750 2    39   ~ 0
IO-3
Text Label 3900 1850 2    39   ~ 0
IO-4
Text Label 3900 1950 2    39   ~ 0
IO-5
Text Label 3900 2050 2    39   ~ 0
IO-6
Text Label 3900 2150 2    39   ~ 0
IO-7
Wire Wire Line
	1600 1450 1650 1450
Wire Wire Line
	1600 1550 1650 1550
Wire Wire Line
	1600 1650 1650 1650
Wire Wire Line
	1600 1750 1650 1750
Wire Wire Line
	1600 1850 1650 1850
Wire Wire Line
	1600 1950 1650 1950
Wire Wire Line
	1600 2050 1650 2050
Wire Wire Line
	1600 2150 1650 2150
Wire Wire Line
	1600 2250 1650 2250
Wire Wire Line
	2650 1450 2700 1450
Wire Wire Line
	2650 1550 2700 1550
Wire Wire Line
	2650 1650 2700 1650
Wire Wire Line
	2650 1750 2700 1750
Wire Wire Line
	2650 1850 2700 1850
Wire Wire Line
	2650 1950 2700 1950
Wire Wire Line
	2650 2050 2700 2050
Wire Wire Line
	2650 2150 2700 2150
Wire Wire Line
	2650 2250 2700 2250
Text Label 3900 2550 2    39   ~ 0
Vcc
Text Label 4650 2550 0    39   ~ 0
GND
$Comp
L Connector:Conn_01x12_Male J1
U 1 1 5DBD7F26
P 4150 2050
F 0 "J1" V 4100 2550 50  0000 R CNN
F 1 "Conn_01x12_Male" V 4100 2100 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x12_P2.54mm_Vertical" H 4150 2050 50  0001 C CNN
F 3 "~" H 4150 2050 50  0001 C CNN
	1    4150 2050
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x12_Male J2
U 1 1 5DBD7FA4
P 4400 1950
F 0 "J2" V 4350 1400 50  0000 C CNN
F 1 "Conn_01x12_Male" V 4350 2150 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x12_P2.54mm_Vertical" H 4400 1950 50  0001 C CNN
F 3 "~" H 4400 1950 50  0001 C CNN
	1    4400 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 1450 3950 1450
Wire Wire Line
	3900 1550 3950 1550
Wire Wire Line
	3900 1650 3950 1650
Wire Wire Line
	3900 1750 3950 1750
Wire Wire Line
	3900 1850 3950 1850
Wire Wire Line
	3900 1950 3950 1950
Wire Wire Line
	3900 2050 3950 2050
Wire Wire Line
	3900 2150 3950 2150
Wire Wire Line
	3900 2250 3950 2250
Wire Wire Line
	3900 2350 3950 2350
Wire Wire Line
	3900 2450 3950 2450
Wire Wire Line
	3900 2550 3950 2550
Wire Wire Line
	4600 1450 4650 1450
Wire Wire Line
	4600 1550 4650 1550
Wire Wire Line
	4600 1650 4650 1650
Wire Wire Line
	4600 1750 4650 1750
Wire Wire Line
	4600 1850 4650 1850
Wire Wire Line
	4600 1950 4650 1950
Wire Wire Line
	4600 2050 4650 2050
Wire Wire Line
	4600 2150 4650 2150
Wire Wire Line
	4600 2250 4650 2250
Wire Wire Line
	4600 2350 4650 2350
Wire Wire Line
	4600 2450 4650 2450
Wire Wire Line
	4600 2550 4650 2550
$EndSCHEMATC
