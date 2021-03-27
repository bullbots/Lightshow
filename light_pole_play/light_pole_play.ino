#include <FastLED.h>

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
uint8_t cycle = 0;
uint8_t r = 0;
uint8_t g = 255;
uint8_t gi = 1;
uint8_t b = 0;
uint8_t bi = 0;

#define BRIGHTNESS 200
#define FRAMES_PER_SECOND 120

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
SimplePatternList gPatterns = { rainbow, confetti, sinelon, juggle, gb };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 80; // rotating "base color" used by many of the patterns
  
void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
//  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 10 ) { nextPattern(); }
  EVERY_N_SECONDS( 2 ) {
    if( gCurrentPatternNumber != 0 ){
      if(random(2)%2 == 0){
        g = 0;
        b = 255;
      }else{
        b = 0;
        g = 255;
      }
    }
  } // change patterns periodically
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow() 
{
  if(gi%2==0){
    g += (1);
    if(g >= 254){
      gi ++;
      bi = 0;
    }
  }else{
    g -= (1);
    if(g <= 1){
      gi ++;
      bi = 0;
    }
  }
  for( int i = 0; i < NUM_LEDS; i++) {
//    if(gi%2==0){
//      g += (1);
//    }else{
//      g -= (1);
//    }
//    if(g >= 253 or g <= 0){
//      gi ++;
//      bi = 0;
//    }
//    leds[i] = CRGB(0, g, 255-g);
    
    leds[i] = CRGB(0, g, 255-g);
//    bi += 1;
  }
}

//void rainbowWithGlitter() 
//{
//  // built-in FastLED rainbow, plus some random sparkly glitter
//  rainbow();
//  addGlitter(80);
//}
//
//void addGlitter( fract8 chanceOfGlitter) 
//{
//  if( random8() < chanceOfGlitter) {
//    leds[ random16(NUM_LEDS) ] += CRGB::White;
//  }
//}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 1);
  int pos = random16(NUM_LEDS); 
  if(cycle % 16 == 0){
    leds[pos] = CRGB(0, random8(255), 0);
  }else if(cycle % 16 == 8){
    leds[pos] = CRGB(0, 0, random8(255));
  }
  cycle++;
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CRGB( 0, g, b);
}

//void bpm()
//{
//  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
//  uint8_t BeatsPerMinute = 62;
////  CRGBPalette16 palette = PartyColors_p;
//  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
//  for( int i = 0; i < NUM_LEDS; i++) { //9948
//    leds[i] = CRGB(0, g, b);
//  }
//}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
//  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CRGB(0, g, b);
//    dothue += 32;
  }
}

void gb() {
  for( int i = 0; i < NUM_LEDS; i++) {
    if(i > NUM_LEDS/2){
      leds[i] = CRGB(0, g, b);
    }else{
      leds[i] = CRGB(0, 255-g, 255-b);
    }
  }
  FastLED.show();
  delay(500);
  for( int i = 0; i < NUM_LEDS; i++) {
    if(i > NUM_LEDS/2){
      leds[i] = CRGB(0, 255-g, 255-b);
    }else{
      leds[i] = CRGB(0, g, b);
    }
  }
  FastLED.show();
  delay(500);
}
