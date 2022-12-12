Přiřazení vývodů
----------------
22 SCL
21 SDA

15 SCL OLED
 4 SDA OLED

16 OLED_RST

Test výstupu na display
-----------------------
OLED_RST = pio.GPIO16
pio.pin.setdir(pio.OUTPUT, OLED_RST)
pio.pin.sethigh(OLED_RST) --musi byt nastaven, jinak to nejede

gdisplay.attach(gdisplay.SSD1306_128_64, gdisplay.LANDSCAPE_FLIP, false, 0x3C)
gdisplay.on()
gdisplay.clear(gdisplay.WHITE)
gdisplay.setfont(gdisplay.UBUNTU16_FONT)
gdisplay.write({gdisplay.CENTER,gdisplay.CENTER},"HELTEC", gdisplay.BLACK)

gdisplay.clear()
gdisplay.setfont(gdisplay.UBUNTU16_FONT)
gdisplay.write({gdisplay.CENTER,gdisplay.CENTER},"HELTEC")
