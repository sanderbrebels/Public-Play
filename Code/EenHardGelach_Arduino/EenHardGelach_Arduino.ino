#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
// adafruit libary is nodig om de neonpixel lichtjes aan te sturen. Deze kan je vinden op de website van AdaFruit
#define PIN            7 // de pin van adafruit op digitale poort 7
#define NUMPIXELS      50
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Alle gedeclareerde variabelen

volatile byte state = LOW;
int firstSensor = 0;    // first analog sensor
int secondSensor = 0;   // second analog sensor
int thirdSensor = 0;    // digital sensor
int inByte = 0;         // incoming serial byte

int knobvalue;
int potpin = 0;

int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
const int  buttonPin = 4;

void setup() {
  attachInterrupt(0, sendData, CHANGE ); // 0 staat voor digitale pin 2. Deze is nodig om tegen digitale pin 2 te zeggen dat de status is veranderd. Het verhelpt het timingsprobleem. Hierdoor kan er gelijktijdig gecommuniceerd worden tussen 2 arduino's.
  // Door de delay's was het niet meer mogelijk om tegelijk te communiceren, daarom het gebruik van de interrupt.
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library.

  // start serial port at 115200 bps:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  pinMode(11, OUTPUT); // dit was om de LED-pin aan te sturen, maar deze wordt niet meer gebruikt in de huidige setup. Het kan gebruikt worden om een LED'tje te doen branden om spacebrew te testen.
  establishContact();  // send a byte to establish contact until receiver responds
}

void loop() {
  // if we get a valid byte, read analog ins:
  if (Serial.available() > 0) {
    // get incoming byte:

    inByte = Serial.read(); // hetgeen wat er binnen komt, stop dit in de integer 'inByte'.
    // Serial.println(inByte);
    ledview(inByte); // voer de functie ledview uit met als parameter 'inByte'.
    pixels.show(); // laat de uitkomst van ledview zien

    delay(10); // vertraging van 10ms zodat het programma niet vastloopt
  }

}

void establishContact() { // hier wordt een seriële verbinding gemaakt met processing
  while (Serial.available() <= 0) {
    Serial.print('A');   // send a capital A
    delay(300);
  }
}

void ledview (int mybyte) {
  switch (mybyte) { // kijk welke for functie moet uitgevoerd worden
    case 'R': // show ledstrip in red
      for (int i = 0; i < NUMPIXELS; i++) { // alle neonpixels overlopen en voor elke i wordt er de setPixelColor uitgevoerd
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(0, 150, 0)); // ROOD
      }
      break;
    case 'G':
      for (int i = 0; i < NUMPIXELS; i++) {
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(150, 0, 0)); // GROEN
      }
      break;
    case 'B':
      for (int i = 0; i < NUMPIXELS; i++) {
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(0, 0, 150)); // BLAUW
      }
      break;
    case 'X':
      for (int i = 0; i < NUMPIXELS; i++) {
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(0, 0, 150)); // BLAUW
      }
      break;

    case 'Y':
      for (int i = 0; i < NUMPIXELS; i++) {
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(255, 255, 0)); // licht geel
      }
      break;


    case 'W':
      for (int i = 0; i < NUMPIXELS; i++) {
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(127, 127, 127)); // wit
      }
      break;


    case 'P':
      colorWipe(pixels.Color(0, 0, 255), 50); // Blue
      colorWipe(pixels.Color(127, 127, 127), 50); // wit
      colorWipe(pixels.Color(255, 0, 0), 50); // groen
      break;

    case 'O':
      rainbow(25);
      break;
  }
}


void colorWipe(uint32_t c, uint8_t wait) { // standaard functies uit de neonpixel libary
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, c);
    pixels.show();
    delay(wait);
  }
}


void rainbow(uint8_t wait) { // standaard functies uit de neonpixel libary
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel((i + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

// deze functie zorgt ervoor dat alle lichtjes één voor één oplichten en niet tegelijk. Als een soort 'kleurenwiel'.
uint32_t Wheel(byte WheelPos) { // standaard functies uit de neonpixel libary
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


void sendData() { // controleer of de huidige status veranderd is of niet, en stuur terug of de seriële verbinding bezig is of niet // TRUE/FALSE.

  state = !state;
  int thirdSensor = map(state, 0, 1, 0, 255);
  Serial.write(thirdSensor);


}

