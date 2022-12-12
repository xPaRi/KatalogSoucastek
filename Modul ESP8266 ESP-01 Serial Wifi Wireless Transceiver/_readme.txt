ESP8266 ESP-01 Serial Wifi Wireless Transceiver Module - Arduino Compatible

Add WiFi capabilities to your Arduino or other microcontroller project
WiFi serial transceiver module, based on ESP8266 SoC
Interface using simple AT commands over serial/uart
Integrated TCP/IP protocol stack. 802.11 b/g/n
SDIO 1.1/2.0, SPI, UART, Wi-Fi Direct (P2P), soft-AP
---
ESP-01 WiFi modul s ESP8266, internet v�c�
WiFi modul s integrovanou ant�nou
Ovl�d�n� AT-p��kazy

popis pin� zde:
http://www.electrodragon.com/w/Wi07c#Pin_Wiring_.28V090.29
---
�esky: http://robodoupe.cz/2014/esp8266-internet-veci-prichazi/

V�tah:

ESP8266: Internet v�c� p�ich�z�
Napsal Dex, 29.10.2014

Jedn�m z fenom�n� posledn� doby je �The Internet of Things�, nej�ast�ji do 
�e�tiny p�ekl�dan� jako Internet v�c�. V podstat� jde o to, �e d�ky rozvoji 
modern�ch technologi� bude mo�n� ka�dou v�c p�ipojit p��mo do Internetu. P�ed 
deseti lety rozhodn� nebylo b�n� aby nap��klad televize byla vybavena WiFi 
p�ipojen�m. Dnes je to prakticky standard, pronikaj�c� do st�le ni���ch 
modelov�ch �ad jednotliv�ch v�robc�. T�m by to ale zdaleka nem�lo skon�it.


P�edstavte si dom�cnost plnou �v�c� p�ipojen�ch k Internetu. R�zn� �idla jako 
teplom�ry, vlhkom�ry, sp�na�e topen�, osv�tlen� a cokoliv co V�s napadne. 
V�echny tyto v�ci budou s V�mi a klidn� i spolu navz�jem komunikovat. Jednou z 
cest jak toho doc�lit je vybavit je WiFi modulem. Jist� si �eknete, �e to nen� 
nic a� tak nov�ho a budete m�t pravdu � nen�. Co je ale nov� je cena. P�ed p�r 
m�s�c� se na trh dostal modul postaven� okolo �ipu ESP8266. B�n� cena 
nap��klad na eBay je 5$ za kus v�etn� dopravn�ho. Dod�n� obvykle trv� cca 
m�s�c, n�kdy i d�le.

Tak �vod m�me za sebou a poj�me se v rychlosti pod�vat o co jde. N�sleduj�c� 
text vych�z� z m�ch dnes �erstv� z�skan�ch zku�enost�. Rozhodn� nejde o 
kompletn� n�vod pro za��te�n�ky :-)

Modul� postaven�ch na ESP8266 existuje v�ce druh�. J� m�m moduly ozna�ovan� 
jako ESP-01. Srovn�n� jednotliv�ch variant je nap��klad zde. Modul pracuje v 
tzv. 3V3 logick�ch �rovn�ch. S t�m je pot�eba po��tat. K b�n�mu Arduino ho 
jen tak nep�ipoj�te. Pro prvn� experimenty jsem pou�il obvod MAX3232, kter� 
konvertuje klasickou RS-232 na 3V3. Popis jednotliv�ch pin� modulu je 
nap��klad tady. Pokud jde o nap�jec� zdroj tak po��tejte s t�m, �e modul m��e 
kr�tkodob� odeb�rat proud a� 300 mA. J� to podcenil a v�e zpo��tku fungovalo 
norm�ln�. Jakmile jsem ale za�al opravdu pou��vat WiFi, za�alo to z�hadn� 
padat.

Modul se kompletn� ovl�d� �AT p��kazy� � pamatujete na modemy?! :-). Jejich 
asi nejlep�� popis jsem na�el tady (http://www.electrodragon.com/w/Wi07c#AT_Commands). 
Pro rychl� test spojen� pou�ijte t�eba 
AT+GMR, kter� vr�t� verzi firmware. Doporu�uji aktualizovat na V0.922, kter� 
v�etn� n�stroje pro aktualizaci st�hnete zde. P�ed vlastn� aktualizac� je 
pot�eba uzemnit pin GPIO 0 a restartovat modul (prost� mu vypnout a zapnout 
nap�jen�). Jakmile se firmware nahraje, nap�e program chybu p�i �opou�t�n� 
aktualiza�n�ho re�imu�. Nic se ned�je. Odpojte modul o nap�jen�, zru�te 
uzemn�n� GPIO 0 a zase ho zapn�te. Dejte si pozor na komunika�n� rychlost. 
P�edchoz� firmware je natvrdo nastaven na 115200 bps a nejde to zm�nit. Ten co 
jste nahr�li m� ale v�choz� rychlost 9600 bps a z�rove� jde rychlost zm�nit. 
Op�t pomoc� AT+GMR m��ete zkontrolovat jestli komunikace funguje a jestli m�te 
vy��� verzi firmware.

Je�t� pop�u p�r v�c�, kter� jsem v dokumentaci (zat�m?) nena�el. Pokud pomoc� 
p��kazu AT+CWMODE=3 nastav�te WiFi do re�imu Sta+AP, tak si modul jako AP 
nastav� IP adresu 192.168.4.1. Nep�i�el jsem na to jak to zm�nit. V principu 
to ni�emu nevad� pokud Va�e s� nepou��v� stejn� rozsah. To pak asi bude 
probl�m. M�te-li na dom�c� WiFi zapnut� �ifrov�n� (kdo ne?!) tak se jen pomoc� 
p��kazu AT+CWJAP nep�ipoj�te. Budete muset nejd��ve pomoc� jin�ho p��kazu 
nastavit podrobn�j�� parametry pro Va�i s�. Bude to tedy vypadat nap��klad 
takto AT+CWSAP=�mojewifi�,�mojeheslo�,11,wap2 kde 11 je kan�l a wap2 verze 
�ifrov�n�. Pak u� sta�� jen AT+CWJAP=�mojewifi�,�mojeheslo� a modul je 
p�ipojen. Snadno si to ov���te p��kazem AT+CIFSR, kter� vr�t� IP adresu modulu,
 na kterou m��ete zkusit ping z po��ta�e. Zat�m se domn�v�m, �e parametry 
zadan� p��kazem AT+CWSAP si modul pamatuje i po vypnut� a sma�ou se teprve 
p�ehr�n�m firmware. Pokud m�te doma lep�� WiFi Access Point, ur�it� se 
pod�vejte i do n�j. J� t�eba v m�m MikroTiku po aktualizaci firmware modulu 
zjistil, �e je sign�l siln�j�� a spojen� b�� rychleji.

Mysl�m, �e jako prvn� sezn�men� to sta��. Modul toho um� mnohem v�ce a je 
vid�t jak�m bou�liv�m v�vojem software okolo n�j proch�z�. U� je dokonce mo�n� 
programovat i p��mo ESP8266 pomoc� SDK. Budu r�d, kdy� se s v�sledky sv�ch 
pokus� pod�l�te bu� hned v diskuzi pod �l�nkem nebo na na�em f�ru 
(http://forum.robodoupe.cz/)