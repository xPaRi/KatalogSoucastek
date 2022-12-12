Infrared Sensor LED020

Infrared sensor (infra red sensor- infra-red sensor- IR sensor). For use with 
PICAXE circuits to detect infra-red signals from a TV style remote control or 
another PICAXE chip. 2.7V to 5.5V operation.

PICAXE code
----------------------------
SYMBOL IR_PIN  = pin0
SYMBOL LED_PIN = 0
symbol infra = b14

HIGH LED_PIN
pause 250
LOW LED_PIN

WaitForSignal:

irin 0, infra
debug infra
HIGH LED_PIN
PAUSE 250
LOW LED_PIN

GOTO WaitForSignal
----------------------------


PICAXE code ---------------------------- 
;Read an infra-red key press code 
;This program will receive key presses from an infra-red remote control using 
;an infra-red receiver connected to input pin C.0 and will display all key 
;press codes in the 'b1' variable. The infra-red remote control must be set to 
;produce Sony TV infra-red command codes.

;Infra-red sensor LED020
;4k7 resistor
;4.7uF electrolytic capacitor

;Code Example:
main:	
  irin C.0, b1; Wait for a key press code
  debug; Display the value
  goto main; Repeat
