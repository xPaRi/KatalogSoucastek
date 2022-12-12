#picaxe 20M2
setfreq M32

symbol CLK = C.1
symbol DIO = C.2
symbol RDIO= pinC.2
symbol STB = C.4
symbol dot=bit0
symbol i=b1
symbol intensity=b2
symbol startaddress=b3
symbol databyte=b4
symbol cmd=b5
symbol tmp=b6
symbol color=b7
symbol j=b8
symbol temp=b9
symbol keys=b10
symbol receive=b11
symbol getbuttons=b12
symbol datastore=b13
symbol address =b14
symbol sendloop=b15
symbol position=b16
symbol io=b17
symbol k=b18
'
symbol fixedaddress = $44
symbol incrementingaddress = $40
symbol displaymask=$80
symbol displayon=$08
symbol addressmask=$C0
symbol dotmask=$80
symbol readscan=$42
'
EEPROM 0,( %00111111, %00000110, %01011011, %01001111, %01100110, %01101101, %01111101, %00000111, %01111111, %01101111, %01110111, %01111100, %00111001, %01011110, %01111001, %01110001 )
EEPROM 32,(%00000000, %10000110, %00100010,%01111110, %01101101,%00000000, %00000000,%00000010,%00110000,%00000110)
EEPROM 42,(%01100011, %00000000, %00000100, %01000000,%10000000, %01010010, %00111111, %00000110, %01011011, %01001111) 
EEPROM 52,(%01100110, %01101101, %01111101, %00100111, %01111111, %01101111, %00000000, %00000000, %00000000, %01001000) 
EEPROM 62,(%00000000, %01010011, %01011111, %01110111, %01111111, %00111001, %00111111, %01111001, %01110001, %00111101)
EEPROM 72,(%01110110, %00000110, %00011111, %01101001,%00111000, %00010101, %00110111,%00111111, %01110011, %01100111) 
EEPROM 82,(%00110001, %01101101, %01111000, %00111110, %00101010, %00011101, %01110110, %01101110, %01011011,%00111001) 
EEPROM 92,(%01100100, %00001111, %00000000, %00001000, %00100000,%01011111, %01111100, %01011000, %01011110, %01111011) 
EEPROM 102,(%00110001, %01101111, %01110100, %00000100, %00001110, %01110101, %00110000, %01010101, %01010100, %01011100) 
EEPROM 112,(%01110011, %01100111, %01010000, %01101101, %01111000, %00011100, %00101010,%00011101, %01110110, %01101110) 
EEPROM 122,(%01000111, %01000110, %00000110, %01110000,%00000001)

symbol Green=2
symbol Red=1
symbol LEDOff=0
'
pause 1000
intensity=7
gosub setupdisplay
gosub cleardisplay
pause 1000
'
' Example program
'
dot=0' set no dot on seven segment output
position=3' set position of character to be output
read "H",databyte ' convert ascii to seven segment representation as far as possible
gosub sendchar
read "E",databyte
inc position
gosub sendchar
read "L",databyte
inc position
gosub sendchar
read "L",databyte
inc position
gosub sendchar
read "O",databyte
inc position
dot=1' Add a dot to last character
gosub sendchar
'
'
do
gosub func_getbuttons 'read in the buttons
tmp = getbuttons and $0F 'get the low nibble
dot=0
position=1
read tmp,databyte 'convert to hex representation
gosub sendchar
tmp=getbuttons/16 ' get the high nibble
position=0
read tmp,databyte 'convert to hex representation
gosub sendchar
for tmp=0 to 7 'output to all LEDs with each representing one button
i=getbuttons and 1
if i <> 0 then
color=red
position=tmp
gosub setled
else
color=green
position=tmp
gosub setled
endif
getbuttons =getbuttons /2
next
loop
'
' sub setupDisplay(intensity as byte) // Initialise the display
setupdisplay:
high STB
high DIO
high CLK
cmd=incrementingaddress
gosub sendcommand
startaddress=0
address=addressmask + startaddress
gosub sendaddress
for i= 0 to 15
io=0
gosub DOio
next
high STB
cmd=intensity & 7 | displaymask | displayon
gosub sendcommand
return
'end sub
'
'sub doIO(io as byte) // send output data to LKM1638
DOio:
for sendloop=0 to 7
j = io and 1
if j=1 then
high DIO
else 
LOW DIO
endif
io=io/2
pulsout CLK,1
next
return
'end sub
'
'sub sendCommand(cmd as byte) // send a command
sendcommand:
low STB
io=cmd
gosub DOio
high STB
return
'end sub
'
'sub sendAddress(address as byte) // send an address
sendaddress:
low STB
io=address
gosub DOio
return
'end sub
'
'sub Send(databyte as byte) // send a data byte
send:
io=databyte
gosub DOio
return 
'end sub
'
'sub sendData(startaddress as byte, databyte as byte) //send a databyte to a particular hardware address
senddata:
cmd=fixedaddress
gosub sendcommand
address= addressmask OR startaddress
gosub sendaddress
gosub send
high STB
return
'end sub
'
'sub sendChar (position as byte, databyte as byte, dot as boolean) //output to a particular position
sendchar:
startaddress=position*2
if dot=1 then 
databyte=databyte Or dotmask
endif
gosub senddata
return
'end sub
'
'sub setLED(color as byte, position as byte) // light or extinguish a particular LED
setled:
databyte=color//3
startaddress=position*2 +1
gosub senddata
return
'end sub
'
'sub clearDisplay() // clear the display
cleardisplay:
for i=0 to 14 step 2
startaddress=i
databyte=0
gosub senddata
next
return
'end sub
'
'sub clearLEDs() // clear the LEDs
clearLEDs:
for i=1 to 15 step 2
startaddress=i
databyte=0
gosub senddata
next
return
'end sub
'
'function receive () as byte // read in one byte of inputs
func_receive:
temp=0
high DIO 
input DIO
for j=0 to 7
low CLK
temp=temp / 2
if RDIO=1 then
temp=temp or $80
endif
high CLK
next
output DIO
low DIO
receive=temp
return
'end function
'
'Function getbuttons() as byte // read all four bytes of inputs and collate results
func_getbuttons:
getbuttons=0
k=1
low STB
databyte=readscan
gosub send
for i=0 to 3
gosub func_receive
keys=receive * k
k=k*2
getbuttons=getbuttons or keys
next
high STB
return
'end function
