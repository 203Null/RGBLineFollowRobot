#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 12
#define DATA_PIN 9

int timer;

long cyanshift[18] = {0x00ffff,0x008e8e,0x005252,0x000a0a,0x000303,0x000000,0x00ffff,0x008e8e,0x005252,0x000a0a,0x000303,0x000000,0x00ffff,0x008e8e,0x005252,0x000a0a,0x000303,0x000000};
long greenshift[12] = {0x00ff00,0x008e00,0x005200,0x000a00,0x000300,0x000000,0x00ff00,0x008e00,0x005200,0x000a00,0x000300,0x000000};

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
  	  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
      Serial1.begin(9600);
      Serial.begin(9600);
      fill_solid(leds,12,CRGB::White);FastLED.show();
      delay(500);
      fill_solid(leds,12,0);FastLED.show();
      Serial1.println("HelloWorld");
      Serial1.println("HelloWorld");
}

void alloff(){
 fill_solid(leds,12,0);
}

boolean check(char in){
    char cache = Serial1.peek();
    if(in == cache){
      Serial1.read();
      return false;
    }
  if(cache == 'O' || cache == 'F' || cache == '<' || cache == '>' || cache == 'M' || cache == 'm' || cache == 'P' || cache == 'L'|| cache == 'R'|| cache == 'l'|| cache == 'r'|| cache == 'G'){
    return true;
  }else{
    Serial1.read();
    return false;
  }
}

void turnleft(){
  while(true){
      for(int i = 6; i > 0 ;i--){
      fill_solid(leds,12,0);
      for(int l = 0; l < 12;l++){
        leds[l]=cyanshift[i+l];
      }
      FastLED.show();
            if(check('<')){
            alloff();
            FastLED.show();
            return;
        }
      delay(50);
    }
  }
}

void turnright(){
  while(true){
      for(int i = 0; i < 6;i++){
      fill_solid(leds,12,0);
      for(int l = 0; l < 12;l++){
        leds[l]=cyanshift[i+l+1];
      }
      FastLED.show();
        if(check('>')){
            alloff();
            FastLED.show();
            return;
        }
      delay(50);
    }
  }
}

void Pause(){
  while(true){
    //fill_solid(leds,12,CRGB::Purple);
    for(int h = 0; h <256;h++){
    fill_rainbow(leds+6, 6,h,-20);
    fill_rainbow(leds, 6,h-100,20);     
    FastLED.show();
    if(check('P')){
            alloff();
            FastLED.show();
            return;
    }
    }
  }
}

void forward(){
  while(true){
      for(int i = 6; i > 0 ;i-- ){
      fill_solid(leds,12,0);
      for(int l = 0; l < 6;l++){
        leds[l] = greenshift[l+i];
        leds[11-l] = greenshift[l+i];
      }
       FastLED.show();
        if(check('G')){
            alloff();
            FastLED.show();
            return;
        }
        delay(50);
     }
}
}


void loop() {
    if (Serial1.available() > 0) {
    char mode = Serial1.read();
       switch (mode){
        case 'F': //all off
          fill_solid(leds,12,0);
          Serial1.println("Alloff");
          break;
        case 'O': //all white
          fill_solid(leds,12,CRGB::White);
          Serial1.println("Allon");
          break;
        case 'P':
          Serial1.println("Pause");
          Pause();
          break;
        case 'G':
          Serial1.println("G");
          forward();
          break;
        case 'm':
          leds[4] = 0;
          leds[5] = 0;
          leds[6] = 0;
          leds[7] = 0;
          FastLED.show();
          Serial1.println("M off");
          break;
        case 'M':
          leds[4] = 0x00ff00;
          leds[5] = 0x00ff00;
          leds[6] = 0x00ff00;
          leds[7] = 0x00ff00;
          FastLED.show();
          Serial1.println("M on");
          break;          
        case 'l':
          leds[0] = 0;
          leds[1] = 0;
          leds[2] = 0;
          leds[3] = 0;
          FastLED.show();
          Serial1.println("L off");
          break;
        case 'L':
          leds[0] = 0xffff00;
          leds[1] = 0xffff00;
          leds[2] = 0xffff00;
          leds[3] = 0xffff00;
          FastLED.show();
          Serial1.println("L on");
          break;
        case 'r':
          leds[8] = 0;
          leds[9] = 0;
          leds[10] = 0;
          leds[11] = 0;
          FastLED.show();
          Serial1.println("R off");
          break;
        case 'R':
          leds[8] = 0xffff00;
          leds[9] = 0xffff00;
          leds[10] = 0xffff00;
          leds[11] = 0xffff00;
          FastLED.show();
          Serial1.println("R on");
          break;
        case '<':
          Serial1.println("L");
          turnleft();
          break;
        case '>':
          Serial1.println("R");
          turnright();
          break;
        FastLED.show();
  }
}
}


