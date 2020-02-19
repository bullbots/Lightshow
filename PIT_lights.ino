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
#define NUM_LEDS    870
CRGB leds[NUM_LEDS];
#define BLUE_HUE 160
#define GREEN_HUE 96

int BRIGHTNESS = 255;
#define FRAMES_PER_SECOND  100 

void setup() {
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  Serial.begin(9600);
  Serial.flush();
  // set master brightness control
  
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
//SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, juggle_bullbot, bpm };
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, juggle, bpm_bullbot };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint8_t gHue_rainbow = 0;
uint8_t gHue_bullbot = BLUE_HUE;
void loop()
{ 
  String input = "";
  while(Serial.available() > 0)
  {
    input += (char) Serial.read();
    delay(5);
  }
   if(input == "on")
  {
    BRIGHTNESS = 255;
    }
  else if (input == "off")
  {
    BRIGHTNESS = 0;
  }

  FastLED.setBrightness(BRIGHTNESS);
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();
 
  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;
    if(gHue_bullbot==BLUE_HUE){
      gHue_bullbot=GREEN_HUE;
    }else{
      gHue_bullbot=BLUE_HUE;
    } 
    } // slowly cycle the "base color" through the rainbow
 // EVERY_N_MILLISECONDS( 1 ) { gHue_rainbow++; }
 gHue_rainbow++;
  EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue_rainbow*8, 5);
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
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue_bullbot, 200, 255);
  for(int i=0; i<10; i++){
    pos = random16(NUM_LEDS);
    leds[pos] += CHSV( gHue_bullbot, 200, 255);
  }
}


void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos;
  for( int i = 0; i < 8; i++) {
    pos = beatsin16( 30, 0, NUM_LEDS-1 );
    leds[pos] += CHSV( gHue, 255, 192);
  }
}

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
void bpm_bullbot()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = OceanColors_p;
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

void juggle_bullbot() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte blue_hue = 160;
  byte green_hue = 96;
 //   leds[beatsin16( 21, 0, NUM_LEDS-1 )] |= CHSV(0, 255, 255); // red

  for( int i = 0; i < 2; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(green_hue, 255, 255);
    leds[beatsin16( i+14, 0, NUM_LEDS-1 )] |= CHSV(blue_hue, 255, 255);
  }
}


