
/*----------------------------------------------------------------------------------------------------------------------------------------*/

//Import Libraries
import processing.video.*;
import processing.serial.*;
import ddf.minim.*;
import ddf.minim.analysis.*;

/*----------------------------------------------------------------------------------------------------------------------------------------*/

//Global Variables
Serial arduino; //for serial communication with Arduino
Capture video;  //for capturing video from cam

//Audio stuff
Minim minim;
AudioInput input;
AudioPlayer player;
FFT fft;

byte[] rgb_matrix = new byte[512]; //This is the matrix send to Arduino for each frame

//For display on PC  
byte pixel_diameter = 57 / 4;
byte pixel_pitch    = 75 / 4;

byte mode = 0;
long end_time = 0;

//For "Boxes"-Function
byte[] boxes_col = new byte[16];
byte[] boxes_pos = new byte[16];
byte[] boxes_dir = new byte[16];

//For "Pong"-Function
byte[] pong_pos_x = new byte[5];
byte[] pong_pos_y = new byte[5];
byte[] pong_dir_x = new byte[5];
byte[] pong_dir_y = new byte[5];
byte[] pong_col   = new byte[5];

/*----------------------------------------------------------------------------------------------------------------------------------------*/

void setup() 
{
  size(pixel_pitch * 32, pixel_pitch * 16, JAVA2D);
  
  smooth();
  noStroke();
  colorMode(RGB, 255, 255, 255);
  
  //Establish new serial cennection
  arduino = new Serial(this, Serial.list()[1], 115200); //Select Serial Port here!

  //Use the default video input device
  //video = new Capture(this, 320, 240, 15);  //Last number is frames per second
  
  minim = new Minim (this);
  input = minim.getLineIn (Minim.STEREO, 2048); //Use for "Line_IN" as input
  //player = minim.loadFile ("lied.mp3", 2048);    //This would be use for playing an mp3 directly
  //player.loop (); //Sart the playback of mp3 (endless)
  
  fft = new FFT (input.bufferSize (), input.sampleRate ()); //again this is used for "Line_IN" input
  //fft = new FFT (player.bufferSize (), player.sampleRate ());
  fft.logAverages(22,4); //Our FFT starts at 22Hz and each octave is devided into 4 bands
  
  background(0);
  
  //Random start pattern for "Boxes"-function
  for (byte x=0; x<16; x++)
  {
    boxes_col[x] = (byte) random(6);
    boxes_pos[x] = (byte) random(14);
    boxes_dir[x] = (byte) random(2);
  }
  
  //Random start pattern for "Pong"-function
  for (byte i=0; i<5; i++)
  {
    pong_pos_x[i] = (byte) random(32);
    pong_dir_x[i] = (byte) random(2);
    pong_pos_y[i] = (byte) random(16);
    pong_dir_y[i] = (byte) random(2);
    pong_col[i]   = (byte) random(6);
  }
  
}

/*----------------------------------------------------------------------------------------------------------------------------------------*/

void draw() 
{ 
  
  //if time has come get new mode and new duration
  if (millis() > end_time) 
  {
    end_time = millis() + (long)(random(5,20) * 1000);
    mode = (byte)(random(1,9));
    
    //Random start pattern for "Boxes"-function
    for (byte x=0; x<16; x++)
    {
      boxes_col[x] = (byte) random(6);
      boxes_pos[x] = (byte) random(14);
      boxes_dir[x] = (byte) random(2);
    }
  
    //Random start pattern for "Pong"-function
    for (byte i=0; i<5; i++)
    {
      pong_pos_x[i] = (byte) random(32);
      pong_dir_x[i] = (byte) random(2);
      pong_pos_y[i] = (byte) random(16);
      pong_dir_y[i] = (byte) random(2);
      pong_col[i]   = (byte) random(6);
    } 
  }
  
  switch (mode)
  {
    //case 0: render_frame_from_web_cam(); break;
    case 1:                     boxes(); break;
    case 2:                 plasma_01(); break;
    case 3:                 plasma_02(); break;
    case 4:                 plasma_03(); break;
    case 5:         spectrum_analyzer(); break;
    case 6:                      pong(); break;
    case 7:                  ellipses(); break;
    case 8:              stereo_boxes(); break;
  }
  
  //Whatever was calculated, send it to Arduino! ;-)
  arduino.write(255); //Start command for Arduino
  arduino.write(rgb_matrix);
    
} 

/*----------------------------------------------------------------------------------------------------------------------------------------*/


//Subroutines

/*----------------------------------------------------------------------------------------------------------------------------------------*/

void boxes()
{  
  //Local Variables
  int scaled_value_r  = 0;
  int scaled_value_g  = 0;
  int scaled_value_b  = 0;
 
  float value_r = 0;
  float value_g = 0;
  float value_b = 0;
  
  //Clear processing window and RGB-Matrix     
  for (byte i=0; i<16; i++)
  {
    for (byte j=0; j<32; j++)
    {
      fill(0);
      ellipse(j * pixel_pitch + pixel_pitch / 2, i * pixel_pitch + pixel_pitch / 2, pixel_diameter, pixel_diameter);
      rgb_matrix[i*32+j] = 0;
    }
  } 
   
  for (byte x = 0; x < 16; x++)
  {
    byte x_1 = (byte)(2*x);
    byte x_2 = (byte)(2*x);
    byte x_3 = (byte)(2*x+1);
    byte x_4 = (byte)(2*x+1);
        
    byte y_1 = (byte)(boxes_pos[x]    );
    byte y_2 = (byte)(boxes_pos[x] + 1);
    byte y_3 = (byte)(boxes_pos[x]    );
    byte y_4 = (byte)(boxes_pos[x] + 1);       
                
    switch (boxes_col[x])
    {
      case 0: scaled_value_r = 5; scaled_value_g = 0; scaled_value_b = 0; break;
      case 1: scaled_value_r = 5; scaled_value_g = 5; scaled_value_b = 0; break;
      case 2: scaled_value_r = 0; scaled_value_g = 5; scaled_value_b = 0; break;
      case 3: scaled_value_r = 0; scaled_value_g = 5; scaled_value_b = 5; break;
      case 4: scaled_value_r = 0; scaled_value_g = 0; scaled_value_b = 5; break;
      case 5: scaled_value_r = 5; scaled_value_g = 0; scaled_value_b = 5; break;
    }
    
    rgb_matrix[y_1*32+x_1] = (byte)(36*scaled_value_r + 6*scaled_value_g + scaled_value_b);
    rgb_matrix[y_2*32+x_2] = (byte)(36*scaled_value_r + 6*scaled_value_g + scaled_value_b);
    rgb_matrix[y_3*32+x_3] = (byte)(36*scaled_value_r + 6*scaled_value_g + scaled_value_b);
    rgb_matrix[y_4*32+x_4] = (byte)(36*scaled_value_r + 6*scaled_value_g + scaled_value_b);
   
    value_r = scaled_value_r * 51;
    value_g = scaled_value_g * 51;
    value_b = scaled_value_b * 51; 
        
    //Draw current pixel in Processing Window
    fill(value_r, value_g, value_b);
    ellipse(x_1 * pixel_pitch + pixel_pitch / 2, y_1 * pixel_pitch + pixel_pitch / 2, pixel_diameter, pixel_diameter);
    ellipse(x_2 * pixel_pitch + pixel_pitch / 2, y_2 * pixel_pitch + pixel_pitch / 2, pixel_diameter, pixel_diameter);
    ellipse(x_3 * pixel_pitch + pixel_pitch / 2, y_3 * pixel_pitch + pixel_pitch / 2, pixel_diameter, pixel_diameter);
    ellipse(x_4 * pixel_pitch + pixel_pitch / 2, y_4 * pixel_pitch + pixel_pitch / 2, pixel_diameter, pixel_diameter);    
        
    //caculated new Position
    if (boxes_pos[x] == 0 ) {boxes_dir[x] = 1;}
    if (boxes_pos[x] == 14) {boxes_dir[x] = 0;}
    if (boxes_dir[x] == 1 ) {boxes_pos[x]++;} else {boxes_pos[x]--;}         
  }
  
  delay(30);
}

/*----------------------------------------------------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------------------------------------------------*/

void ellipses()
{  
  //Local Variables
  int timeDisplacement = frameCount;
  int pixelSize=20;
  
  float  xc = 25;
  float calculation1 = sin( radians(timeDisplacement * 0.61655617));
  float calculation2 = sin( radians(timeDisplacement * -3.6352262));
  
  float a = 25.0 * input.mix.level();
  float b = 45.0 * input.mix.level();
  float e =        input.mix.level();
        
  for (byte i=0; i<16; i++)
  {
    float  yc    = 25;
    float s1 = 128 + 128 * sin(radians(xc) * calculation1);
    xc += pixelSize;
    
    for (byte j=0; j<32; j++)
    {
      if ( (((i-7.5)*(i-7.5)/(a*a) + (j-15.5)*(j-15.5)/(b*b)) < (1+e)) && (((i-7.5)*(i-7.5)/(a*a) + (j-15.5)*(j-15.5)/(b*b)) > (1-e)) )
      {
        yc += pixelSize;
        float s2 = 128 + 128 * sin(radians(yc) * calculation2 );
        float s3 = 128 + 128 * sin(radians((xc + yc + timeDisplacement * 5) / 2));  
        float s  = (s1+ s2 + s3) / 3;
      
        colorMode(HSB);
      
        int temp_color = color(s, 255 - s / 2.0, 255);
      
        colorMode(RGB, 255, 255, 255);
      
        int scaled_value_r = (byte)(  red(temp_color) / 43);
        int scaled_value_g = (byte)(green(temp_color) / 43);
        int scaled_value_b = (byte)( blue(temp_color) / 43);
      
        rgb_matrix[i*32+j] = (byte)(36*scaled_value_r + 6*scaled_value_g + scaled_value_b);
   
        float value_r = scaled_value_r * 51;
        float value_g = scaled_value_g * 51;
        float value_b = scaled_value_b * 51; 
        
        //Draw current pixel in Processing Window
        fill(value_r, value_g, value_b);
      }
      else
      {
        fill(0);
        rgb_matrix[i*32+j] = 0;
      }
      ellipse(j * pixel_pitch + pixel_pitch / 2, i * pixel_pitch + pixel_pitch / 2, pixel_diameter, pixel_diameter);
    }
  } 
}

/*----------------------------------------------------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------------------------------------------------*/

void plasma_01()
{     
  int timeDisplacement = frameCount;
  int pixelSize=30;
  
  float  xc = 25;
  float calculation1 = sin( radians(timeDisplacement * 0.61655617));
  float calculation2 = sin( radians(timeDisplacement * -3.6352262));
    
  for (byte x = 0; x < 32; x++)
  {     
    float  yc    = 25;
    float s1 = 128 + 128 * sin(radians(xc) * calculation1);
    xc += pixelSize;
    
    for (byte y = 0; y < 16; y++)
    {
      yc += pixelSize;
      float s2 = 128 + 128 * sin(radians(yc) * calculation2);
      float s3 = 128 + 128 * sin(radians((xc + yc + timeDisplacement * 5) / 2));  
      float s  = (s1+ s2 + s3) / 3;
      
      colorMode(HSB);
      
      //int temp_color = color(s, 255 - s / 2.0, 255);
      int temp_color = color(s, 255, 255);
      
      colorMode(RGB, 255, 255, 255);
      
      int scaled_value_r = (byte)(  red(temp_color) / 43);
      int scaled_value_g = (byte)(green(temp_color) / 43);
      int scaled_value_b = (byte)( blue(temp_color) / 43);
      
      rgb_matrix[y*32+x] = (byte)(36*scaled_value_r + 6*scaled_value_g + scaled_value_b);
   
      float value_r = scaled_value_r * 51;
      float value_g = scaled_value_g * 51;
      float value_b = scaled_value_b * 51; 
        
      //Draw current pixel in Processing Window
      fill(value_r, value_g, value_b);
      ellipse(x * pixel_pitch + pixel_pitch / 2, y * pixel_pitch + pixel_pitch / 2, pixel_diameter, pixel_diameter);
    }            
  }
}

/*----------------------------------------------------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------------------------------------------------*/

void plasma_02()
{     
  int timeDisplacement = frameCount;
  
  int pixelSize = (int) (input.mix.level () * 70); //Here the "Size" of the plasma is driven be our audio input
  
  float  xc = 60;
  float calculation1 = sin( radians(timeDisplacement * 0.61655617));
  float calculation2 = sin( radians(timeDisplacement * -3.6352262));
    
  for (byte x = 0; x < 32; x++)
  {     
    float  yc    = 25;
    float s1 = 128 + 128 * sin(radians(xc) * calculation1 );
    xc += pixelSize;
    
    for (byte y = 0; y < 16; y++)
    {
      yc += pixelSize;
      float s2 = 128 + 128 * sin(radians(yc) * calculation2 );
      float s3 = 128 + 128 * sin(radians((xc + yc + timeDisplacement * 5) / 2));  
      float s  = (s1+ s2 + s3) / 3;
      
      colorMode(HSB);
      
      //int temp_color = color(s, 255 - s / 2.0, 255);
      int temp_color = color(s, 255, 255);
      
      colorMode(RGB, 255, 255, 255);
      
      int scaled_value_r = (byte)(  red(temp_color) / 43);
      int scaled_value_g = (byte)(green(temp_color) / 43);
      int scaled_value_b = (byte)( blue(temp_color) / 43);
      
      rgb_matrix[y*32+x] = (byte)(36*scaled_value_r + 6*scaled_value_g + scaled_value_b);
   
      float value_r = scaled_value_r * 51;
      float value_g = scaled_value_g * 51;
      float value_b = scaled_value_b * 51; 
        
      //Draw current pixel in Processing Window
      fill(value_r, value_g, value_b);
      ellipse(x * pixel_pitch + pixel_pitch / 2, y * pixel_pitch + pixel_pitch / 2, pixel_diameter, pixel_diameter);
    }            
  }
}

/*----------------------------------------------------------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------------------------------------------------------*/

void plasma_03()
{     
  int timeDisplacement = 20;
  int pixelSize= (int) (80*sin( radians(timeDisplacement * 0.61655617)));
  
  float  xc = 10 * frameCount;
  float calculation1 = sin( radians(timeDisplacement * 0.61655617));
  float calculation2 = sin( radians(timeDisplacement * -3.6352262));
    
  for (byte x = 0; x < 32; x++)
  {     
    float  yc    = -15 * frameCount;
    float s1 = 128 + 128 * sin(radians(xc) * calculation1);
    xc += pixelSize;
    
    for (byte y = 0; y < 16; y++)
    {
      yc += pixelSize;
      float s2 = 128 + 128 * sin(radians(yc) * calculation2);
      float s3 = 128 + 128 * sin(radians((xc + yc + timeDisplacement * 5) / 2));  
      float s  = (s1+ s2 + s3) / 3;
      
      colorMode(HSB);
      
      //int temp_color = color(s, 255 - s / 2.0, 255);
      int temp_color = color(s, 255, 255);
      
      colorMode(RGB, 255, 255, 255);
      
      int scaled_value_r = (byte)(  red(temp_color) / 43);
      int scaled_value_g = (byte)(green(temp_color) / 43);
      int scaled_value_b = (byte)( blue(temp_color) / 43);
      
      rgb_matrix[y*32+x] = (byte)(36*scaled_value_r + 6*scaled_value_g + scaled_value_b);
   
      float value_r = scaled_value_r * 51;
      float value_g = scaled_value_g * 51;
      float value_b = scaled_value_b * 51; 
        
      //Draw current pixel in Processing Window
      fill(value_r, value_g, value_b);
      ellipse(x * pixel_pitch + pixel_pitch / 2, y * pixel_pitch + pixel_pitch / 2, pixel_diameter, pixel_diameter);
    }            
  }
}

/*----------------------------------------------------------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------------------------------------------------------*/

void pong()
{  
  //Local Variables
  int scaled_value_r  = 0;
  int scaled_value_g  = 0;
  int scaled_value_b  = 0;
 
  float value_r = 0;
  float value_g = 0;
  float value_b = 0;
    
  //Clear processing window and RGB-Matrix     
  for (byte i=0; i<16; i++)
  {
    for (byte j=0; j<32; j++)
    {
      fill(0);
      ellipse(j * pixel_pitch + pixel_pitch / 2, i * pixel_pitch + pixel_pitch / 2, pixel_diameter, pixel_diameter);
      rgb_matrix[i*32+j] = 0;
    }
  } 
   
  for (byte i = 0; i < 5; i++)
  {
    
    byte x = pong_pos_x[i];
    byte y = pong_pos_y[i];
    
    switch (pong_col[i])
    {
      case 0: scaled_value_r = 5; scaled_value_g = 0; scaled_value_b = 0; break;
      case 1: scaled_value_r = 5; scaled_value_g = 5; scaled_value_b = 0; break;
      case 2: scaled_value_r = 0; scaled_value_g = 5; scaled_value_b = 0; break;
      case 3: scaled_value_r = 0; scaled_value_g = 5; scaled_value_b = 5; break;
      case 4: scaled_value_r = 0; scaled_value_g = 0; scaled_value_b = 5; break;
      case 5: scaled_value_r = 5; scaled_value_g = 0; scaled_value_b = 5; break;
    }
    
    rgb_matrix[y*32+x] = (byte)(36*scaled_value_r + 6*scaled_value_g + scaled_value_b);
    rgb_matrix[y*32+x] = (byte)(36*scaled_value_r + 6*scaled_value_g + scaled_value_b);
    rgb_matrix[y*32+x] = (byte)(36*scaled_value_r + 6*scaled_value_g + scaled_value_b);
    rgb_matrix[y*32+x] = (byte)(36*scaled_value_r + 6*scaled_value_g + scaled_value_b);
   
    value_r = scaled_value_r * 51;
    value_g = scaled_value_g * 51;
    value_b = scaled_value_b * 51; 
        
    //Draw current pixel in Processing Window
    fill(value_r, value_g, value_b);
    ellipse(x * pixel_pitch + pixel_pitch / 2, y * pixel_pitch + pixel_pitch / 2, pixel_diameter, pixel_diameter);
           
    if (pong_pos_x[i] == 0 ) {pong_dir_x[i] = 1;}
    if (pong_pos_x[i] == 31) {pong_dir_x[i] = 0;}
    if (pong_dir_x[i] == 1 ) {pong_pos_x[i]++;} else {pong_pos_x[i]--;}

    if (pong_pos_y[i] == 0 ) {pong_dir_y[i] = 1;}
    if (pong_pos_y[i] == 15) {pong_dir_y[i] = 0;}
    if (pong_dir_y[i] == 1 ) {pong_pos_y[i]++;} else {pong_pos_y[i]--;}    
  }
  
  delay(30);
}

/*----------------------------------------------------------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------------------------------------------------------*/

void spectrum_analyzer()
{         
  
  int timeDisplacement = frameCount;
  int pixelSize=20;
  
  float  xc = 25;
  float calculation1 = sin( radians(timeDisplacement * 0.61655617));
  float calculation2 = sin( radians(timeDisplacement * -3.6352262));
  
  //Clear processing window and RGB-Matrix     
  for (byte i=0; i<16; i++)
  {
    for (byte j=0; j<32; j++)
    {
      fill(0);
      ellipse(j * pixel_pitch + pixel_pitch / 2, i * pixel_pitch + pixel_pitch / 2, pixel_diameter, pixel_diameter);
      rgb_matrix[i*32+j] = 0;
    }
  } 
      
  fft.forward(input.mix); //Calculated the current FFT
    
  for (byte x = 0; x < 32; x++)
  {         
    float  yc    = 25;
    float s1 = 128 + 128 * sin(radians(xc) * calculation1);
    xc += pixelSize;
    
    for (byte y = 0; (y < (fft.getAvg(x+1)*0.2) && (y < 16)); y++)
    { 
      yc += pixelSize;
      float s2 = 128 + 128 * sin(radians(yc) * calculation2 );
      float s3 = 128 + 128 * sin(radians((xc + yc + timeDisplacement * 5) / 2));  
      float s  = (s1+ s2 + s3) / 3;
      
      colorMode(HSB);
      
      int temp_color = color(s, 255 - s / 2.0, 255);
      
      colorMode(RGB, 255, 255, 255);
      
      int scaled_value_r = (byte)(  red(temp_color) / 43);
      int scaled_value_g = (byte)(green(temp_color) / 43);
      int scaled_value_b = (byte)( blue(temp_color) / 43);
      
      rgb_matrix[(15-y)*32+x] = (byte)(36*scaled_value_r + 6*scaled_value_g + scaled_value_b);
   
      float value_r = scaled_value_r * 51;
      float value_g = scaled_value_g * 51;
      float value_b = scaled_value_b * 51; 
        
      //Draw current pixel in Processing Window
      fill(value_r, value_g, value_b);
      ellipse(x * pixel_pitch + pixel_pitch / 2, (15-y) * pixel_pitch + pixel_pitch / 2, pixel_diameter, pixel_diameter);
    }            
  }
}

/*----------------------------------------------------------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------------------------------------------------------*/

void stereo_boxes()
{     
  int timeDisplacement = frameCount;
  int pixelSize = 40;
  
  int left  = (int) (input.left.level  () * 30);
  int right = (int) (input.right.level () * 30);
  if (left  > 8) {left  = 8;}
  if (right > 8) {right = 8;}
  int offset_left_x = 8 - left;
  int offset_left_y = 8 - left;
  int offset_right_x = 24 - right;
  int offset_right_y = 8  - right;
  
  float  xc = 40;
  float calculation1 = sin( radians(timeDisplacement * 0.61655617));
  float calculation2 = sin( radians(timeDisplacement * -3.6352262));
  
  //Clear processing window and RGB-Matrix     
  for (byte i=0; i<16; i++)
  {
    for (byte j=0; j<32; j++)
    {
      fill(0);
      ellipse(j * pixel_pitch + pixel_pitch / 2, i * pixel_pitch + pixel_pitch / 2, pixel_diameter, pixel_diameter);
      rgb_matrix[i*32+j] = 0;
    }
  } 
  
  //draw frame of left box  
  for (int x = offset_left_x; x < (offset_left_x + 2*left); x++)
  {     
    float  yc    = 25;
    float s1 = 128 + 128 * sin(radians(xc) * calculation1 );
    xc += pixelSize;
    
    for (int y = offset_left_y; y < (offset_left_y + 2*left); y++)
    {
      yc += pixelSize;
      float s2 = 128 + 128 * sin(radians(yc) * calculation2 );
      float s3 = 128 + 128 * sin(radians((xc + yc + timeDisplacement * 5) / 2));  
      float s  = (s1+ s2 + s3) / 3;
      
      colorMode(HSB);
      
      int temp_color = color(s, 255 - s / 2.0, 255);
      
      colorMode(RGB, 255, 255, 255);
      
      int scaled_value_r = (byte)(  red(temp_color) / 43);
      int scaled_value_g = (byte)(green(temp_color) / 43);
      int scaled_value_b = (byte)( blue(temp_color) / 43);
      
      rgb_matrix[y*32+x] = (byte)(36*scaled_value_r + 6*scaled_value_g + scaled_value_b);
   
      float value_r = scaled_value_r * 51;
      float value_g = scaled_value_g * 51;
      float value_b = scaled_value_b * 51; 
        
      //Draw current pixel in Processing Window
      fill(value_r, value_g, value_b);
      ellipse(x * pixel_pitch + pixel_pitch / 2, y * pixel_pitch + pixel_pitch / 2, pixel_diameter, pixel_diameter);
    }            
  }
  
  //draw frame of right box
  for (int x = offset_right_x; x < (offset_right_x + 2*right); x++)
  {     
    float  yc    = 25;
    float s1 = 128 + 128 * sin(radians(xc) * calculation1 );
    xc += pixelSize;
    
    for (int y = offset_right_y; y < (offset_right_y + 2*right); y++)
    {
      yc += pixelSize;
      float s2 = 128 + 128 * sin(radians(yc) * calculation2 );
      float s3 = 128 + 128 * sin(radians((xc + yc + timeDisplacement * 5) / 2));  
      float s  = (s1+ s2 + s3) / 3;
      
      colorMode(HSB);
      
      int temp_color = color(s, 255 - s / 2.0, 255);
      
      colorMode(RGB, 255, 255, 255);
      
      int scaled_value_r = (byte)(  red(temp_color) / 43);
      int scaled_value_g = (byte)(green(temp_color) / 43);
      int scaled_value_b = (byte)( blue(temp_color) / 43);
      
      rgb_matrix[y*32+x] = (byte)(36*scaled_value_r + 6*scaled_value_g + scaled_value_b);
   
      float value_r = scaled_value_r * 51;
      float value_g = scaled_value_g * 51;
      float value_b = scaled_value_b * 51; 
        
      //Draw current pixel in Processing Window
      fill(value_r, value_g, value_b);
      ellipse(x * pixel_pitch + pixel_pitch / 2, y * pixel_pitch + pixel_pitch / 2, pixel_diameter, pixel_diameter);
    }            
  }

  //fill insight of left box
  for (int x = offset_left_x + 1; x < (offset_left_x + 2*left - 1); x++)
  {     
    
    for (int y = offset_left_y +1; y < (offset_left_y + 2*left - 1); y++)
    {
      
      int scaled_value_r = 5;
      int scaled_value_g = 0;
      int scaled_value_b = 5;
      
      rgb_matrix[y*32+x] = (byte)(36*scaled_value_r + 6*scaled_value_g + scaled_value_b);
   
      float value_r = scaled_value_r * 51;
      float value_g = scaled_value_g * 51;
      float value_b = scaled_value_b * 51; 
        
      //Draw current pixel in Processing Window
      fill(value_r, value_g, value_b);
      ellipse(x * pixel_pitch + pixel_pitch / 2, y * pixel_pitch + pixel_pitch / 2, pixel_diameter, pixel_diameter);
    }            
  } 
  
  
  //fill insight of right box
  for (int x = offset_right_x + 1; x < (offset_right_x + 2*right - 1); x++)
  {     
    
    for (int y = offset_right_y +1; y < (offset_right_y + 2*right - 1); y++)
    {
      
      int scaled_value_r = 0;
      int scaled_value_g = 0;
      int scaled_value_b = 5;
      
      rgb_matrix[y*32+x] = (byte)(36*scaled_value_r + 6*scaled_value_g + scaled_value_b);
   
      float value_r = scaled_value_r * 51;
      float value_g = scaled_value_g * 51;
      float value_b = scaled_value_b * 51; 
        
      //Draw current pixel in Processing Window
      fill(value_r, value_g, value_b);
      ellipse(x * pixel_pitch + pixel_pitch / 2, y * pixel_pitch + pixel_pitch / 2, pixel_diameter, pixel_diameter);
    }            
  }  
  
  
}

/*----------------------------------------------------------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------------------------------------------------------*/

void render_frame_from_web_cam()
{
  //Local Variables
  int video_width  = video.width ;
  int video_height = video.height;
  
  byte scaled_value_r  = 0;
  byte scaled_value_g  = 0;
  byte scaled_value_b  = 0;

  if (video.available())
  { 
    video.read(); 
    video.loadPixels();     
      
    //Scale Video down from 320 x 240 to 32 x 16
    for (byte row = 0; row < 16; row++)
    {
      for (byte column = 0; column < 32; column++)
      {
        
        int start_position = (column * 10) + (row * 15) * video_width;
        
        float value_r = 0;
        float value_g = 0;
        float value_b = 0;
        
        for (int y = 0; y < 15; y++)
        {
          for (int x = 0; x < 10; x++)
          {
            int index = start_position + (x + y * video_width);
           
            value_r  +=        red(video.pixels[index]);
            value_g  +=      green(video.pixels[index]);
            value_b  +=       blue(video.pixels[index]);
          }
        }
        
        //Get the mean values
        value_r /= 150;
        value_g /= 150;
        value_b /= 150;
        
        //scale down        
        scaled_value_r = (byte)(value_r / 43);
        scaled_value_g = (byte)(value_g / 43);
        scaled_value_b = (byte)(value_b / 43);
        
        //re-scale up
        value_r = scaled_value_r * 51;
        value_g = scaled_value_g * 51;
        value_b = scaled_value_b * 51;        
        
        //Draw current pixel in Processing Window  
        fill(value_r, value_g, value_b);
        ellipse(column * pixel_pitch + pixel_pitch / 2, row * pixel_pitch + pixel_pitch / 2, pixel_diameter, pixel_diameter);    
        
        rgb_matrix[row * 32 + column] = (byte)(36*scaled_value_r + 6*scaled_value_g + scaled_value_b); 
 
      }    
    }
  }
}

/*----------------------------------------------------------------------------------------------------------------------------------------*/

