/*
TM1638.h - Library for TM1638.

Copyright (C) 2011 Ricardo Batista (rjbatista <at> gmail <dot> com)

Based on a sketch by: Martin Hubacek (http://www.martinhubacek.cz)

This program is free software: you can redistribute it and/or modify
it under the terms of the version 3 GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "WProgram.h"
#include "TM1638.h"

/*
The bits are displayed by mapping bellow
 -- 0 --
|       |
5       1
 -- 6 --
4       2
|       |
 -- 3 --  .7
*/
const byte NUMBER_DATA[] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111, // 9
  0b01110111, // A
  0b01111100, // B
  0b00111001, // C
  0b01011110, // D
  0b01111001, // E
  0b01110001  // F
};

const byte ERROR[] = {
  0b01111001, // E
  0b01010000, // r
  0b01010000, // r
  0b01011100, // o
  0b01010000, // r
  0,
  0,
  0
};

TM1638::TM1638(byte dataPin, byte clockPin, byte strobePin)
{
  this->dataPin = dataPin;
  this->clockPin = clockPin;
  this->strobePin = strobePin;

  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(strobePin, OUTPUT);

  digitalWrite(strobePin, HIGH);
  digitalWrite(clockPin, HIGH);

  sendCommand(0x8B);
  sendCommand(0x40);
  
  digitalWrite(strobePin, LOW);
  send(0xC0);
  for (int i = 0; i < 16; i++) {
    send(0x00);
  }
  digitalWrite(strobePin, HIGH);
}

void TM1638::setDisplayToHexNumber(unsigned long number, byte dots)
{
  for (int i = 0; i < 8; i++) {
    setDisplayDigit(number & 0xF, 7 - i, (dots & (1 << i)) != 0);
    number >>= 4;
  }
}

void TM1638::setDisplayToDecNumber(unsigned long number, byte dots)
{
  if (number > 99999999L) {
    setDisplay(ERROR);
  } else {
    for (int i = 0; i < 8; i++) {
      if (number != 0) {
        setDisplayDigit(number % 10, 7 - i, (dots & (1 << i)) != 0);
        number /= 10;
      } else {
        setDisplayDigit(0, 7 - i, (dots & (1 << i)) != 0);
      }
    }
  }
}

void TM1638::setDisplayToBinNumber(byte number, byte dots)
{
  for (int i = 0; i < 8; i++) {
    setDisplayDigit((number & (1 << i)) == 0 ? 0 : 1, 7 - i, (dots & (1 << i)) != 0);
  }
}

void TM1638::setDisplayDigit(byte digit, byte pos, boolean dot)
{
  sendData(pos << 1, NUMBER_DATA[digit & 0xF] | (dot ? 0b10000000 : 0));
}

void TM1638::setDisplay(const byte values[])
{
  for (int i = 0; i < 8; i++) {
    sendData(i << 1, values[i]);
  }
}

void TM1638::setLED(byte color, byte pos)
{
    sendData((pos << 1) + 1, color);  
}

void TM1638::setLEDs(word leds)
{
  for (int i = 0; i < 8; i++) {
    byte val = 0;

    if (leds & (1 << i)) {
      val |= 1;
    }
      
    if (leds & (1 << (i + 8))) {
      val |= 2;
    }

    sendData((i << 1) + 1, val);
  }
}

byte TM1638::getButtons(void)
{
  byte keys = 0;

  digitalWrite(strobePin, LOW);  
  send(0x42);
  for (int i = 0; i < 4; i++) {
    keys |= receive() << i;
  }
  digitalWrite(strobePin, HIGH);

  return keys;
}

void TM1638::sendCommand(byte cmd)
{
  digitalWrite(strobePin, LOW);
  send(cmd);
  digitalWrite(strobePin, HIGH);
}

void TM1638::sendData(byte add, byte data)
{
  sendCommand(0x44);
  digitalWrite(strobePin, LOW);
  send(0xc0 | add);
  send(data);
  digitalWrite(strobePin, HIGH);
}

void TM1638::send(byte data)
{
  for (int i = 0; i < 8; i++) {
    digitalWrite(clockPin, LOW);
    digitalWrite(dataPin, data & 1 ? HIGH : LOW);
    data >>= 1;
    digitalWrite(clockPin, HIGH);
  }
}

byte TM1638::receive()
{
  byte temp = 0;

  // Pull-up on
  pinMode(dataPin, INPUT);
  digitalWrite(dataPin, HIGH);        

  for (int i = 0; i < 8; i++) {
    temp >>= 1;

    digitalWrite(clockPin, LOW);        

    if (digitalRead(dataPin)) {
      temp |= 0x80;
    }

    digitalWrite(clockPin, HIGH);        
  }

  // Pull-up off
  pinMode(dataPin, OUTPUT);
  digitalWrite(dataPin, LOW); 

  return temp;
}

