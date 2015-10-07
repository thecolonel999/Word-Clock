#include <FastLED.h>
#include <Wire.h>
#include <SPI.h>
#include <RTClib.h>
#include <RTC_DS3231.h>
#include "WordClock12x12Array.h"

RTC_DS3231 RTC;

const uint8_t kMatrixWidth  = 12;
const uint8_t kMatrixHeight = 12;
const bool    kMatrixSerpentineLayout = true;

// Set 'kMatrixSerpentineLayout' to false if your pixels are 
// laid out all running the same way, like this:
//
//     0 >  1 >  2 >  3 >  4
//                         |
//     .----<----<----<----'
//     |
//     5 >  6 >  7 >  8 >  9
//                         |
//     .----<----<----<----'
//     |
//    10 > 11 > 12 > 13 > 14
//                         |
//     .----<----<----<----'
//     |
//    15 > 16 > 17 > 18 > 19
//
// Set 'kMatrixSerpentineLayout' to true if your pixels are 
// laid out back-and-forth, like this:
//
//     0 >  1 >  2 >  3 >  4
//                         |
//                         |
//     9 <  8 <  7 <  6 <  5
//     |
//     |
//    10 > 11 > 12 > 13 > 14
//                        |
//                        |
//    19 < 18 < 17 < 16 < 15

#define LED_PIN  4
#define COLOR_ORDER GRB
#define CHIPSET     WS2811
#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds[NUM_LEDS];

uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t j;
  
  if( kMatrixSerpentineLayout == false) {
    j = (y * kMatrixWidth) + x;
  }

  if( kMatrixSerpentineLayout == true) {
    if( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      j = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      j = (y * kMatrixWidth) + x;
    }
  }
  
  return j;
}

uint16_t matrix[12];  //Holds matrix matrix
uint16_t last[12];  //Holds previous matrix

unsigned int curHours;
unsigned int hours = 99;
unsigned int curMins;
unsigned int mins = 99;
int aniChoice = 0;

void setup() {  
  //Serial.begin(57600);

  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);

  //--------RTC SETUP ------------
  RTC.begin();
  delay(100);

  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);

  if (! RTC.isrunning()) {
      Serial.println("RTC is NOT running!");
      // following line sets the RTC to the date & time this sketch was compiled
      RTC.adjust(DateTime(__DATE__, __TIME__));
    }
  
    DateTime now = RTC.now();
    DateTime compiled = DateTime(__DATE__, __TIME__);
    if (now.unixtime() < compiled.unixtime()) {
      //Serial.println("RTC is older than compile time!  Updating");
      RTC.adjust(DateTime(__DATE__, __TIME__));
    }

}

unsigned long currentMillis = millis();
unsigned long previousMillis = 0; 
unsigned long resetMillis = 0; 
unsigned long pressCheckA0 = 0;
unsigned long pressCheckA1 = 0;
unsigned long pressCheckA2 = 0;


bool A0pressed = false;
bool A1pressed = false;
bool A2pressed = false;
bool A0read = false;
bool A1read = false;
bool A2read = false;

void loop() {

  if((unsigned long)(currentMillis - previousMillis) >= 5000) {
   
    Serial.print(hours);
    Serial.print(':');
    Serial.println(mins);
    checkTime();
    previousMillis = currentMillis;
  }
  
  if( digitalRead(5) == HIGH && A0pressed == false) {
     A0pressed = true;
     pressCheckA0 = currentMillis;
     A0read = false;
  }  

  if( digitalRead(6) == HIGH && A1pressed == false) {
      A1pressed = true;
      pressCheckA1 = currentMillis;
      A1read = false;
  }  


  if( digitalRead(7) == HIGH && A2pressed == false) {
      A2pressed = true;
      pressCheckA2 = currentMillis;
      A2read = false;
  }

  if( A0pressed == true && A0read == false && (unsigned long)(currentMillis - pressCheckA0) >= 50) {
      DateTime now = RTC.now();
      int newHour = now.hour() + 1;
      if (newHour > 23) {
         newHour = 0; 
      }
      RTC.adjust(DateTime(now.year(),now.month(),now.day(),newHour,now.minute(),now.second()));
      hours = newHour;
      mins = now.minute(); 
      curHours = newHour;
      curMins = now.minute(); 
      wipe();
      assemble();
      drawDisplay();
      A0read = true;
  }
  
  if( A1pressed == true && A1read == false && (unsigned long)(currentMillis - pressCheckA1) >= 50) {
      DateTime now = RTC.now();   
      int newMinute = now.minute() + 1;
      if (newMinute > 59) {
         newMinute = 0; 
      }
      RTC.adjust(DateTime(now.year(),now.month(),now.day(),now.hour(),newMinute,now.second()));
      hours = now.hour();
      mins = newMinute; 
      curHours = now.hour();
      curMins = newMinute;
      wipe();
      assemble();
      drawDisplay();
      A1read = true;
  }

  if( digitalRead(5) == LOW) {
     A0pressed = false; 
     A0read = false;
  }
   
  if( digitalRead(6) == LOW) {
     A1pressed = false; 
     A1read = false;
  } 
 
  if( digitalRead(7) == LOW) {
     A2pressed = false; 
     A2read = false;
  } 

  wipe();
  assemble();
  delay(10); 
  currentMillis = millis();
}

void checkTime () {
  DateTime now = RTC.now();
  curHours = now.hour();
  curMins = now.minute();  

  if (curHours != hours || curMins != mins) {
    hours = curHours;
    mins = curMins;
   //software_Reset();
    doAnimation();

  }
}

void assemble() {

  // "IT IS" handling
  
  for( int i = 0; i < 12; i++ ) {
    matrix[i] = matrix[i] | pgm_read_word(&(it_is[i]));
  }

  // Hour handling

  if ( hours == 0 || hours == 12) {
    for( int i = 0; i < 12; i++ ) {
      if (mins > 30) {
        matrix[i] = matrix[i] | pgm_read_word(&(h1[i]));
      } else {
        matrix[i] = matrix[i] | pgm_read_word(&(h12[i]));
      }
    }
  }  else


  if ( hours == 1 || hours == 13) {
    for( int i = 0; i < 12; i++ ) {
      if (mins > 30) {
        matrix[i] = matrix[i] | pgm_read_word(&(h2[i]));
      } else {
        matrix[i] = matrix[i] | pgm_read_word(&(h1[i]));
      }
    }
   } else
    
  if ( hours == 2 || hours == 14) {
    for( int i = 0; i < 12; i++ ) {
     if (mins > 30) {
        matrix[i] = matrix[i] | pgm_read_word(&(h3[i]));
      } else {
        matrix[i] = matrix[i] | pgm_read_word(&(h2[i]));
      }
    }
   } else
    
  if ( hours == 3 || hours == 15) {
    for( int i = 0; i < 12; i++ ) {
      if (mins > 30) {
        matrix[i] = matrix[i] | pgm_read_word(&(h4[i]));
      } else {
        matrix[i] = matrix[i] | pgm_read_word(&(h3[i]));
      }
    }
  } else
    
  if ( hours == 4 || hours == 16) {
    for( int i = 0; i < 12; i++ ) {
      if (mins > 30) {
        matrix[i] = matrix[i] | pgm_read_word(&(h5[i]));
      } else {
        matrix[i] = matrix[i] | pgm_read_word(&(h4[i]));
      }
    }
  } else
    

  if ( hours == 5 || hours == 17) {
    for( int i = 0; i < 12; i++ ) {
      if (mins > 30) {
        matrix[i] = matrix[i] | pgm_read_word(&(h6[i]));
      } else {
        matrix[i] = matrix[i] | pgm_read_word(&(h5[i]));
      }
    }
  } else
    
  if ( hours == 6 || hours == 18) {
    for( int i = 0; i < 12; i++ ) {
      if (mins > 30) {
        matrix[i] = matrix[i] | pgm_read_word(&(h7[i]));
      } else {
        matrix[i] = matrix[i] | pgm_read_word(&(h6[i]));
      }
    }
  } else
    
  if ( hours == 7 || hours == 19) {
    for( int i = 0; i < 12; i++ ) {
      if (mins > 30) {
        matrix[i] = matrix[i] | pgm_read_word(&(h8[i]));
      } else {
        matrix[i] = matrix[i] | pgm_read_word(&(h7[i]));
      }
    }
  } else
    
  if ( hours == 8 || hours == 20) {
    for( int i = 0; i < 12; i++ ) {
      if (mins > 30) {
        matrix[i] = matrix[i] | pgm_read_word(&(h9[i]));
      } else {
        matrix[i] = matrix[i] | pgm_read_word(&(h8[i]));
      }
    }
  } else
    
  if ( hours == 9 || hours == 21) {
    for( int i = 0; i < 12; i++ ) {
      if (mins > 30) {
        matrix[i] = matrix[i] | pgm_read_word(&(h10[i]));
      } else {
        matrix[i] = matrix[i] | pgm_read_word(&(h9[i]));
      }
    }
  } else
  
  if ( hours == 10 || hours == 22) {
    for( int i = 0; i < 12; i++ ) {
      if (mins > 30) {
        matrix[i] = matrix[i] | pgm_read_word(&(h11[i]));
      } else {
        matrix[i] = matrix[i] | pgm_read_word(&(h10[i]));
      }
    }
  } else
    
  if ( hours == 11 || hours == 23) {
    for( int i = 0; i < 12; i++ ) {
      if (mins > 30) {
        matrix[i] = matrix[i] | pgm_read_word(&(h12[i]));
      } else {
        matrix[i] = matrix[i] | pgm_read_word(&(h11[i]));
      }
    }
  }
  
  // Minute handling

  if (mins == 1 || mins == 59  || mins == 21 || mins == 39) {
    for( int i = 0; i < 12; i++ ) {
      matrix[i] = matrix[i] | pgm_read_word(&(m1[i]));
    }
  } else

  if (mins == 2 || mins == 58 || mins == 22 || mins == 38) {
    for( int i = 0; i < 12; i++ ) {
      matrix[i] = matrix[i] | pgm_read_word(&(m2[i]));
    }
  } else

  if (mins == 3 || mins == 57 || mins == 23 || mins == 37) {
    for( int i = 0; i < 12; i++ ) {
      matrix[i] = matrix[i] | pgm_read_word(&(m3[i]));
    }
  } else

  if (mins == 4 || mins == 56 || mins == 24 || mins == 36) {
    for( int i = 0; i < 12; i++ ) {
      matrix[i] = matrix[i] | pgm_read_word(&(m4[i]));
    }
  } else

  if (mins == 5 || mins == 55 || mins == 25 || mins == 35) {
    for( int i = 0; i < 12; i++ ) {
      matrix[i] = matrix[i] | pgm_read_word(&(m5[i]));
    }
  } else

  if (mins == 6 || mins == 54 || mins == 26 || mins == 34) {
    for( int i = 0; i < 12; i++ ) {
      matrix[i] = matrix[i] | pgm_read_word(&(m6[i]));
    }
  } else

  if (mins == 7 || mins == 53 || mins == 27 || mins == 33) {
    for( int i = 0; i < 12; i++ ) {
      matrix[i] = matrix[i] | pgm_read_word(&(m7[i]));
    }
  } else

  if (mins == 8 || mins == 52 || mins == 28 || mins == 32) {
    for( int i = 0; i < 12; i++ ) {
      matrix[i] = matrix[i] | pgm_read_word(&(m8[i]));
    }
  } else

  if (mins == 9 || mins == 51 || mins == 29 || mins == 31) {
    for( int i = 0; i < 12; i++ ) {
      matrix[i] = matrix[i] | pgm_read_word(&(m9[i]));
    }
  } else

  if (mins == 10 || mins == 50) {
    for( int i = 0; i < 12; i++ ) {
      matrix[i] = matrix[i] | pgm_read_word(&(m10[i]));
    }
  } else

  if (mins == 11 || mins == 49) {
    for( int i = 0; i < 12; i++ ) {
      matrix[i] = matrix[i] | pgm_read_word(&(m11[i]));
    }
  } else

  if (mins == 12 || mins == 48) {
    for( int i = 0; i < 12; i++ ) {
      matrix[i] = matrix[i] | pgm_read_word(&(m12[i]));
    }
  } else

  if (mins == 13 || mins == 47) {
    for( int i = 0; i < 12; i++ ) {
      matrix[i] = matrix[i] | pgm_read_word(&(m13[i]));
    }
  } else

  if (mins == 14 || mins == 46) {
    for( int i = 0; i < 12; i++ ) {
      matrix[i] = matrix[i] | pgm_read_word(&(m14[i]));
    }
  } else

  if (mins == 15 || mins == 45) {
    for( int i = 0; i < 12; i++ ) {
      matrix[i] = matrix[i] | pgm_read_word(&(m15[i]));
    }
  } else

  if (mins == 16 || mins == 46) {
    for( int i = 0; i < 12; i++ ) {
      matrix[i] = matrix[i] | pgm_read_word(&(m16[i]));
    }
  } else

  if (mins == 17 || mins == 43) {
    for( int i = 0; i < 12; i++ ) {
      matrix[i] = matrix[i] | pgm_read_word(&(m17[i]));
    }
  } else

  if (mins == 18 || mins == 42) {
    for( int i = 0; i < 12; i++ ) {
      matrix[i] = matrix[i] | pgm_read_word(&(m18[i]));
    }
  } else

  if (mins == 19 || mins == 41) {
    for( int i = 0; i < 12; i++ ) {
      matrix[i] = matrix[i] | pgm_read_word(&(m19[i]));
    }
  } else

  if (mins >= 20 && mins < 30) {
    for( int i = 0; i < 12; i++ ) {
      matrix[i] = matrix[i] | pgm_read_word(&(m20[i]));
    }
  }

  if (mins == 30) {
    for( int i = 0; i < 12; i++ ) {
      matrix[i] = matrix[i] | pgm_read_word(&(m30[i]));
    }
  }

  if (mins == 0) {
    for( int i = 0; i < 12; i++ ) {
      matrix[i] = matrix[i] | pgm_read_word(&(o_clock[i]));
    }
  }

  if (mins > 0 && mins <= 30) {
    for( int i = 0; i < 12; i++ ) {
      matrix[i] = matrix[i] | pgm_read_word(&(past[i]));
    }
  }

  if (mins > 30 && mins < 60) {
    for( int i = 0; i < 12; i++ ) {
      matrix[i] = matrix[i] | pgm_read_word(&(to[i]));
    }
  }
}

void wipe () {
    for( int i = 0; i < 12; i++ ) {
      matrix[i] = 0b000000000000;
    }  
}


//Animation functions

//No animation
void drawDisplay() {
  wipe();
    assemble();
    
     for( int y = 12; y >= 0; y-- ) {
       for( int x = 0; x < 12; x++) {  

         if(bitRead( matrix[12 - y] , 12 - x)) {
            leds[XY(x,y)].setHSV(224, 187, 255);               
            }
       }
     }
}

//Fade out and back in
void fadeOut() {
     for(int b = 255; b >= 0; b--) {
       FastLED.setBrightness(b);
       delay(5);
    }
    wipe();
    assemble();
    drawDisplay();

    delay(100);

    for(int b = 0; b < 255 ; b++) {
       FastLED.setBrightness(b);
       delay(5);
    } 
}

//Random dissolve to all on, then fade back to time
void whiteOut () {
  
      int randomizer[256] = {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
        17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
        33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
        49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64,
        65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
        81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96,
        97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112,
        113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128,
        129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144,
        145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 
        161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 
        177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 
        193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 
        209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 
        225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 
        241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256
      };  
  

     bubbleUnsort (randomizer, 256);    
  
     for( int i = 0; i < 256; i++) {
        
          int y = randomizer[i]/16;
          int x = randomizer[i]%16;

          leds[XY(x,y)].setHSV(224, 187, 255);
     }
     
     wipe();
     bubbleUnsort (randomizer, 256);    
     assemble();

     for( int i = 0; i < 256; i++) {
      
          int y = randomizer[i]/16;
          int x = randomizer[i]%16;
   
         if(bitRead( matrix[15 - y] , 15 - x)) {
          leds[XY(x,y)].setHSV(224, 187, 0);
          } else {
          leds[XY(x,y)].setHSV(224, 187, 0);        
          }
     }
}

//Horizontal sweep.
void sweep() {      
      
    for( int x = 0; x < 12; x++) {
      last[x] = matrix[x]; 
    }
    

     wipe();
     assemble();   
   
     for( int x = 0; x < 12; x++) {  
       
       for( int y = 12; y >= 0; y-- ) {   

         if(!bitRead( matrix[12 - y] , 12 - x)) {
              leds[XY(x,y)].setHSV(224, 187, 0);
            }
       }          
     }
     
     for( int x = 0; x < 12; x++) {  
       
       for( int y = 12; y >= 0; y-- ) {  
         
         if(bitRead( matrix[12 - y] , 12 - x)) {
            leds[XY(x,y)].setHSV(224, 187, 255);
          }
        } 
     }
}

//Matrixy.
void matrixy() {      
    wipe();
    assemble();
 
     int cols[12] = {0,1,2,3,4,5,6,7,8,9,10,11};
     bubbleUnsort( cols, 12);
 
     
     for(int r = 0; r < 2; r++) {
     
     for( int i = 0; i < 12; i++) {
     
     int x = cols[i];
     int x2 = cols[i+1];
     int x3 = cols[i+2];
     int x4 = cols[i+3];
 
     for( int y = 12; y >= 0; y-- ) {

      leds[XY(x,y)].setHSV(224, 187, 255);
      leds[XY(x2,y)].setHSV(224, 187, 255);
      leds[XY(x3,y)].setHSV(224, 187, 255);
      leds[XY(x4,y)].setHSV(224, 187, 255);
      
      delay(10);
         
         if(!bitRead( matrix[12 - y] , 12 - x)) {
          leds[XY(y,x)].setHSV(224, 187, 0);
          }
         
         if(!bitRead( matrix[12 - y] , 12 - x2)) {
          leds[XY(y,x2)].setHSV(224, 187, 0);
          }         
          
         if(!bitRead( matrix[12 - y] , 12 - x3)) {
          leds[XY(y,x3)].setHSV(224, 187, 0);
          }         
         if(!bitRead( matrix[12 - y] , 12 - x4)) {
          leds[XY(y,x4)].setHSV(224, 187, 0);
         }
     }
     }
     }
}

void doAnimation() {
   bool isRandom = false;
  
  if( aniChoice > 4) {
     aniChoice = 0; 
  }
  
  if(aniChoice == 5) {
    isRandom = true;
    int randAni[4] = {1,2,3,4};
    bubbleUnsort(randAni,4);
    aniChoice = randAni[0];  
  }
  
  if (aniChoice == 0 ) {
    drawDisplay();
  }
  
  if (aniChoice == 1 ) {
    fadeOut();
  }
  
  if (aniChoice == 2 ) {
    whiteOut();
  }
  
  if (aniChoice == 3 ) {
    sweep();
  } 
  
  if (aniChoice == 4 ) {
    matrixy();
  }   
 
  if( isRandom == true) {
    aniChoice = 5;
  }    
}

//Math!
void bubbleUnsort(int *list, int elem)
{
 for (int a=elem-1; a>0; a--)
 {
   //int r = random(a+1);
   int r = rand_range(a+1);
   if (r != a)
   {
     int temp = list[a];
     list[a] = list[r];
     list[r] = temp;
   }
 }
}

// generate a value between 0 <= x < n, thus there are n possible outputs
int rand_range(int n)
{
   int r, ul;
   ul = RAND_MAX - RAND_MAX % n;
   while ((r = random(RAND_MAX+1)) >= ul);
   return r % n;
}
