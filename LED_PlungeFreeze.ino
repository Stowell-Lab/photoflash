/*This is the code for simple operation of LED assembled in Vitrobot. Select "Timed" mode.
 * For customized operation add code in the Manual (M) case.
 * LED stays ON for 6 seconds.
 * Turn LED ON towards the end of blotting process to minimize scattered light.
 * Install Adafruit_GFX, Adafruit_ILI9341 and XPT2046_Touchscreen libraries
 */
#include <Wire.h>
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <XPT2046_Touchscreen.h>

#define CS_PIN  9
#define TFT_DC  8
#define TFT_CS 7
// MOSI=11, MISO=12, SCK=13
//XPT2046_Touchscreen ts(CS_PIN);
#define TIRQ_PIN  2
//XPT2046_Touchscreen ts(CS_PIN);  // Param 2 - NULL - No interrupts
//XPT2046_Touchscreen ts(CS_PIN, 255);  // Param 2 - 255 - No interrupts
XPT2046_Touchscreen ts(CS_PIN, TIRQ_PIN);  // Param 2 - Touch IRQ Pin - interrupt enabled polling

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

unsigned long t0;
int ctr=1;int Pwm_v;float blotTime=0; int expTime=5000;const int slaveSelectPin = 4;float tmpTime;
char power, key[3]={'0','z','z'};//char temp1='z';char temp2='z';
int xpos=0, ypos=0; char choice;

void setup(){
   
  delay(100); // Pause for 2 seconds
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  ts.begin();
  ts.setRotation(3);
  while (!Serial && (millis() <= 1000));
  pinMode(slaveSelectPin, OUTPUT);//to DAC(MCP4822)for SPI 
  SPI.begin();
  digitalWrite(slaveSelectPin, LOW);
  delay(1);
  SPI.transfer(0b00010000);//Ch-A,X,Gain 1,SHDN',and 4 MSB of DAC value
  SPI.transfer(0b00000000);//8 LSB. Dac value 0 - TurnOFF led
  delay(1);
  digitalWrite(slaveSelectPin, HIGH);delay(1);
  digitalWrite(slaveSelectPin, LOW);
  delay(1);SPI.transfer(0b10010000);SPI.transfer(0b00000000);delay(1);
  digitalWrite(slaveSelectPin, HIGH);SPI.end();
}
void(* resetFunc) (void) = 0; boolean istouched= false;

void loop(){
  tft.setTextSize(3);tft.setTextColor(ILI9341_WHITE);tft.setCursor(20, 20);tft.print("CHOOSE LED POWER");
  tft.drawRect(10,130,59,50,ILI9341_YELLOW);tft.setTextSize(2);tft.setTextColor(ILI9341_WHITE);tft.setCursor(15,135);tft.print("100%");
  tft.drawRect(80,130,59,50,ILI9341_YELLOW);tft.setTextSize(2);tft.setTextColor(ILI9341_WHITE);tft.setCursor(85,135);tft.print("50%");
  tft.drawRect(150,130,59,50,ILI9341_YELLOW);tft.setTextSize(2);tft.setTextColor(ILI9341_WHITE);tft.setCursor(155,135);tft.print("25%");
    while(!istouched){ 
    istouched = ts.touched();
    if(istouched){
      TS_Point p = ts.getPoint();
      xpos=map(p.x,200,3700,0,320);   ypos=map(p.y,325,3670,0,240);
      if(ypos>130 && ypos<185){
        if(xpos>10 && xpos<70) {power = 'F'; tft.fillRect(10,130,59,50,ILI9341_GREEN);}
        else if(xpos>80 && xpos<140){power = 'H'; tft.fillRect(80,130,59,50,ILI9341_GREEN);}
        else if(xpos>150 && xpos<210){power = 'Q'; tft.fillRect(150,130,59,50,ILI9341_GREEN);}
        else istouched = false;
      }
    }
    }
    istouched = false; delay(1000);
  tft.fillRect(10,10,195,100,ILI9341_WHITE);tft.setTextSize(3);tft.setTextColor(ILI9341_GREEN);tft.setCursor(35, 50);tft.print("LED OFF");
  tft.fillRect(10,110,195,100,ILI9341_YELLOW);tft.setTextSize(2);tft.setTextColor(ILI9341_BLUE);tft.setCursor(35, 115);tft.print("SELECT MODE->");
  tft.fillRect(215,10,100,100,ILI9341_CYAN);tft.setTextSize(2);tft.setTextColor(ILI9341_BLACK);tft.setCursor(235, 50);tft.print("TIMED");
  tft.fillRect(215,110,100,100,ILI9341_RED);tft.setTextSize(2);tft.setTextColor(ILI9341_GREEN);tft.setCursor(225, 135);tft.print("MANUAL");
  
  while(!istouched){ 
    istouched = ts.touched();
    if(istouched){
      TS_Point p = ts.getPoint();
      xpos=map(p.x,200,3700,0,320);   ypos=map(p.y,325,3670,0,240);
      if((xpos>205 && xpos<305)&&(ypos>10 && ypos<110)) {choice = 'T';tft.fillRect(215,10,100,100,ILI9341_BLUE); delay(1000);}
      else if ((xpos>205 && xpos<305)&&(ypos>110 && ypos<210)) {choice = 'M';tft.fillRect(215,110,100,100,ILI9341_BLUE);delay(1000);}
      else istouched = false;
      } 
    } 
  
   istouched = false; 
 switch (choice){
    case 'T':   
               istouched = false; 
               tft.fillScreen(ILI9341_BLACK); delay(100); 
               tft.fillRect(35,10,250,80,ILI9341_YELLOW);tft.setTextSize(2);tft.setTextColor(ILI9341_BLACK);tft.setCursor(100, 45);tft.print("TURN LED ON");//LED turns ON immediately
               tft.fillRect(35,100,250,80,ILI9341_YELLOW);tft.setTextSize(2);tft.setTextColor(ILI9341_BLACK);tft.setCursor(100, 135);tft.print("CANCEL");
               while(!istouched){
                istouched = ts.touched(); 
                if(istouched){
                  TS_Point p = ts.getPoint();
                  xpos=map(p.x,200,3700,0,320);   ypos=map(p.y,325,3670,0,240);
                  if(xpos > 35 && xpos < 285){
                    if(ypos > 10 && ypos < 90) {tft.fillRect(35,10,250,80,ILI9341_BLUE);}
                    else if (ypos > 100 && ypos < 180) {tft.fillRect(35,100,250,80,ILI9341_BLUE); delay(100);resetFunc();}
                    }
                  else istouched = false;
                  }
                }
               istouched = false;xpos=0; ypos=0;
               /*tft.fillRect(35,100,250,80,ILI9341_WHITE);tft.setTextSize(2);tft.setTextColor(ILI9341_BLACK);tft.setCursor(100, 135);tft.print("Delay....");*/ 
               tft.setTextSize(4);tft.setTextColor(ILI9341_RED);tft.setCursor(100, 45);tft.print("LED ON"); 
               dac(6000, power);//LED stays ON for 6 seconds
               break;
    case 'M': 
              /* For operation of the LED exposure system in a customized way user needs to code here*/
              break;
  }
    
  //delay(expTime); 
  tft.fillScreen(ILI9341_BLACK);
}

void dac (int durExposure, char ledPow) //Digital-2-analog IC generates appropriate voltage for the LED driver
{
  SPI.begin();
  digitalWrite(slaveSelectPin, LOW);
  delayMicroseconds(10);
  if (ledPow == 'H'){//HalfPower 500mA to LED
    SPI.transfer(0b00010100);//Ch-A,X,Gain 2,SHDN', 4 MSB of DAC value
    SPI.transfer(0b11100010);//8 LSB. DAC value 0.625V=0.5A to LED
  }
  else if(ledPow == 'Q'){
    SPI.transfer(0b00010010);SPI.transfer(0b01001110);}//0.59 V to recom from DACa:0.25A to LED
  else if(ledPow == 'L'){SPI.transfer(0b00010000);SPI.transfer(0b00111100);}//0.03 V DACa
  else{//FullPower 1000mA to LED
    SPI.transfer(0b00011001);//Ch-A,X,Gain 2,SHDN', 4 MSB of DAC value
    SPI.transfer(0b11000100);//8 LSB. DAC value 2.5V=1A to LED RECOM Driver
  }
  delayMicroseconds(10);
  digitalWrite(slaveSelectPin, HIGH);delay(1);
  delay(durExposure);
  digitalWrite(slaveSelectPin, LOW);//Dac value 0V - TurnOFF led
  delayMicroseconds(10);
  SPI.transfer(0b00010000);//Ch-A,X,Gain 2,SHDN', 4 MSB of DAC value
  SPI.transfer(0b00000000);//8 LSB
  digitalWrite(slaveSelectPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(slaveSelectPin, HIGH);delayMicroseconds(10);
  SPI.end();
}
