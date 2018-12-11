/*Neopixel Code by group 6
 *BACKUP: https://github.com/narutoliu/DIP
 *Last editted 10/12/18 
 */
 
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <Servo.h>

Servo servo;
#define servopin 3

#define NUMBER_PIXEL 95
#define LEDPIN    6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMBER_PIXEL, LEDPIN, NEO_GRB + NEO_KHZ800);

int lng = 100;//long delay
int sht = 50;//short delay
int x = 0;
int soundLevel = 0;
int count = 0;
int dir = 0;


void setup()
{
  servo.attach(servopin);
  //start receiving
  Wire.begin(9); 
  
  Wire.onReceive(receiveEvent);
  
  strip.begin();//initialises neopixels
  strip.setBrightness(255);
  strip.show();//clears any previous data in the strip
  Serial.begin(9600);
}

void receiveEvent(int bytes) {
  soundLevel = Wire.read();    //read data from master arduino
  Serial.println("123");

  servo.write(dir);
  dir= dir + 10;

  if (dir==180){
    dir=0;
  }
  
  /*for(int pos = 180; pos > 0 ; pos--){
    servo.write(pos);
     }

  for(int pos = 180; pos > 0 ; pos--){
    servo.write(pos);
     }*/
}


void loop()
{
  //open void loop

  Serial.print("The volt level is  ");
  Serial.println(soundLevel);//for debugging

  //Play patterns depending on soundlevel
  if (soundLevel <= 0)
  {
    /*int level0Color = random(1, 3);
    //open if 0. When there is silence
    if (level0Color==1){
      Single_theaterChase(strip.Color(200, 0, 0), 95);
      } // Red
    if(level0Color==2){
      Single_theaterChase(strip.Color(0, 200, 0), 95);  
    }
    if(level0Color==3){
      Single_theaterChase(strip.Color(0, 0, 200), 95);
    }
      
    strip.show();
    delay(lng);*/
    meteorRain(0xff,0xff,0xff,10,95, true, 15);
   

  }//close if 0 statement


  if (soundLevel == 1)
  {
    meteorRain(0xff,0xff,0,10, 95, true, 15);

  }//end of if sound level 1 options


  if (soundLevel == 2)
  {
    //open level 2
    meteorRain(0xff,0,0xff,10,95, true, 15);
    
    
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
      theaterChase(strip.Color(200, 0, 0), 95); 
      strip.show();
      delay(lng);
      
    }else if(color==1){
      theaterChase(strip.Color(0, 200, 0), 95); 
      strip.show();
      delay(lng);
      
    }else{
      theaterChase(strip.Color(0, 0, 200), 95); 
      strip.show();
      delay(lng);
    }
  }//close if sound level 5


  else if (soundLevel == 6)
  {
    int color;
    color=random(0,2);
    if (color==0){
      theaterChase(strip.Color(200, 0, 0), 95); 
      strip.show();
      delay(lng);
      
    }else if(color==1){
      theaterChase(strip.Color(0, 200, 0), 95);
      strip.show();
      delay(lng);
      
    }else{
      theaterChase(strip.Color(0, 0, 200), 95);
      strip.show();
      delay(lng);
    }

    
  }//close if sound level 7

  else if (soundLevel == 7)
  {
    int color;
    color=random(0,2);
    if (color==0){
      theaterChase(strip.Color(200, 200, 0), 95); 
      strip.show();
      delay(lng);
      
    }else if(color==1){
      theaterChase(strip.Color(0, 200, 200), 95);
      strip.show();
      delay(lng);
      
    }else{
      theaterChase(strip.Color(200, 0, 200), 95);
      strip.show();
      delay(lng);
    }
    //open if sound level 8

    

  }//close if sound level 8

  else if (soundLevel == 8)
  {
    flash();

  }//close if sound level 9

  else if (soundLevel == 9)

  {
    flash();
    //Strobe(0xff, 0xff, 0xff, 5, 50, 1000);
  }//close if sound level 11

   
   
  //close if sound level 10

  
  else if (soundLevel >= 10)
  {
   int level10Color;
   level10Color = random(1, 4);
   if (level10Color == 1){
    Strobe(0xff, 0xff, 0xff, 5, 100, 1000); }
   if (level10Color== 2){
    Strobe(0, 0xff, 0xff, 5, 100, 1000); }
  if (level10Color == 3){
    Strobe(0xff, 0, 0xff, 5, 100, 1000); }
  if (level10Color == 4){
    Strobe(0xff, 0xff,0, 5, 100, 1000); } 
    //Strobe(0xff, 0xff, 0xff, 5, 50, 1000);
  }//close if sound level 11


}//close void loop()




//neopixel functions below
void flash(){
  //flash pattern the "BANG BANG BANG"
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
  for(int i=70;i<95;i++){
    strip.setPixelColor(i, 0, 0, 0);
  }
  
      strip.show();
      delay(sht);

 }


void colorWipe(uint32_t c, uint8_t wait)
{
  //colourwipe
  for(uint16_t i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}//close colorWipe function



void theaterChase(uint32_t c, uint8_t wait)
{
  //theaterchase
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
  //theaterchase but with only one light
  for (int j = 0; j < 3; j++) //do 3 cycles of chasing
  {
    for (int q = 0; q < 95; q++)
    {
      for (int i = 0; i < strip.numPixels(); i = i + 95)
      {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 95)
      {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}


void Split_theaterChase(uint32_t c, uint8_t wait)
{
  
  for (int j = 0; j < 3; j++) //do 3 cycles of chasing
  {
    for (int q = 0; q < 95; q++)
    {
      for (int i = 0; i < strip.numPixels(); i = i + 95)
      {
        strip.setPixelColor(i + q, c);  
        strip.setPixelColor(95-i - q, c);  
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 95)
      {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
        strip.setPixelColor(95-i - q, 0);
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
  
  for(int i = 0; i < 95; i++) {
    
    
    // fade brightness 
    for(int j=0; j<95; j++) {
      if( (!meteorRandomDecay) || (random(10)>5) ) {
        fadeToBlack(j, meteorTrailDecay );        
      }
    }
    
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
      if( ( i-j <95) && (i-j>=0) ) {
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
    int RGBj;
    RGBj=random(0,2);
    // Fade IN
    for(int k = 0; k < 256; k=k+2) { 
      switch(RGBj) { 
        case 0: setAll(k,0,0); break;
        case 1: setAll(0,k,0); break;
        case 2: setAll(0,0,k); break;
      }
      strip.show();
      delay(3);
    }
    // Fade OUT
    for(int k = 255; k >= 0; k=k-2) { 
      switch(RGBj) { 
        case 0: setAll(k,0,0); break;
        case 1: setAll(0,k,0); break;
        case 2: setAll(0,0,k); break;
      }
      strip.show();
      delay(3);
    }
  
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < 95; i++ ) {
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
