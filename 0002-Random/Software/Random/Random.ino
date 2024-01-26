#include<avr/io.h>

#define RANDLOW 1
#define RANDHIGH 255

// Pin definitions
#define clockInt 2

volatile byte randomness1 = 0;
volatile byte randomness2 = 0;
volatile unsigned int lfsr = 31;  // initialize to whatever, so long as its not zero

boolean doFlip(byte p) {
  clockLfsr();
  byte c1 = lfsr & B11111111;
  if (c1 < p) {
    return true;
  }
  return false;
}

byte getPatternValue() {
  byte result = B00000000;

  if (doFlip(randomness1)) {
    result = B00000010;
  } 
    
  if (doFlip(randomness2)) {
    result = result | B00000001;
  }

  return result;
}

void clockCounter()  // called by interrupt
{
  PORTB = getPatternValue();
}

void resetCounter()  // called by interrupt
{
  PORTB = B00000000;
}

ISR (PCINT0_vect)
{
  if(digitalRead(clockInt) == HIGH){
    clockCounter();
  } else if (digitalRead(clockInt) == LOW) {
    resetCounter();
  }
}

void clockLfsr() {
  boolean outputBit = bitRead(lfsr, 10) ^ bitRead(lfsr, 12)
                      ^ bitRead(lfsr, 13) ^ bitRead(lfsr, 15);
  lfsr = lfsr << 1;
  lfsr |= outputBit;
}

void setup() {
  DDRB = B00000011;
  PCMSK |= (1 << PCINT2);
  GIMSK |= (1 << PCIE);
  sei();
}

void loop() {

  while(1){
    clockLfsr();
    randomness1 = map(analogRead(A2), 0, 1023, RANDLOW, RANDHIGH);
    randomness2 = map(analogRead(A3), 0, 1023, RANDLOW, RANDHIGH);
  }

}
