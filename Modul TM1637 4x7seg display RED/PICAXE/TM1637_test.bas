'******************************************************************************
'TM1637
'
'Test modulu TM1637 a knihovny TM1637.basinc
'******************************************************************************

'--- Definice pro TM1637.basinc
symbol clk = C.2 'pin s hodinami
symbol dio = C.1 'I/O pin
symbol pinDio = pinC.1

symbol ledData = b0		'data, která budou přesunuta do zobrazovače
symbol charIndex = b1	'index znaku, který je momentálně přesouván do zobrazovače
symbol bitIndex = b2		'index čítače jednotlivých bitů
symbol numIndex = b3		'index čítače jednotlivých číslic
symbol bright = b10		'proměnná pro nastavení jasu
symbol isColon = b11		'indikuje, že se bude (1) / nebude (0) zobrazovat dvojtečka za druhou číslicí
symbol value = w2			'zobrazované 4 místné číslo
symbol divTmp = w3		'dočasná proměnná pro děličku deseti
symbol valueTmp = w4		'dočasná proměnná pro zobrazení

pullup %1100 'nohodit pullup na C.1 a C.2
'---

'--- Tady je nějaký prográmenk, který to celé otestuje
'Na zobrazovači poběží čítač od 9999 do 0,
'pak se to celé resetuje a pojede to znova

bright = 1 : gosub SetBright 'nastavit jas na minimum

for value = 9999 to 0 step -1
	isColon = value % 10 'u každé desáté blikne dvojtečka
	gosub ShowValue
next
'---

reset

#include "TM1637.basinc" 'knihovna pro TM1637
