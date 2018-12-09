
//Sound Level to light show sketch for the
//autogain microphone Amplifier from Adafruit on pin AO
//plus neopixel led functions (pin 6) mapped on to different sound levels to give music to light effects
//
//*************

//lines below set variables for neopixels
//hihi
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <Servo.h>

Servo servo;
#define servopin 3

#define NUMBER_PIXEL 90
#define LEDPIN    6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMBER_PIXEL, LEDPIN, NEO_GRB + NEO_KHZ800);

int lng = 100;//long delay
int sht = 50;//short delay
int x = 0;
int soundLevel = 0;

//lines below are for the microphone sampling from Adafruit autogain mic

const int sampleWindow = 200; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

void setup()
{
  servo.attach(servopin);
  // Start the I2C Bus as Slave on address 9
  Wire.begin(9); 
  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent);
  
  strip.begin();//initialises neopixels
  strip.setBrightness(255);// set brightness from 0 to max is 255
  strip.show();//clears any previous data in the strip
  Serial.begin(9600);//set up for checking mic is working
}

void receiveEvent(int bytes) {
  soundLevel = Wire.read();    // read one character from the I2C
  Serial.println("123");
  
}

void loop()
{
  //open void loop
  //first run the sound sampling
  for(int pos = 0; pos <= 180; pos++){
    servo.write(pos);
    delay(10);
  }
  for(int pos = 180; pos > 0 ; pos--){
    servo.write(pos);
    delay(20);
  }
  
  Serial.print("The volt level is  ");
  Serial.println(soundLevel);//for debugging

//next section is a series of 12 (0-11) 'if' statements which trigger different patterns.
//it is a combination of a traditional VU style meter fill of the strip
// combined with randomised animated patterns to keep it interesting
  if (soundLevel == 0)
  {
    int level0Color = random(1, 3);
    //open if 0. When there is silence a rainbow pattern runs
    if (level0Color==1){
      Single_theaterChase(strip.Color(200, 0, 0), 80);
      } // Red
    if(level0Color==2){
      Single_theaterChase(strip.Color(0, 200, 0), 80);  
    }
    if(level0Color==3){
      Single_theaterChase(strip.Color(0, 0, 200), 80);
    }
      
    strip.show();
    delay(lng);
   

  }//close if 0 statement


  if (soundLevel == 1)
  {
    //open level 1 if statement which contains 4 randomised options
    meteorRain(0xff,0xff,0xff,10, 90, true, 15);

  }//end of if sound level 1 options


  if (soundLevel == 2)
  {
    //open level 2
    meteorRain(0xff,0xff,0xff,10, 90, true, 15);
    
    
  }//close level 2

  if (soundLevel == 3)
  {
    //open if sound level 3
     RGBLoop();
    
  }//close level 3


  if (soundLevel == 4)
  {
    rainbowCycle(2);
    
  }//close if sound level 4

  else if (soundLevel == 5)
  {
    //open if sound level 5
        int color;
    color=random(0,2);
    if (color==0){
      theaterChase(strip.Color(200, 0, 0), 80); // Red
      strip.show();
      delay(lng);
      
    }else if(color==1){
      theaterChase(strip.Color(0, 200, 0), 80); // Red
      strip.show();
      delay(lng);
      
    }else{
      theaterChase(strip.Color(0, 0, 200), 80); // Red
      strip.show();
      delay(lng);
    }
  }//close if sound level 5


  else if (soundLevel == 6)
  {
    int color;
    color=random(0,2);
    if (color==0){
      theaterChase(strip.Color(200, 0, 0), 80); // Red
      strip.show();
      delay(lng);
      
    }else if(color==1){
      theaterChase(strip.Color(0, 200, 0), 80); // Red
      strip.show();
      delay(lng);
      
    }else{
      theaterChase(strip.Color(0, 0, 200), 80); // Red
      strip.show();
      delay(lng);
    }

    
  }//close if sound level 7

  else if (soundLevel == 7)
  {
    int color;
    color=random(0,2);
    if (color==0){
      theaterChase(strip.Color(200, 200, 0), 80); // Red
      strip.show();
      delay(lng);
      
    }else if(color==1){
      theaterChase(strip.Color(0, 200, 200), 80); // Red
      strip.show();
      delay(lng);
      
    }else{
      theaterChase(strip.Color(200, 0, 200), 80); // Red
      strip.show();
      delay(lng);
    }
    //open if sound level 8

    

  }//close if sound level 8

  else if (soundLevel == 8)
  {
    flash();
    //close option 9

  }//close if sound level 9

  else if (soundLevel == 9)

  {
    flash();
    //Strobe(0xff, 0xff, 0xff, 5, 50, 1000);
  }//close if sound level 11

   
   
  //close if sound level 10


  else if (soundLevel >= 10)
  {
   int level10Color = random(1, 4);
   if (int level0Color = 1){
    Strobe(0xff, 0xff, 0xff, 5, 100, 1000); }
   if (int leve1l0Color = 2){
    Strobe(0, 0xff, 0xff, 5, 100, 1000); }
  if (int level10Color = 3){
    Strobe(0xff, 0, 0xff, 5, 100, 1000); }
  if (int level10Color = 4){
    Strobe(0xff, 0xff,0, 5, 100, 1000); } 
    //Strobe(0xff, 0xff, 0xff, 5, 50, 1000);
  }//close if sound level 11

}//close void loop()

//neopixel functions below



void flash(){
  for(int i=0;i<10;i++){
    strip.setPixelColor(i, 255, 0, 0);
  }
  for(int i=10;i<20;i++){
    strip.setPixelColor(i, 0, 0, 0);
  }
    
      
      strip.show();
      delay(lng);

  for(int i=20;i<30;i++){
    strip.setPixelColor(i, 0, 255, 0);
  }
  for(int i=30;i<40;i++){
    strip.setPixelColor(i, 0, 0, 0);
  }
   
      strip.show();
      delay(lng);
      
  for(int i=40;i<50;i++){
    strip.setPixelColor(i, 0, 0, 255);
  }
  for(int i=50;i<60;i++){
    strip.setPixelColor(i, 0, 0, 0);
  }
  
      strip.show();
      delay(lng);
  
  for(int i=60;i<70;i++){
    strip.setPixelColor(i, 255, 255, 0);
  }
  for(int i=70;i<81;i++){
    strip.setPixelColor(i, 0, 0, 0);
  }
  
      strip.show();
      delay(sht);

 }//end of if sound level 1 options
  



void colorWipe(uint32_t c, uint8_t wait)
{
  //open colowipe
  for(uint16_t i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}//close colorWipe function



void theaterChase(uint32_t c, uint8_t wait)
{
  //open theaterchase function
  for (int j = 0; j < 3; j++) //do 3 cycles of chasing
  {
    for (int q = 0; q < 3; q++)
    {
      for (int i = 0; i < strip.numPixels(); i = i + 3)
      {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3)
      {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

  


void Single_theaterChase(uint32_t c, uint8_t wait)
{
  //open theaterchase function
  for (int j = 0; j < 3; j++) //do 3 cycles of chasing
  {
    for (int q = 0; q < 90; q++)
    {
      for (int i = 0; i < strip.numPixels(); i = i + 90)
      {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 90)
      {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}//close theater chase function


void Split_theaterChase(uint32_t c, uint8_t wait)
{
  //open theaterchase function
  for (int j = 0; j < 3; j++) //do 3 cycles of chasing
  {
    for (int q = 0; q < 90; q++)
    {
      for (int i = 0; i < strip.numPixels(); i = i + 90)
      {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
        strip.setPixelColor(90-i - q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 90)
      {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
        strip.setPixelColor(90-i - q, 0);
      }
    }
  }
}






void rainbowCycle(uint8_t wait)  //open rainbow function
{
  uint16_t i, j;

  for(j = 0; j < 256; j++) // 1 cycles of all colors on wheel
  {
    for(i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(10);
  }
}//close rainbow function
//you need to included the code below for the neopixel functions that use the 'wheel' code:
uint32_t Wheel(byte WheelPos)
{
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85)
  {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170)
  {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


//FIRE
void Fire(int Cooling, int Sparking, int SpeedDelay) {
  static byte heat[90];
  int cooldown;
  
  // Step 1.  Cool down every cell a little
  for( int i = 0; i < 90; i++) {
    cooldown = random(0, ((Cooling * 10) / 90) + 2);
    
    if(cooldown>heat[i]) {
      heat[i]=0;
    } else {
      heat[i]=heat[i]-cooldown;
    }
  }
  
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k= 90 - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
    
  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160,255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for( int j = 0; j < 90; j++) {
    setPixelHeatColor(j, heat[j] );
  }

  strip.show();
  delay(SpeedDelay);
}

void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature/255.0)*191);
 
  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
 
  // figure out which third of the spectrum we're in:
  if( t192 > 0x80) {                     // hottest
    strip.setPixelColor(Pixel, 255, 255, heatramp);
  } else if( t192 > 0x40 ) {             // middle
    strip.setPixelColor(Pixel, 255, heatramp, 0);
  } else {                               // coolest
    strip.setPixelColor(Pixel, heatramp, 0, 0);
  }
} 




//meteor
void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {  
        strip.setPixelColor(0, 0, 0, 0);  
      strip.setPixelColor(1, 0, 0, 0);  
      strip.setPixelColor(2, 0, 0, 0);  
      strip.setPixelColor(3, 0, 0, 0);  
      strip.setPixelColor(4, 0, 0, 0);  
      strip.setPixelColor(5, 0, 0, 0);  
      strip.setPixelColor(6, 0, 0, 0);  
      strip.setPixelColor(7, 0, 0, 0);  
      strip.setPixelColor(8, 0, 0, 0);  
      strip.setPixelColor(9, 0, 0, 0);  
      strip.setPixelColor(10, 0, 0, 0);  
      strip.setPixelColor(11, 0, 0, 0);
      strip.setPixelColor(12, 0, 0, 0); 
      strip.setPixelColor(13, 0, 0, 0); 
      strip.setPixelColor(14, 0, 0, 0); 
  
  for(int i = 0; i < 80+80; i++) {
    
    
    // fade brightness all LEDs one step
    for(int j=0; j<80; j++) {
      if( (!meteorRandomDecay) || (random(10)>5) ) {
        fadeToBlack(j, meteorTrailDecay );        
      }
    }
    
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
      if( ( i-j <80) && (i-j>=0) ) {
        strip.setPixelColor(i-j, red, green, blue);
      } 
    }
   
    strip.show();
    delay(SpeedDelay);
  }
}

void fadeToBlack(int ledNo, byte fadeValue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
    // NeoPixel
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;
    
    oldColor = strip.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
    
    strip.setPixelColor(ledNo, r,g,b);
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[ledNo].fadeToBlackBy( fadeValue );
 #endif  
}


void RGBLoop(){
    int j;
    j=random(0,2);
    // Fade IN
    for(int k = 0; k < 256; k=k+2) { 
      switch(j) { 
        case 0: setAll(k,0,0); break;
        case 1: setAll(0,k,0); break;
        case 2: setAll(0,0,k); break;
      }
      strip.show();
      delay(3);
    }
    // Fade OUT
    for(int k = 255; k >= 0; k=k-2) { 
      switch(j) { 
        case 0: setAll(k,0,0); break;
        case 1: setAll(0,k,0); break;
        case 2: setAll(0,0,k); break;
      }
      strip.show();
      delay(3);
    }
  
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < 80; i++ ) {
    strip.setPixelColor(i, red, green, blue); 
  }
  strip.show();
}

void Strobe(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause){
  for(int j = 0; j < StrobeCount; j++) {
    setAll(red,green,blue);
    strip.show();
    delay(FlashDelay);
    setAll(0,0,0);
    strip.show();
    delay(FlashDelay);
  }
 
 delay(FlashDelay);
}

//end
