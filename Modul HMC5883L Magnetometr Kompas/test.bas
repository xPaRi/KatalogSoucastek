#picaxe 08M2
;#no_data

; zpracování dat z I2C magnetometru HMC5883L (modul GY-271)
; C.0 - sertxd (sériový výstup k PC nebo k dalšímu PICAXE)
; C.1 - SCL (k magnetometru)
; C.2 - SDA (k magnetometru)
; C.3 - in (-- nepoužito --)
; C.4 - out (LED, připojena přes odpor cca 800 ohm k zemi nebo k + napájení)
; C.5 - serin (-- nepoužito --)
; napájení 5V (4x AA NiMH nabíjitelné akumulátory) [pro PICAXE i modul současně]
; měřený rozsah v každé ose magnetometru je cca 1200 (+-600), šum cca 30

eeprom 0, (0,6,11,17,22,27,31,35,39,42,45) ; předpočítaná tabulka úhlů (atan)

symbol xm = w0           ; měřená osa X magnetometru
symbol ym = w1           ; měřená osa Y magnetometru
symbol zm = w2           ; měřená osa Z magnetometru
symbol cas = w3          ; pro sledování změn proměnné "time"
symbol cyklu = w4        ; počítadlo cyklů (pro měření rychlosti zpracování dat)
symbol azimut = w5       ; výsledný přepočítaný azimut

symbol uhel0 = w6        ; pomocný úhel (bude to pamět zákl. azimutu po zapnutí)
symbol uhel = b14        ; výsledný úhel po výpočtu arctan
symbol uhel2 = b15       ; pomocný úhel určený z tabulky
symbol tan100 = b16      ; tangens * 100 (X / Y * 100)
symbol index = b17       ; vypočítaný index do tabulky
symbol offset = b18      ; vypočítaný posun mezi body indexu
symbol kvadrant = b19    ; kvadrant azimutu podle hodnot X a Y

symbol x = w10           ; zkalibrovaná a normalizovaná hodnota X (0 - 200)
symbol y = w11           ; zkalibrovaná a normalizovaná hodnota Y (0 - 200)
symbol z = w12           ; zkalibrovaná a normalizovaná hodnota Z (0 - 200)

setfreq m32              ; zvýšení frekvence procesoru (max. 32 MHz [8x normal])

pause 100                ; počkáme na ustálení napájení a vstupů po startu
sertxd ("PICAXE 08M2 kompas s HMC5883L",CR,LF)

hi2csetup i2cmaster, $3C, i2cfast_32, i2cbyte  ; inicializace I2C na adrese 3C
hi2cout 0, ( %01010100 ) ; config A - 4 averaged, 30Hz updates, normal measurement
hi2cout 1, ( %00100000 ) ; config B - 1.3Ga gain
hi2cout 2, ( %00000000 ) ; mode - continuous conversion

sertxd ("setup OK",CR,LF)

do
  inc cyklu
  if time>cas then       ; měříme a vypisujeme po každé změně "time" (2x za sec.)
    cas = time
    hi2cin 3, ( b1,b0 , b3,b2 , b5,b4 ) ; načtení holých dat z magnetometru (x,y,z)
    xm = xm + 800 / 8    ; normalizace X do max. rozsahu 0 - 200 se středem 100
    ym = ym + 800 / 8    ; normalizace Y do max. rozsahu 0 - 200 se středem 100
    zm = zm + 800 / 8    ; normalizace Z do max. rozsahu 0 - 200 se středem 100
    x = xm
    y = 200 - ym min 0 max 200
    z = zm
    gosub atan2           ; výpočet azimutu
    sertxd ("time:",#time,"  azimuth:",#azimut,CR,LF)
    toggle C.4           ; kontrolní blikání LEDkou
    cyklu = 0
  endif
  ;gosub atan2            ; jen pro odhad rychlosti výpočtu
loop

atan2:
  ; výpočet azimutu podle X a Y (arcustangens a kvadrant)
  ; hodnoty X a Y musí být předem normalizovány do rozsahu 0 - 200
  ; nula je na hodnotě 100 (pro X i pro Y)
  if x>=100 then
    x = x - 100
    if y>=100 then
      y = y - 100
      kvadrant = 0
    else
      y = 100 - y
      kvadrant = 3
    endif
  else
    x = 100 - x
    if y>=100 then
      y = y - 100
      kvadrant = 1
    else
      y = 100 - y
      kvadrant = 2
    endif
  endif
  tan100 = 100
  if x>y then
    tan100 = 100 * y / x
  else if y>x then
    tan100 = 100 * x / y
  endif
  ; výpočet atan interpolací mezi dvěma předpočítanými hodnotami z tabulky (0 - 45°)
  index = tan100 / 10
  offset = tan100 % 10
  read index,uhel
  inc index
  read index,uhel2
  azimut = uhel2 - uhel * offset / 10 + uhel
  ; posun azimutu do správného kvadrantu
  if y>=x then
    azimut = 90 - azimut
  endif
  if kvadrant = 1 or kvadrant = 3 then
    azimut = kvadrant * 90 - azimut + 90
  else
    azimut = kvadrant * 90 + azimut
  endif
return
