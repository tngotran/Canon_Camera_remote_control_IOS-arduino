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
#include "pitches.h"

#define FOCUS_LENGTH 1//length of RESTART
#define SHOOT_LENGTH 1//length of SHOOT
#define STOP_LENGTH 1//length of STOP
#define RXBUF_SIZE 50
#define le 1


 #define rxPin 7 //from AP
 #define txPin 3 //to AP
 int ledState = LOW;             // ledState used to set the LED
 unsigned long previousMillis = 0;        // will store last time LED was updated
 unsigned long currentMillis = 0;        // will store last time LED was updated
 const long interval = 500;           // interval at which to blink (milliseconds)

int focus = 11;  //PWM control for motor outputs 3 and 4 is on digital pin 11
int shoot = 8;  //direction control for motor outputs 1 and 2 is on digital pin 12
int LED = 13;
int loop_shot = 1;
int delay_time = 0;
bool isSoundOn = true;

int val = 0;     //value for fade
char rxbuf[RXBUF_SIZE]={};
int rxbuf_index=0; // index of rxbuf
//focus = a
//shoot = b
//cancel = c
//char FOCUS[FOCUS_LENGTH] = {'f','o','c','u','s'};
//char SHOOT[SHOOT_LENGTH] = {'s','h','o','o','t'};
//char STOP[STOP_LENGTH] = {'c','a','n','c','e','l'};
char FOCUS[FOCUS_LENGTH] = {'a'};
char SHOOT[SHOOT_LENGTH] = {'b'};
char STOP[STOP_LENGTH] = {'c'};
//3sOn = d shoot after 3 second
//3sOf = e
//5sOn = f
//5sOf = g
//char sw3secon[le] =   {'3','s','O','n'};
//char sw3secoff[le] = {'3','s','O','f'};
//char sw5secon[le] =   {'5','s','O','n'};
//char sw5secoff[le] = {'5','s','O','f'};

char sw3secon[le] =   {'d'};
char sw3secoff[le] = {'e'};
char sw5secon[le] =   {'f'};
char sw5secoff[le] = {'g'};

//3tOn = h shoot 3 times
//3tOf = i
//5tOn = j
//5tOf = k
char sw3shoon[le] =   {'h'};
char sw3shooff[le] = {'i'};
char sw5shoon[le] =   {'j'};
char sw5shooff[le] = {'k'};

char soundOn[le] = {'l'};
char soundOf[le] = {'m'};
char soundDisconnect[le] = {'n'};
char soundConnected[le] = {'o'};
char soundShoot[le] = {'p'};


SoftwareSerial port2Ap (rxPin, txPin);
void setup()

{
  Serial.begin(9600);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
   port2Ap.begin(9600);

  pinMode(LED, OUTPUT);
  pinMode(focus, OUTPUT);
  pinMode(shoot, OUTPUT);

  digitalWrite(focus, 0);  //set both motors to run at (100/255 = 39)% duty cycle (slow)
  digitalWrite(shoot, 0);
  digitalWrite(LED, HIGH); ledState = 1;
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


    do_TEST(sw3secoff, le,sw_3sec_off);
    do_TEST(sw5secoff, le,sw_5sec_off);
    do_TEST(sw3secon, le,sw_3sec_on);
    do_TEST(sw5secon, le,sw_5sec_on);


    do_TEST(sw3shooff, le,sw_3shot_off);
    do_TEST(sw5shooff, le,sw_5shot_off);
    do_TEST(sw3shoon, le,sw_3shot_on);
    do_TEST(sw5shoon, le,sw_5shot_on);

    do_TEST(soundOn, le,soundOn_func);
    do_TEST(soundOf, le,soundOf_func);

  }

}

void soundOn_func(void){
  isSoundOn = true;
}


void soundOf_func(void){
  isSoundOn = false;
}

void sw_3sec_on(void){
  port2Ap.print("3sOn");
  delay_time = 3;
}
void sw_3sec_off(void){
  port2Ap.print("3sOf");
  delay_time = 0;
}

void sw_5sec_on(void){
  port2Ap.print("5sOn");
  delay_time = 5;
}
void sw_5sec_off(void){
  port2Ap.print("5sOf");
  delay_time = 0;
}
void sw_5shot_on(void){
  port2Ap.print("5tOn");
  delay_time = 3;
  loop_shot = 5;
}
void sw_5shot_off(void){
  port2Ap.print("5tOf");
  loop_shot = 1;
}
void sw_3shot_on(void){
  port2Ap.print("3tOn");
  delay_time = 3;
  loop_shot = 3;
}
void sw_3shot_off(void){
  port2Ap.print("3tOf");
  loop_shot = 1;
}

void focus1(void){
  Serial.print("focus1");
  delay(20);
  digitalWrite(focus, HIGH);
  digitalWrite(LED, HIGH); ledState = 1;  
  //playSoundFocus()
}


void playSoundShoot(){
  if(!isSoundOn) return;  
  for (int i = 0; i < 3; i++) {    
      tone(10, NOTE_B4, 20);
      delay(100);
  }
}

void playSoundFocus(){  
  if(!isSoundOn) return;
  for (int i = 0; i < 2; i++) {    
      tone(10, NOTE_A5, 20);
      delay(100);
  }
}

void playSoundCancel(){
  if(!isSoundOn) return;
  for (int i = 0; i < 2; i++) {    
      tone(10, NOTE_C7, 20);
      delay(100);
  }
}



void shoot1(){
  Serial.print("shoot1");

//port2Ap.print(loop_shot);
  for (int k = 0;k<loop_shot;k++){
    count_sec();
    focus1();//turn on focus before shoot a picture
    delay(50);
    digitalWrite(shoot, HIGH);
    delay(30);
    digitalWrite(shoot, LOW);

    playSoundShoot();
  }

  delay(50);
  digitalWrite(focus, LOW); //turn off focus after shoot a picture
  digitalWrite(LED, HIGH); ledState = 1;
  port2Ap.print("Y");
}

  void count_sec(){
    if(delay_time == 3){
      //port2Ap.print("dl3");
      // interval = 3000;
      int count = 0;
      while(count < 6){
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= interval) {
            // save the last time you blinked the LED
            previousMillis = currentMillis;

            // if the LED is off turn it on and vice-versa:
            if (ledState == LOW) {
              ledState = HIGH;
            } else {
              ledState = LOW;
            }


            digitalWrite(LED, ledState);
            count = count + 1;
       }
      }
    //  delay(3000);
    }else if(delay_time == 5){
      //port2Ap.print("dl5");
      //interval = 5000;
      int count = 0;
      while(count < 10){
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= interval) {
            // save the last time you blinked the LED
            previousMillis = currentMillis;

            // if the LED is off turn it on and vice-versa:
            if (ledState == LOW) {
              ledState = HIGH;
            } else {
              ledState = LOW;
            }
            digitalWrite(LED, ledState);
            count = count + 1;
       }
      }
      //delay(5000);
    }

 }

void cancel1(){
  loop_shot = 1;
  delay_time = 0;
  digitalWrite(focus, LOW);
  delay(20);
  digitalWrite(shoot, LOW);
  digitalWrite(LED, HIGH); ledState = 1;
  playSoundCancel();
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
