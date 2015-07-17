/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  This example code is in the public domain.
 */

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;
int FRONT_RED = 52;
int FRONT_BLACK = 53;
int BACK_RED = 50;
int BACK_BLACK = 51;

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  pinMode(FRONT_RED, OUTPUT);
  pinMode(FRONT_BLACK, OUTPUT);
  pinMode(BACK_RED, OUTPUT);
  pinMode(BACK_BLACK, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(FRONT_RED, HIGH);
  digitalWrite(FRONT_BLACK, LOW);
  digitalWrite(BACK_RED, HIGH);
  digitalWrite(BACK_BLACK, LOW);
  delay(1000);               // wait for a second
  digitalWrite(led, LOW);
  digitalWrite(FRONT_RED, LOW);
  digitalWrite(FRONT_BLACK, HIGH);
  digitalWrite(BACK_RED, LOW);
  digitalWrite(BACK_BLACK, HIGH);
  delay(1000);               // wait for a second
}

