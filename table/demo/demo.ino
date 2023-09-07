#include <FastLED.h>

FASTLED_USING_NAMESPACE

// GPIO
#define DATA_PIN    A0
constexpr uint8_t GPIO_BRIGHTNESS_POT = A1;
constexpr uint8_t GPIO_BRIGHTNESS_VCC = A2;


#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    22
CRGB leds[NUM_LEDS];

constexpr uint8_t BRIGHTNESS_MIN = 17;
#define FRAMES_PER_SECOND  10

void setup() {
  Serial.begin(115200);

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(11);

  pinMode(GPIO_BRIGHTNESS_VCC, 1);
  digitalWrite(GPIO_BRIGHTNESS_VCC, 1);
}

static uint8_t gHue;

void loop() {
  fill_rainbow( leds, NUM_LEDS, gHue, 7);

  // send the 'leds' array out to the actual LED strip
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;  // slowly cycle the "base color" through the rainbow
  }

  EVERY_N_MILLISECONDS(100) {
    uint8_t b = (map(analogRead(GPIO_BRIGHTNESS_POT), 0, 1023, BRIGHTNESS_MIN, 255));
    Serial.println(b);
    FastLED.setBrightness(dim8_video(b));
  }
}
