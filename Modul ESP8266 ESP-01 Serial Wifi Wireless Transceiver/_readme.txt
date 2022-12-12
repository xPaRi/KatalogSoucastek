ESP8266 ESP-01 Serial Wifi Wireless Transceiver Module - Arduino Compatible

Add WiFi capabilities to your Arduino or other microcontroller project
WiFi serial transceiver module, based on ESP8266 SoC
Interface using simple AT commands over serial/uart
Integrated TCP/IP protocol stack. 802.11 b/g/n
SDIO 1.1/2.0, SPI, UART, Wi-Fi Direct (P2P), soft-AP
---
ESP-01 WiFi modul s ESP8266, internet vìcí
WiFi modul s integrovanou anténou
Ovládání AT-pøíkazy

popis pinù zde:
http://www.electrodragon.com/w/Wi07c#Pin_Wiring_.28V090.29
---
èesky: http://robodoupe.cz/2014/esp8266-internet-veci-prichazi/

Vıtah:

ESP8266: Internet vìcí pøichází…
Napsal Dex, 29.10.2014

Jedním z fenoménù poslední doby je „The Internet of Things“, nejèastìji do 
èeštiny pøekládanı jako Internet vìcí. V podstatì jde o to, e díky rozvoji 
moderních technologií bude moné kadou vìc pøipojit pøímo do Internetu. Pøed 
deseti lety rozhodnì nebylo bìné aby napøíklad televize byla vybavena WiFi 
pøipojením. Dnes je to prakticky standard, pronikající do stále niších 
modelovıch øad jednotlivıch vırobcù. Tím by to ale zdaleka nemìlo skonèit.


Pøedstavte si domácnost plnou „vìcí“ pøipojenıch k Internetu. Rùzná èidla jako 
teplomìry, vlhkomìry, spínaèe topení, osvìtlení a cokoliv co Vás napadne. 
Všechny tyto vìci budou s Vámi a klidnì i spolu navzájem komunikovat. Jednou z 
cest jak toho docílit je vybavit je WiFi modulem. Jistì si øeknete, e to není 
nic a tak nového a budete mít pravdu – není. Co je ale nové je cena. Pøed pár 
mìsící se na trh dostal modul postavenı okolo èipu ESP8266. Bìná cena 
napøíklad na eBay je 5$ za kus vèetnì dopravného. Dodání obvykle trvá cca 
mìsíc, nìkdy i déle.

Tak úvod máme za sebou a pojïme se v rychlosti podívat o co jde. Následující 
text vychází z mıch dnes èerstvì získanıch zkušeností. Rozhodnì nejde o 
kompletní návod pro zaèáteèníky :-)

Modulù postavenıch na ESP8266 existuje více druhù. Já mám moduly oznaèované 
jako ESP-01. Srovnání jednotlivıch variant je napøíklad zde. Modul pracuje v 
tzv. 3V3 logickıch úrovních. S tím je potøeba poèítat. K bìnému Arduino ho 
jen tak nepøipojíte. Pro první experimenty jsem pouil obvod MAX3232, kterı 
konvertuje klasickou RS-232 na 3V3. Popis jednotlivıch pinù modulu je 
napøíklad tady. Pokud jde o napájecí zdroj tak poèítejte s tím, e modul mùe 
krátkodobì odebírat proud a 300 mA. Já to podcenil a vše zpoèátku fungovalo 
normálnì. Jakmile jsem ale zaèal opravdu pouívat WiFi, zaèalo to záhadnì 
padat.

Modul se kompletnì ovládá „AT pøíkazy“ – pamatujete na modemy?! :-). Jejich 
asi nejlepší popis jsem našel tady (http://www.electrodragon.com/w/Wi07c#AT_Commands). 
Pro rychlı test spojení pouijte tøeba 
AT+GMR, kterı vrátí verzi firmware. Doporuèuji aktualizovat na V0.922, kterı 
vèetnì nástroje pro aktualizaci stáhnete zde. Pøed vlastní aktualizací je 
potøeba uzemnit pin GPIO 0 a restartovat modul (prostì mu vypnout a zapnout 
napájení). Jakmile se firmware nahraje, napíše program chybu pøi „opouštìní 
aktualizaèního reimu“. Nic se nedìje. Odpojte modul o napájení, zrušte 
uzemnìní GPIO 0 a zase ho zapnìte. Dejte si pozor na komunikaèní rychlost. 
Pøedchozí firmware je natvrdo nastaven na 115200 bps a nejde to zmìnit. Ten co 
jste nahráli má ale vıchozí rychlost 9600 bps a zároveò jde rychlost zmìnit. 
Opìt pomocí AT+GMR mùete zkontrolovat jestli komunikace funguje a jestli máte 
vyšší verzi firmware.

Ještì popíšu pár vìcí, které jsem v dokumentaci (zatím?) nenašel. Pokud pomocí 
pøíkazu AT+CWMODE=3 nastavíte WiFi do reimu Sta+AP, tak si modul jako AP 
nastaví IP adresu 192.168.4.1. Nepøišel jsem na to jak to zmìnit. V principu 
to nièemu nevadí pokud Vaše sí nepouívá stejnı rozsah. To pak asi bude 
problém. Máte-li na domácí WiFi zapnuté šifrování (kdo ne?!) tak se jen pomocí 
pøíkazu AT+CWJAP nepøipojíte. Budete muset nejdøíve pomocí jiného pøíkazu 
nastavit podrobnìjší parametry pro Vaši sí. Bude to tedy vypadat napøíklad 
takto AT+CWSAP=“mojewifi“,“mojeheslo“,11,wap2 kde 11 je kanál a wap2 verze 
šifrování. Pak u staèí jen AT+CWJAP=“mojewifi“,“mojeheslo“ a modul je 
pøipojen. Snadno si to ovìøíte pøíkazem AT+CIFSR, kterı vrátí IP adresu modulu,
 na kterou mùete zkusit ping z poèítaèe. Zatím se domnívám, e parametry 
zadané pøíkazem AT+CWSAP si modul pamatuje i po vypnutí a smaou se teprve 
pøehráním firmware. Pokud máte doma lepší WiFi Access Point, urèitì se 
podívejte i do nìj. Já tøeba v mém MikroTiku po aktualizaci firmware modulu 
zjistil, e je signál silnìjší a spojení bìí rychleji.

Myslím, e jako první seznámení to staèí. Modul toho umí mnohem více a je 
vidìt jakım bouølivım vıvojem software okolo nìj prochází. U je dokonce moné 
programovat i pøímo ESP8266 pomocí SDK. Budu rád, kdy se s vısledky svıch 
pokusù podìlíte buï hned v diskuzi pod èlánkem nebo na našem fóru 
(http://forum.robodoupe.cz/)