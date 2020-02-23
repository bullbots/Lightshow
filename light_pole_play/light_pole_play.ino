#include "FastLED.h"

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
//
// This example also shows one easy way to define multiple 
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    3
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    90
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          999
#define FRAMES_PER_SECOND  120

void setup() {
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
//SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };
//SimplePatternList gPatterns = { bounce, rainbow, Rows, confetti, chase, sinelon };
SimplePatternList gPatterns = { rainbow };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint8_t gWhichLED = 0;
uint8_t gdirection = 1;
  
void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();
  nextPattern();
}
  
   
  /*
  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { 
    gHue++; 
    gWhichLED += gdirection;
    if ( gWhichLED >= NUM_LEDS/4 ) {
      gdirection = -1;
     
    }
    else if(gWhichLED <= 0){
      
      gdirection = 1;
    }
    
  } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 10 ) { 
    nextPattern(); 
    gWhichLED = 0;
    } // change patterns periodically
  */


#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void bounce()
{
  for ( int times = 1; times <= 4; times++ ) {
    for(int i = 0; i < NUM_LEDS/2; i++) {

      // blank out all the lights
      for ( int b = 0; b < NUM_LEDS; b++ ) {
        leds[b] = CRGB(0, 0, 0);
      }
  
      leds[i] = CRGB( 0, 0x40, 0xff);
      leds[int(NUM_LEDS/2) + i] = CRGB( 0, 0x40, 0xff);
      leds[int(NUM_LEDS/2) - i]= CRGB( 0, 0xff, 0);
      leds[NUM_LEDS - i]= CRGB( 0, 0xff, 0);
  
      FastLED.show();
      delay(20);
      
//      if(i == NUM_LEDS/4 || i == 0){
//        FastLED.delay(70);
//      }
    }
  }
  
  FastLED.clear();
  delay(0);
}
void rainbow() 
{
  int color = 0;
  int leingth = NUM_LEDS;
  uint8_t hue = 121;
//  FastLED.clear();
  for ( int i = 0; i < 1000; i++ ) {
    // FastLED's built-in rainbow generator
    leingth = NUM_LEDS - random8(15);
    for (int a = 0; a < NUM_LEDS; a++){
      leds[a] = CHSV(hue, 255, 192);
      color += 7;
      if (hue+color > 300){
        color = 0;
      }
    }
    FastLED.show();
    delay(30);
    hue ++;
    if (hue > 300){
      hue = 121;
    }
  }
  FastLED.clear();

  delay(0);
  
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  FastLED.clear();

  int color = 0;
  // random colored speckles that blink in and fade smoothly
  for (int i = 0; i < 500; i++ ) {
    color = random8(255);
    int pos = random16(NUM_LEDS);
    if ((i % 2 ) == 0) {
      leds[pos] += CRGB( 0, color, 0);
    } else {
      leds[pos] += CRGB( 0, 0, color);
    }
    FastLED.show();
    delay(20);
    fadeToBlackBy( leds, NUM_LEDS, 8);
  }
}

void chase()
{  
  FastLED.clear();
  int green = 0;
  int blue = 0;
  int brightness = 255;
  if ((random8(2) % 2) == 0){
    blue = brightness;
  } else {
    green = brightness;
  }
  FastLED.show();
  delay(10);
  for (int i = 0; i < NUM_LEDS; i++){
    fadeToBlackBy( leds, NUM_LEDS, 100);
    for (int l = i; l < NUM_LEDS; l++){
    if ((l % 15) == 0){
      leds[l] = CRGB(0, blue, green);
    }
  }
    leds[i] = CRGB(0, (green / (i%15 + 1)), (blue / (i%15 + 1)));
    FastLED.show();
    delay(100);
  }
}



void sinelon()
{
  int green = 0;
  int blue = 0;
  int brightness = 255;
  FastLED.clear();
  for (int l = 0; l < 4; l++)
  {
    brightness = random8(150, 255);
    if ((brightness % 2) == 0){
    blue = brightness;
    green = 0;
  } else {
    green = brightness;
    blue = 0;
  }
  for (int i = 0; i < NUM_LEDS; i ++) {
    fadeToBlackBy( leds, NUM_LEDS, 30);
    leds[i] = CRGB(0, green, blue);
    FastLED.show();
    delay(10);
  }
  for (int i = 0; i < NUM_LEDS; i ++) {
    fadeToBlackBy( leds, NUM_LEDS, 30);
    leds[int(NUM_LEDS - i)] = CRGB(0, blue, green);
    FastLED.show();
    delay(10);
  }
  }
  
}
/*
void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}
*/
void Dots () {
  for(int i = 0; i <= NUM_LEDS; i+=7){
    leds[i] = CRGB( 0xff, 0xe2, 0x0a);
    
  }
  
}
void Rows(){
  FastLED.clear();
  Dots();
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB( 0x10, 0x1f, 0xef);   // blue
    leds[NUM_LEDS - i] = CRGB( 0x00, 0xef, 0x00);     // green
    FastLED.show();
    delay(30);
  }
  delay(0);
}
  /*
 void showleds(){
    // send the 'leds' array out to the actual LED strip
    FastLED.show();  
    // insert a delay to keep the framerate modest
 }
 */


