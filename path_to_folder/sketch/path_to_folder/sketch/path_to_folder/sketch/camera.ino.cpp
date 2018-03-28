#include <Arduino.h>
#line 1 "D:\\Dropbox\\camera\\camera.ino"
#line 1 "D:\\Dropbox\\camera\\camera.ino"
/* 5/22/2012
  Timothy Holmberg
  SparkFun Electronics

  This code includes the ddition of fade in and out PWM. Also a stop feature. And the addition of individual functions for motor control

  This was a revision of the example sketch that originated from Pete Dokter's code for Arduino that shows very basically how to control an Ardumoto
  motor driver shield with a 5V Arduino controller board. http://www.sparkfun.com/datasheets/DevTools/Arduino/Ardumoto_test_3.pde

  This also includes parts of the Fading Example,  Created 1 Nov 2008 By David A. Mellis, modified 30 Aug 2011 By Tom Igoe http://arduino.cc/en/Tutorial/Fading

*/
#include <SoftwareSerial.h>
#include <HardwareSerial.h>

#define FOCUS_LENGTH 5//length of RESTART
#define SHOOT_LENGTH 5//length of RESTART
#define STOP_LENGTH 6//length of RESTART
#define RXBUF_SIZE 50
#define swsecon_LENGTH 10
#define swsecoff_LENGTH 11
#define swshocon_LENGTH 10
#define swshocoff_LENGTH 11

 #define rxPin 10 //from AP
 #define txPin 9 //to AP
 int ledState = LOW;             // ledState used to set the LED
 unsigned long previousMillis = 0;        // will store last time LED was updated
 const long interval = 1000;           // interval at which to blink (milliseconds)

int focus = 11;  //PWM control for motor outputs 3 and 4 is on digital pin 11
int shoot = 8;  //direction control for motor outputs 1 and 2 is on digital pin 12
int LED = 13;

bool  flag_3sec = 0;
bool  flag_5sec = 0;
bool  flag_3shot = 0;
bool  flag_5shot = 0;

int val = 0;     //value for fade
char rxbuf[RXBUF_SIZE]={};
int rxbuf_index=0; // index of rxbuf
char FOCUS[FOCUS_LENGTH] = {'f','o','c','u','s'};
char SHOOT[SHOOT_LENGTH] = {'s','h','o','o','t'};
char STOP[STOP_LENGTH] = {'c','a','n','c','e','l'};

char sw3secon[swsecon_LENGTH] =   {'s','w','_','3','s','e','c','_','o','n'};
char sw3secoff[swsecoff_LENGTH] = {'s','w','_','3','s','e','c','_','o','f','f'};
char sw5secon[swsecon_LENGTH] =   {'s','w','_','5','s','e','c','_','o','n'};
char sw5secoff[swsecoff_LENGTH] = {'s','w','_','5','s','e','c','_','o','f','f'};

char sw3shoon[swshocon_LENGTH] =   {'s','w','_','3','s','h','o','_','o','n'};
char sw3shooff[swshocoff_LENGTH] = {'s','w','_','3','s','h','o','_','o','f','f'};
char sw5shoon[swshocon_LENGTH] =   {'s','w','_','5','s','h','o','_','o','n'};
char sw5shooff[swshocoff_LENGTH] = {'s','w','_','5','s','h','o','_','o','f','f'};

SoftwareSerial port2Ap (rxPin, txPin);
void setup()

{   Serial.begin(9600);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
   port2Ap.begin(9600);

  pinMode(LED, OUTPUT);
  pinMode(focus, OUTPUT);
  pinMode(shoot, OUTPUT);

  digitalWrite(focus, 0);  //set both motors to run at (100/255 = 39)% duty cycle (slow)
  digitalWrite(shoot, 0);
  digitalWrite(LED, HIGH);
}

void loop()
{
//    digitalWrite(focus, HIGH);   // turn the LED on (HIGH is the voltage level)
//      digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
//  delay(1000);              // wait for a second
////  digitalWrite(focus, LOW);    // turn the LED off by making the voltage LOW
//   digitalWrite(LED, LOW);   // turn the LED on (HIGH is the voltage level)
//  delay(1000);              // wait for a second
//    unsigned long currentMillis = millis();
  if(saveData()){
    do_TEST(FOCUS, FOCUS_LENGTH,focus1);
    do_TEST(SHOOT, SHOOT_LENGTH,shoot1);
    do_TEST(STOP, STOP_LENGTH,cancel1);

    do_TEST(sw3secon, swsecon_LENGTH,sw_3sec_on);
    do_TEST(sw3secoff, swsecoff_LENGTH,sw_3sec_off);
    do_TEST(sw5secon, swsecon_LENGTH,sw_5sec_on);
    do_TEST(sw5secoff, swsecoff_LENGTH,sw_5sec_off);

    do_TEST(sw3shoon, swshocon_LENGTH,sw_3shot_on);
    do_TEST(sw3shooff, swshocoff_LENGTH,sw_3shot_off);
    do_TEST(sw5shoon, swshocon_LENGTH,sw_5shot_on);
    do_TEST(sw5shooff, swshocoff_LENGTH,sw_5shot_off);
  }
  //  if (currentMillis - previousMillis >= interval) {
 //   // save the last time you blinked the LED
   // previousMillis = currentMillis;

  //  // if the LED is off turn it on and vice-versa:
  //  if (ledState == LOW) {
  //    ledState = HIGH;
  //  } else {
   //   ledState = LOW;
  //  }

    // set the LED with the ledState of the variable:
   // digitalWrite(LED, ledState);
  //}
}
void sw_3sec_on(void){
  flag_3sec = 1;
}
void sw_3sec_off(void){
  flag_3sec = 0;
}

void sw_5sec_on(void){
  flag_5sec = 1;
}
void sw_5sec_off(void){
  flag_5sec = 0;
}
void sw_5shot_on(void){
  flag_5shot = 1;
}
void sw_5shot_off(void){
  flag_5shot = 0;
}
void sw_3shot_on(void){
  flag_3shot = 1;
}
void sw_3shot_off(void){
  flag_3shot = 0;
}

void focus1(void){
  delay(20);
  digitalWrite(focus, HIGH);
}
void shoot1(){
  int loop_shot = 1;
  if(flag_5shot)
    loop_shot = 5;
  else if(flag_3shot)
    loop_shot = 3;
  else
    loop_shot = 1;

  for (int k = 0;k<loop_shot;k++){
    delay(20);
    digitalWrite(shoot, HIGH);
    count_sec();
  }
  delay(30);
  digitalWrite(shoot, LOW);
  delay(50);
  digitalWrite(focus, LOW);

}

  void count_sec(){
    if(flag_5sec)
      delay(5000);
    else if(flag_3sec)
      delay(3000);
 }

void cancel1(){
  digitalWrite(focus, LOW);
  delay(20);
  digitalWrite(shoot, LOW);
}

void seting_loop_shot(){

}

bool saveData(void){

  bool flag = false;
  while(port2Ap.available()){

    if(rxbuf_index>=RXBUF_SIZE){
      rxbuf_index = 0;
      flushrx();
      return false;
    }
    rxbuf[rxbuf_index++]=port2Ap.read();
    flag = true;
    delay(2);//debounce for hardware transfer data. Wait sometime then check available would be more safety
  }
  return flag;
}
/*
function: do_TEST
description:
do_TEST means checking the received data on rxbuf_index array. Does it match with TestString?
if match, let's do the corresponding action
*/
void do_TEST(char * TestString, int leng, void (*action)(void)){
    //  Serial.print("do_TEST1");
      bool test = false;

      for(int k = 0; k<rxbuf_index;k++){
        if(rxbuf[k]==TestString[0] && (k + leng) <= rxbuf_index){
          test = true;
          for(int ki=1; ki<leng;ki++){
            k++;
            if(rxbuf[k]!=TestString[ki]){
              test = false;
              break;
            }
          }
        }
      }
      if(test) {
          Serial.print("do_TEST2");
        action();
        flushrx();rxbuf_index = 0;
      }
}
/*
function: flushrx
description:
this doesn't flux the RX buffer explicitly created by the user but the RX buffer of the UART peripheral.
*/
void flushrx(void){
  while(port2Ap.available()){
        port2Ap.read();
      }
}

