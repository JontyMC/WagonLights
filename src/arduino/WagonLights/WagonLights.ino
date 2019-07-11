#include <bluefruit.h>
#include <Adafruit_NeoPixel.h>

#define NUMPIX    54
#define POT_PIN   A5 //potentiometer
#define MIC_PIN   A2 // Microphone is attached to this analog pin
#define LED_PIN    30  // NeoPixel LED strand is connected to this pin
#define BUTTON_PIN    7  // NeoPixel LED strand is connected to this pin
#define DC_OFFSET  0  // DC offset in mic signal - if unusure, leave 0
#define NOISE     30  // Noise/hum/interference in mic signal
#define SAMPLES   60  // Length of buffer for dynamic level adjustment
#define WSIDE1      0
#define WSIDE2      6
#define WSIDE3     25
#define WSIDE4      35
#define TOP1       8 // Allow dot to go slightly off scale
#define TOP2       21 // Allow dot to go slightly off scale`
#define TOP3       12 // Allow dot to go slightly off scale`
#define TOP4       21 // Allow dot to go slightly off scale`
#define PEAK_FALL 60  // Rate of peak falling dot

byte
  peak1      = 0,      // Used for falling dot
  peak2      = 0,      // Used for falling dot
  peak3      = 0,      // Used for falling dot
  peak4      = 0,      // Used for falling dot
  dotCount  = 0,      // Frame counter for delaying dot-falling speed
  volCount  = 0;      // Frame counter for storing past volume data
int
  vol[SAMPLES],       // Collection of prior volume samples
  lvl       = 10,      // Current "dampened" audio level
  minLvlAvg = 0,      // For dynamic adjustment of graph low & high
  maxLvlAvg1 = 512,
  maxLvlAvg2 = 512,
  maxLvlAvg3 = 512,
  maxLvlAvg4 = 512,
  mode = 0,
  buttonState = 0,
  potThreshold = 10,
  potLastValue = 0,
  brightness = 230,
  cycleNum = 0,
  cycleNum2 = 0,
  colorCycleNum = 0,
  wait = 50,
  paletteLength,
  defaultShowNum = 0,
  width = 6,
  defaultShowCycles = 0,
  currentShowCycles = 1,
  dotNum = -1,
  pulseColor = 0,
  customPaletteNum = -1,
  customPaletteLength = 0;
unsigned long time,
  lastTime = 0,
  temperatureInterval = 1000,
  temperatureLastRefresh = 0,
  buttonInterval = 150,
  buttonLastRefresh = 50,
  modeLastRefresh = 0;
bool switchShow = false, strobe = false;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIX, LED_PIN, NEO_RGB + NEO_KHZ400);

uint32_t palette[10], customPalette[10],
  red = strip.Color(255, 0, 0),
  green = strip.Color(0, 255, 0),
  blue = strip.Color(0, 0, 255),
  yellow = strip.Color(255, 255, 0),
  orange = strip.Color(255, 100, 0),
  pink = strip.Color(255, 105, 180),
  purple = strip.Color(128, 0, 128),
  white = strip.Color(255, 255, 255),
  flowerPalette[54];

static uint8_t wagonServiceId[31] = { 0x01,0x01,0x06, 0x1A,0xFF,0x4C,0x00,0x02,0x15,0x71,0x3d,0x00,0x00,0x50,0x3e,0x4c,0x75,0xba,0x94,0x31,0x48,0xf1,0x8d,0x94,0x1e,0x00,0x00,0x00,0x00,0xC5 };
static uint8_t wagonCharacteristic1Id[31] = { 0x02,0x01,0x06, 0x1A,0xFF,0x4C,0x00,0x02,0x15,0x71,0x3d,0x00,0x00,0x50,0x3e,0x4c,0x75,0xba,0x94,0x31,0x48,0xf1,0x8d,0x94,0x1e,0x00,0x00,0x00,0x00,0xC5 };
static uint8_t wagonCharacteristic2Id[31] = { 0x03,0x01,0x06, 0x1A,0xFF,0x4C,0x00,0x02,0x15,0x71,0x3d,0x00,0x00,0x50,0x3e,0x4c,0x75,0xba,0x94,0x31,0x48,0xf1,0x8d,0x94,0x1e,0x00,0x00,0x00,0x00,0xC5 };
static uint8_t wagonCharacteristic3Id[31] = { 0x04,0x01,0x06, 0x1A,0xFF,0x4C,0x00,0x02,0x15,0x71,0x3d,0x00,0x00,0x50,0x3e,0x4c,0x75,0xba,0x94,0x31,0x48,0xf1,0x8d,0x94,0x1e,0x00,0x00,0x00,0x00,0xC5 };
static uint8_t wagonCharacteristic4Id[31] = { 0x05,0x01,0x06, 0x1A,0xFF,0x4C,0x00,0x02,0x15,0x71,0x3d,0x00,0x00,0x50,0x3e,0x4c,0x75,0xba,0x94,0x31,0x48,0xf1,0x8d,0x94,0x1e,0x00,0x00,0x00,0x00,0xC5 };
static uint8_t wagonCharacteristic5Id[31] = { 0x06,0x01,0x06, 0x1A,0xFF,0x4C,0x00,0x02,0x15,0x71,0x3d,0x00,0x00,0x50,0x3e,0x4c,0x75,0xba,0x94,0x31,0x48,0xf1,0x8d,0x94,0x1e,0x00,0x00,0x00,0x00,0xC5 };
static uint8_t wagonCharacteristic6Id[31] = { 0x07,0x01,0x06, 0x1A,0xFF,0x4C,0x00,0x02,0x15,0x71,0x3d,0x00,0x00,0x50,0x3e,0x4c,0x75,0xba,0x94,0x31,0x48,0xf1,0x8d,0x94,0x1e,0x00,0x00,0x00,0x00,0xC5 };
static uint8_t wagonCharacteristic8Id[31] = { 0x09,0x01,0x06, 0x1A,0xFF,0x4C,0x00,0x02,0x15,0x71,0x3d,0x00,0x00,0x50,0x3e,0x4c,0x75,0xba,0x94,0x31,0x48,0xf1,0x8d,0x94,0x1e,0x00,0x00,0x00,0x00,0xC5 };
static uint8_t wagonCharacteristic9Id[31] = { 0x09,0x02,0x06, 0x1A,0xFF,0x4C,0x00,0x02,0x15,0x71,0x3d,0x00,0x00,0x50,0x3e,0x4c,0x75,0xba,0x94,0x31,0x48,0xf1,0x8d,0x94,0x1e,0x00,0x00,0x00,0x00,0xC5 };
BLEService wagonService = BLEService(wagonServiceId);
BLECharacteristic wagonCharacteristic1 = BLECharacteristic(wagonCharacteristic1Id);
BLECharacteristic wagonCharacteristic2 = BLECharacteristic(wagonCharacteristic2Id);
BLECharacteristic wagonCharacteristic3 = BLECharacteristic(wagonCharacteristic3Id);
BLECharacteristic wagonCharacteristic4 = BLECharacteristic(wagonCharacteristic4Id);
BLECharacteristic wagonCharacteristic5 = BLECharacteristic(wagonCharacteristic5Id);
BLECharacteristic wagonCharacteristic6 = BLECharacteristic(wagonCharacteristic6Id);
BLECharacteristic wagonCharacteristic8 = BLECharacteristic(wagonCharacteristic8Id);
BLECharacteristic wagonCharacteristic9 = BLECharacteristic(wagonCharacteristic9Id);

void setup() {
  Serial.begin(115200);
  Serial.println("Wagon Lights Server");
  Serial.println("-----------------------");
  pinMode(BUTTON_PIN, INPUT);
  memset(vol, 0, sizeof(vol));
  setupFlowers();
  strip.begin();
  changePalette(1);
  Bluefruit.begin();
  Bluefruit.setName("Wagon");
  setupWagonService();
  startAdv();
}

void loop() {
  time = millis();
  if (pulseColor > 0) {
    pulse();
  }
  switch (mode) {
    default: //also case 0
      defaultShow();
      break;
    case 1:
      flower();
      break;
    case 2:
      soundReactive();
      break;
    case 3:
      solidChase();
      break;
    case 4:
      fade();
      break;
    case 5:
      rainbow();
      break;
    case 6:
      rainbowDist();
      break;
    case 7:
      colorWipe();
      break;
    case 8:
      theaterChase();
      break;
    case 9:
      solid();
      break;
    case 10:
      fillUp();
      break;
    case 11:
      ledsOff();
      break;
  }
  
  if(time - temperatureLastRefresh >= temperatureInterval)
  {
    temperatureLastRefresh = time;
    takeTemperature();
  }

  if (time - buttonLastRefresh >= buttonInterval) {
    buttonLastRefresh = time;
    int newButtonState = digitalRead(BUTTON_PIN);
    if (newButtonState == HIGH && buttonState != HIGH) {
      //Serial.println("b on");
      incrementMode();
    }
    buttonState = newButtonState;
    int reading = analogRead(POT_PIN);
    if (abs(potLastValue - reading) > potThreshold) {
      potLastValue = reading;
      brightness = map(reading,0,1000,0,255);
      //Serial.println(brightness);
      strip.setBrightness(brightness);
    }
  } 
}

void pulse() {
  uint32_t pColor = getColor(pulseColor - 2);
  int c = strobe == true ? 10 : 1;
  for (int i = 0; i < c; i++) {
    for (int j = 255; j >= 0; j=j-5) {
      for(int k=0; k<NUMPIX; k++) {
        strip.setPixelColor(k, pColor);
      }
      strip.setBrightness(j);
      strip.show();
      delay(1);
    }
  }
  strip.setBrightness(brightness);
  pulseColor = 0;
}

void flower() {
  if (time - modeLastRefresh >= wait || modeLastRefresh == 0) {
    modeLastRefresh = time;
    for(int i=0; i<NUMPIX; i++) {
      strip.setPixelColor(i, flowerPalette[i]);
    }
    strip.show();
    cycleNum++;
    if (cycleNum > 100) {
      switchShow = true;
    }
  }
}

void defaultShow() {
  if (switchShow == true) {
    switchShow = false;
    defaultShowNum++;
    if (defaultShowNum > 8){
      defaultShowNum = 0;
    }
    cycleNum = 0;
    cycleNum2 = 0;
    strip.setBrightness(brightness);
    Serial.print("Took: "); Serial.println(time - lastTime);
    lastTime = time;
    Serial.print("Mode: "); Serial.println(defaultShowNum);
  }
  switch (defaultShowNum) {
    case 0:
      flower();
      break;
    case 1:
      solidChase();
      break;
    case 2:
      rainbow();
      break;
    case 3:
      colorWipe();
      break;
    case 4:
      solid();
      break;
    case 5:
      fillUp();
      break;
    case 6:
      rainbowDist();
      break;
    case 7:
      theaterChase();
      break;
    case 8:
      fade();
      break;
  }
}

void incrementMode() {
  mode += 1;
  if (mode > 11) {
    mode = 0;
  }
  cycleNum = 0;
  cycleNum2 = 0;
}

void takeTemperature() {
  int reading = analogRead(A0);
  float voltage = reading * 3.3;
  voltage /= 1024.0; 
 
  // print out the voltage
  //Serial.print(voltage); Serial.println(" volts");
 
  // now print out the temperature
  float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                               //to degrees ((voltage - 500mV) times 100)
  //Serial.print(temperatureC); Serial.println(" degrees C");
  char str[3];
  sprintf(str, "%d", (int)temperatureC);
  wagonCharacteristic1.notify(str);
}

void ledsOff() {
  setAll(strip.Color(0, 0, 0));
}

void setAll(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
}

void fillUp() {
  defaultShowCycles = 1;
  if (time - modeLastRefresh >= wait || modeLastRefresh == 0) {
    modeLastRefresh = time;
    for(int i=18; i>dotNum; i--) {
      //side 1
      if (i < 6) {
        strip.setPixelColor(i, 0);
      }
      //side 2
      strip.setPixelColor(i+6, 0);
      //side 3
      if (i < 10) {
        strip.setPixelColor(i+25, 0);
      }
      //side 4
      strip.setPixelColor(i+35, 0);
    }
    if (dotNum > -1) {
      //side 1
      if (dotNum < 6) {
        strip.setPixelColor(dotNum, palette[colorCycleNum]);
      }
      //side 2
      strip.setPixelColor(dotNum+6, palette[colorCycleNum]);
      //side 3
      if (dotNum < 10) {
        strip.setPixelColor(dotNum+25, palette[colorCycleNum]);
      }
      //side 4
      strip.setPixelColor(dotNum+35, palette[colorCycleNum]);
    }
    int curr = 18 - cycleNum;
    //side 1
    if (dotNum < 6) {
      strip.setPixelColor(curr, palette[colorCycleNum]);
    }
    //side 2
    strip.setPixelColor(curr+6, palette[colorCycleNum]);
    //side 3
    if (dotNum < 10) {
      strip.setPixelColor(curr+25, palette[colorCycleNum]);
    }
    //side 4
    strip.setPixelColor(curr+35, palette[colorCycleNum]);
    strip.show();
    cycleNum++;
    if (curr == dotNum) {
      cycleNum = 0;
      dotNum++;
      if (dotNum > 18) {
        dotNum = -1;
        incrementPalette();
      }
    }
  }
}

void solid() {
  defaultShowCycles = 4;
  if (time - modeLastRefresh >= wait * 20) {
    modeLastRefresh = time;
    setAll(palette[colorCycleNum]);
    incrementPalette();
  }
}

void solidChase() {
  defaultShowCycles = 4;
  if (time - modeLastRefresh >= wait * 3) {
    modeLastRefresh = time;
    if (cycleNum > NUMPIX) {
      cycleNum = 0;
      currentShowCycles++;
      if (currentShowCycles >= defaultShowCycles) {
        switchShow = true;
        currentShowCycles = 1;
      }
    } 
    int currentWidth = 0;
    int currentColor = 0;
    for (uint16_t i=0; i < NUMPIX; i++) {
      strip.setPixelColor((i + cycleNum) % NUMPIX, palette[currentColor]);
      currentWidth++;
      if (currentWidth >= width) {
        currentWidth = 0;
        currentColor++;
        if (currentColor >= paletteLength) {
          currentColor = 0;
        }
      }
    }
    strip.show();
    cycleNum++;
  }
}

void fade() {
  defaultShowCycles = 3;
  if (time - modeLastRefresh >= wait * 1.5) {
    modeLastRefresh = time;
    if (cycleNum > 22) {
      cycleNum = 0;
      incrementPalette();
    }
    int fadeBrightness = 0;
    if (cycleNum < 10) {
      fadeBrightness = map(cycleNum,0,9,0,brightness);
    } else if (cycleNum < 20) {
      fadeBrightness = map(cycleNum,10,20,brightness,0);
    }
    strip.setBrightness(fadeBrightness);
    setAll(palette[colorCycleNum]);
    cycleNum++;
  }
}

void colorWipe() {
  defaultShowCycles = 4;
  if (time - modeLastRefresh >= wait) {
    modeLastRefresh = time;
    if (cycleNum > strip.numPixels()) {
      cycleNum = 0;
      incrementPalette();
    }
    strip.setPixelColor(cycleNum, palette[colorCycleNum]);
    strip.show();
    cycleNum++;
  }
}

void incrementPalette() {
  colorCycleNum++;
  if (colorCycleNum >= paletteLength) {
    colorCycleNum = 0;
    currentShowCycles++;
    if (currentShowCycles >= defaultShowCycles) {
      switchShow = true;
      currentShowCycles = 1;
    }
  }
}

void theaterChase() {
  defaultShowCycles = 2;
  if (time - modeLastRefresh >= wait * 3) {
    modeLastRefresh = time;
    if (cycleNum > 2) {
      cycleNum = 0;
    }
    if (cycleNum2 > 54) {
      cycleNum2 = 0;
      incrementPalette();
      
    }
    for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
      uint16_t previous = i+cycleNum - 1;
      if (previous < 0) {
        previous = strip.numPixels();
      }
      strip.setPixelColor(previous, 0);        //turn every third pixel off
    }
    for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
      strip.setPixelColor(i+cycleNum, palette[colorCycleNum]);    //turn every third pixel on
    }
    strip.show();
    cycleNum++;
    cycleNum2++;
  }
}

void rainbow() {
  defaultShowCycles = 2;
  if (time - modeLastRefresh >= wait) {
    modeLastRefresh = time;
    if (cycleNum > 255) {
      cycleNum = 0;
      currentShowCycles++;
      if (currentShowCycles >= defaultShowCycles) {
        switchShow = true;
        currentShowCycles = 1;
      }
    }
    for(uint16_t i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + cycleNum) & 255));
    }
    strip.show();
    cycleNum++;
  }
}

void rainbowDist() {
  defaultShowCycles = 2;
  if (time - modeLastRefresh >= wait) {
    modeLastRefresh = time;
    if (cycleNum > 255) {
      cycleNum = 0;
      currentShowCycles++;
      if (currentShowCycles >= defaultShowCycles) {
        switchShow = true;
        currentShowCycles = 1;
      }
    }
    for(uint16_t i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + cycleNum) & 255));
    }
    strip.show();
    cycleNum++;
  }
}

void soundReactive() {
  uint8_t  i;
  uint16_t minLvl, maxLvl1, maxLvl2, maxLvl3, maxLvl4;
  int      n;

  n   = analogRead(MIC_PIN);                        // Raw reading from mic 
  n   = abs(n - 512 - DC_OFFSET); // Center on zero
  n   = (n <= NOISE) ? 0 : (n - NOISE);             // Remove noise/hum
  lvl = ((lvl * 7) + n) >> 3;    // "Dampened" reading (else looks twitchy)

  peak1 = setVolPixels(WSIDE1, WSIDE2-1, peak1, TOP1, maxLvlAvg1);
  peak2 = setVolPixels(WSIDE2, WSIDE3-1, peak2, TOP2, maxLvlAvg2);
  peak3 = setVolPixels(WSIDE3, WSIDE4-1, peak3, TOP3, maxLvlAvg3);
  peak4 = setVolPixels(WSIDE4, NUMPIX-1, peak4, TOP4, maxLvlAvg4);
  
  strip.show(); // Update strip

// Every few frames, make the peak pixel drop by 1:
  if(++dotCount >= PEAK_FALL) { //fall rate 
    if(peak1 > 0) peak1--;
    if(peak2 > 0) peak2--;
    if(peak3 > 0) peak3--;
    if(peak4 > 0) peak4--;
    dotCount = 0;
  }

  vol[volCount] = n;                      // Save sample for dynamic leveling
  if(++volCount >= SAMPLES) volCount = 0; // Advance/rollover sample counter

  // Get volume range of prior frames
  minLvl = maxLvl1 = maxLvl2 = maxLvl3 = maxLvl4 = vol[0];
  for(i=1; i<SAMPLES; i++) {
    if(vol[i] < minLvl)      minLvl = vol[i];
    else {
      if(vol[i] > maxLvl1) maxLvl1 = vol[i];
      if(vol[i] > maxLvl2) maxLvl2 = vol[i];
      if(vol[i] > maxLvl3) maxLvl3 = vol[i];
      if(vol[i] > maxLvl4) maxLvl4 = vol[i];
    }
  }
  // minLvl and maxLvl indicate the volume range over prior frames, used
  // for vertically scaling the output graph (so it looks interesting
  // regardless of volume level).  If they're too close together though
  // (e.g. at very low volume levels) the graph becomes super coarse
  // and 'jumpy'...so keep some minimum distance between them (this
  // also lets the graph go to zero when no sound is playing):
  if((maxLvl1 - minLvl) < TOP1) maxLvl1 = minLvl + TOP1;
  if((maxLvl2 - minLvl) < TOP2) maxLvl2 = minLvl + TOP2;
  if((maxLvl3 - minLvl) < TOP3) maxLvl3 = minLvl + TOP3;
  if((maxLvl4 - minLvl) < TOP4) maxLvl4 = minLvl + TOP4;
  minLvlAvg = (minLvlAvg * 63 + minLvl) >> 6; // Dampen min/max levels
  maxLvlAvg1 = (maxLvlAvg1 * 63 + maxLvl1) >> 6; // (fake rolling average)
  maxLvlAvg2 = (maxLvlAvg2 * 63 + maxLvl2) >> 6; // (fake rolling average)
  maxLvlAvg3 = (maxLvlAvg3 * 63 + maxLvl3) >> 6; // (fake rolling average)
  maxLvlAvg4 = (maxLvlAvg4 * 63 + maxLvl4) >> 6; // (fake rolling average)
}

byte setVolPixels(uint8_t first, uint8_t last, byte peak, int top, int maxLvlAvg) {
  // Calculate bar height based on dynamic min/max levels (fixed point):
  int height = top * (lvl - minLvlAvg) / (long)(maxLvlAvg - minLvlAvg);

  if(height < 0L)       height = 0;      // Clip output
  else if(height > top) height = top;
  if(height > peak)     peak   = height; // Keep 'peak' dot at top
  // Color pixels based on rainbow gradient
  uint8_t numPixels = last-first+1;
  for(uint8_t i=0; i<numPixels; i++) {
    if(i >= height)               strip.setPixelColor(i + first,   0,   0, 0);
    else strip.setPixelColor(i + first,Wheel(map(i,0,numPixels-1,30,150)));
  }

  // Draw peak dot  
  if(peak > 0 && peak <= numPixels-1) strip.setPixelColor(peak+first,Wheel(map(peak,0,19-1,30,150)));
  return peak;
}

uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void setupWagonService(void)
{
  wagonService.begin();
  
  wagonCharacteristic1.setProperties(CHR_PROPS_NOTIFY);
  wagonCharacteristic1.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  wagonCharacteristic1.setFixedLen(2);
  wagonCharacteristic1.begin();
  
  wagonCharacteristic2.setProperties(CHR_PROPS_WRITE);
  wagonCharacteristic2.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  wagonCharacteristic2.setFixedLen(20);
  wagonCharacteristic2.setWriteCallback(*write_cb);
  wagonCharacteristic2.begin();

  wagonCharacteristic3.setProperties(CHR_PROPS_WRITE);
  wagonCharacteristic3.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  wagonCharacteristic3.setFixedLen(20);
  wagonCharacteristic3.setWriteCallback(*write_cb2);
  wagonCharacteristic3.begin();

  wagonCharacteristic4.setProperties(CHR_PROPS_WRITE);
  wagonCharacteristic4.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  wagonCharacteristic4.setFixedLen(20);
  wagonCharacteristic4.setWriteCallback(*write_cb3);
  wagonCharacteristic4.begin();

  wagonCharacteristic5.setProperties(CHR_PROPS_WRITE);
  wagonCharacteristic5.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  wagonCharacteristic5.setFixedLen(20);
  wagonCharacteristic5.setWriteCallback(*write_cb4);
  wagonCharacteristic5.begin();

  wagonCharacteristic6.setProperties(CHR_PROPS_WRITE);
  wagonCharacteristic6.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  wagonCharacteristic6.setFixedLen(20);
  wagonCharacteristic6.setWriteCallback(*write_cb5);
  wagonCharacteristic6.begin();

  wagonCharacteristic8.setProperties(CHR_PROPS_WRITE);
  wagonCharacteristic8.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  wagonCharacteristic8.setFixedLen(20);
  wagonCharacteristic8.setWriteCallback(*write_cb7);
  wagonCharacteristic8.begin();

  wagonCharacteristic9.setProperties(CHR_PROPS_WRITE);
  wagonCharacteristic9.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  wagonCharacteristic9.setFixedLen(20);
  wagonCharacteristic9.setWriteCallback(*write_cb8);
  wagonCharacteristic9.begin();
}

union arrayToUint32 {
  byte array[4];
  uint32_t integer;
};

void write_cb(BLECharacteristic& chr, uint8_t* data, uint16_t len, uint16_t offset)
{
  Serial.println("BLE: Change mode");
  arrayToUint32 converter = {data[0],data[1],data[2],data[3]}; //Create a converter
  uint32_t i = converter.integer;
  Serial.println(i);
  mode = i;
  cycleNum = 0;
  cycleNum2 = 0;
}

void changePalette(int num) {
  switch (num) {
    default: //also case 0
      paletteLength = 7;
      palette[0] = red;
      palette[1] = orange;
      palette[2] = yellow;
      palette[3] = strip.Color(0, 128, 0);
      palette[4] = blue;
      palette[5] = strip.Color(75, 99, 130);
      palette[6] = strip.Color(189, 49, 255);
      break;
    case 1:
      paletteLength = 3;
      palette[0] = red;
      palette[1] = green;
      palette[2] = blue;
      break;
    case 2:
      paletteLength = 3;
      palette[0] = red;
      palette[1] = white;
      palette[2] = blue;
      break;
    case 3:
      paletteLength = 3;
      palette[0] = blue;
      palette[1] = yellow;
      break;
    case 4:
      paletteLength = customPaletteLength;
      for (int i = 0; i < customPaletteLength; i++) {
        palette[i] = customPalette[i];
      }
      break;
  }
}

void write_cb2(BLECharacteristic& chr, uint8_t* data, uint16_t len, uint16_t offset)
{
  Serial.println("BLE: Change brightness");
  arrayToUint32 converter = {data[0],data[1],data[2],data[3]}; //Create a converter
  uint32_t i = converter.integer;
  Serial.println(i);
  brightness = i;
  strip.setBrightness(i);
}

void write_cb3(BLECharacteristic& chr, uint8_t* data, uint16_t len, uint16_t offset)
{
  Serial.println("BLE: Change palette");
  arrayToUint32 converter = {data[0],data[1],data[2],data[3]}; //Create a converter
  uint32_t i = converter.integer;
  Serial.println(i);
  changePalette(i);
}

void write_cb4(BLECharacteristic& chr, uint8_t* data, uint16_t len, uint16_t offset)
{
  Serial.println("BLE: Change speed");
  arrayToUint32 converter = {data[0],data[1],data[2],data[3]}; //Create a converter
  uint32_t i = converter.integer;
  Serial.println(i);
  wait = i;
}

void write_cb5(BLECharacteristic& chr, uint8_t* data, uint16_t len, uint16_t offset)
{
  Serial.println("BLE: Change width");
  arrayToUint32 converter = {data[0],data[1],data[2],data[3]}; //Create a converter
  uint32_t i = converter.integer;
  Serial.println(i);
  width = i;
}

void write_cb7(BLECharacteristic& chr, uint8_t* data, uint16_t len, uint16_t offset)
{
  Serial.println("BLE: VJ");
  arrayToUint32 converter = {data[0],data[1],data[2],data[3]}; //Create a converter
  uint32_t i = converter.integer;
  Serial.println(i);
  if (i == 0) {
    strobe = true;
  } else if (i == 1) {
    strobe = false;
  } else {
    pulseColor = i;
  }
}

void write_cb8(BLECharacteristic& chr, uint8_t* data, uint16_t len, uint16_t offset)
{
  Serial.println("BLE: custom palette");
  arrayToUint32 converter = {data[0],data[1],data[2],data[3]}; //Create a converter
  uint32_t i = converter.integer;
  Serial.println(i);
  if (i == 99) {
    customPaletteLength = customPaletteNum + 1;
    customPaletteNum = -1;
  } else {
    customPaletteNum++;
    customPalette[customPaletteNum] = getColor(i);
  }
}

uint32_t getColor(int num) {
  switch (num) {
    default: //also case 0
      return white;
      break;
    case 1:
      return red;
      break;
    case 2:
      return green;
      break;
    case 3:
      return blue;
      break;
    case 4:
      return yellow;
      break;
    case 5:
      return orange;
      break;
    case 6:
      return pink;
      break;
    case 7:
      return purple;
      break;
    case 8:
      return strip.Color(0, 0, 0);
      break;
  }
}

void startAdv(void)
{
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addService(wagonService);
  Bluefruit.ScanResponse.addName();
  Bluefruit.Advertising.start();
}
//
//uint32_t palette[10],
//  red = strip.Color(255, 0, 0),
//  green = strip.Color(0, 255, 0),
//  blue = strip.Color(0, 0, 255),
//  yellow = strip.Color(255, 255, 0),
//  orange = strip.Color(255, 100, 0),
//  pink = strip.Color(255, 105, 180),
//  purple = strip.Color(128, 0, 128),
//  white = strip.Color(255, 255, 255);
void setupFlowers() {
  flowerPalette[0] = pink;
  flowerPalette[1] = yellow;
  flowerPalette[2] = red;
  flowerPalette[3] = white;
  flowerPalette[4] = pink;
  flowerPalette[5] = purple;
  flowerPalette[6] = purple;
  flowerPalette[7] = pink;
  flowerPalette[8] = pink;
  flowerPalette[9] = red;
  flowerPalette[10] = purple;
  flowerPalette[11] = yellow;
  flowerPalette[12] = red;
  flowerPalette[13] = pink;
  flowerPalette[14] = pink;
  flowerPalette[15] = yellow;
  flowerPalette[16] = purple;
  flowerPalette[17] = pink;
  flowerPalette[18] = pink;
  flowerPalette[19] = yellow;
  flowerPalette[20] = pink;
  flowerPalette[21] = red;
  flowerPalette[22] = purple;
  flowerPalette[23] = pink;
  flowerPalette[24] = purple;
  flowerPalette[25] = purple;
  flowerPalette[26] = pink;
  flowerPalette[27] = red;
  flowerPalette[28] = purple;
  flowerPalette[29] = red;
  flowerPalette[30] = yellow;
  flowerPalette[31] = purple;
  flowerPalette[32] = red;
  flowerPalette[33] = pink;
  flowerPalette[34] = purple;
  flowerPalette[35] = purple;
  flowerPalette[36] = yellow;
  flowerPalette[37] = pink;
  flowerPalette[38] = pink;
  flowerPalette[39] = pink;
  flowerPalette[40] = yellow;
  flowerPalette[41] = purple;
  flowerPalette[42] = yellow;
  flowerPalette[43] = pink;
  flowerPalette[44] = pink;
  flowerPalette[45] = purple;
  flowerPalette[46] = pink;
  flowerPalette[47] = pink;
  flowerPalette[48] = yellow;
  flowerPalette[49] = pink;
  flowerPalette[50] = pink;
  flowerPalette[51] = purple;
  flowerPalette[52] = yellow;
  flowerPalette[53] = purple;
}

