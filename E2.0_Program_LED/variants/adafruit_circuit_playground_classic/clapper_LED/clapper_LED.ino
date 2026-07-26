#include <Adafruit_CircuitPlayground.h>

uint8_t color = 0; // 0 - 255
uint8_t intensity = 0; // 0 - 255
bool brighter = true;

unsigned long clapStart = 0;
long interval = 600000;
bool clap_button_on = false;


// Frequency for playing notes
// C: 261.63
// D: 293.66
// E: 329.63
// F: 349.23
// G: 392.00
// A: 440.00
// B: 493.88

uint8_t note_max = 16;
int music[16] = {261, 261, 392, 392, 440, 440, 392, 392, 349, 349, 329,329, 293, 293, 261, 261};
uint8_t note_index = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Circuit Playground test!");

  CircuitPlayground.begin();
}


void loop() {
  // turn off speaker when not in use
  CircuitPlayground.speaker.enable(false);

  unsigned long currentTime = millis();

  float microphone_read = CircuitPlayground.mic.soundPressureLevel(10);

  if (clapStart > 0 && currentTime - clapStart < interval) { // Clap within 2 s
    clap_button_on = true;
    clapStart = 0;
  } else if (clapStart > 0 && currentTime - clapStart > interval) { // Clap timeout
    clapStart = 0;
  }

  if (microphone_read > 70) { // Clap
    clapStart = currentTime;
  }

  
  if (CircuitPlayground.slideSwitch()) {
    CircuitPlayground.speaker.enable(true);
    CircuitPlayground.playTone(music[note_index], 100);
    //Serial.println(music[noteIndex]);
    note_index++;
    if (note_index == note_max) {
      note_index = 0;
    }
    delay(100);
  }

  if (CircuitPlayground.leftButton()) {
    clap_button_on = false;
  }

  if (clap_button_on == true) {
    for (int pixel = 0; pixel < 10; pixel++) {
      CircuitPlayground.setPixelColor(pixel, CircuitPlayground.colorWheel(color));
    }
    CircuitPlayground.setBrightness(microphone_read);
  } else {
    CircuitPlayground.clearPixels();
  }
  color++;

  if (color == 255) {
    color = 0;
    
  }

  if (intensity == 255) {
    brighter = false;
  } else if (intensity == 50){
    brighter = true;
  }

  if (brighter) {
    intensity++;
  } else {
    intensity--;
  }

  delay(100);
}