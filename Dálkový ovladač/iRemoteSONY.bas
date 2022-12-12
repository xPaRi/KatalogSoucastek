;ZDROJ:
;http://galia.fc.uaslp.mx/~cantocar/microcontroladores/PICAXE/PICAXE_INFRA_RED_INTERFACIN.HTM

SYMBOL LED_PIN    = C.0 'signalizační dioda (abychom věděli, že jsme něco zmáčkli)
SYMBOL IR_INPUT   = C.6 'Pin Vout čidla 'Infrared Sensor VS 1838 B'

'---
;Odladěno na 4 MHz, ale nefuguje zcela dobře
;SETFREQ M4
;
;SYMBOL MIN_PULSIN = 200 '130/200 (s tímto se hýbe, pokud je vracený kód občas chybný)
;SYMBOL PAUSE_TIME = 20  '50/20
;SYMBOL BIT_TIME   = 80 '160/80

;Odladěno na 8 MHz
SETFREQ M8
SYMBOL MIN_PULSIN = 130 's tímto se hýbe, pokud je vracený kód občas chybný
SYMBOL PAUSE_TIME = 50
SYMBOL BIT_TIME   = 160

'---

do
	GOSUB BruteForceInfraIn3
	
	high LED_PIN
	pause 300 'vysílač kód zpravidla několikrát opakuje, pauza toto opakování 'zařízne'
	low LED_PIN
 
Loop

end

BruteForceInfraIn3:
	PULSIN IR_INPUT, 0, b1
	
	IF b1 < MIN_PULSIN THEN BruteForceInfraIn3

	PAUSE PAUSE_TIME
	
	PULSIN IR_INPUT, 0, b2 'D0
	PULSIN IR_INPUT, 0, b3
	PULSIN IR_INPUT, 0, b4
	PULSIN IR_INPUT, 0, b5
	PULSIN IR_INPUT, 0, b6
	PULSIN IR_INPUT, 0, b7
	PULSIN IR_INPUT, 0, b8 'D6

	PULSIN IR_INPUT, 0, b9 'C0
	PULSIN IR_INPUT, 0, b10
	PULSIN IR_INPUT, 0, b11
	PULSIN IR_INPUT, 0, b12
	PULSIN IR_INPUT, 0, b13 'C4

	b0   = b0 / BIT_TIME
	bit1 = b1 / BIT_TIME
	bit2 = b2 / BIT_TIME
	bit3 = b3 / BIT_TIME
	bit4 = b4 / BIT_TIME
	bit5 = b5 / BIT_TIME
	bit6 = b6 / BIT_TIME
	bit7 = b7 / BIT_TIME

	b26=b0

	b0   = b9 / BIT_TIME
	bit1 = b10 / BIT_TIME
	bit2 = b11 / BIT_TIME
	bit3 = b12 / BIT_TIME
	bit4 = b13 / BIT_TIME

	b27=b0
	
	'v b0 je kód stisknutého tlačítka
	'sertxd (b0, b1, b2, b3, b4, b5, b6, b7)
	'sertxd (b8, b9, b10,b11,b12,b13, 0,  0)

	debug
RETURN