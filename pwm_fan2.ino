/*
   Target : Nano

   Description : 
   Test computer Fan "be-quiet pure wings 2"
   This Fan is working with a PWM at 25kHz (40 us) and variable duty cycle

   Switch is not necessary. Can update speed via serial Terminal.
   Within terminal, just send a byte 0->255. 
   0 = off
   255 = max speed
   
   Connections:
   Fan 4-pins description
   (1) black  GND <--> switch Drain (see below)
   (2) yellow +12V <--> +5V to +12V
   (3) green  (Sense) unconnected (just for oscilloscope)
   (4) blue   Control/PWM1 <--> D9

   Misc <--> Arduino
   (SW1   <-->  D2)
   EN1   <-->  D8
   PWM1  <-->  D9
   
   Misc 
   On/Off switch:
   NMOS IRF740  (S) --> GND
                (B) --> EN1
                (D) --> Fan GND (1) 

   Control drive (NMOSFET):
   This is inverting the control signal from the Arduino!
   2N7000  (S) --> GND
           (G) --> PWM1
           (D) --> Fan Ctrl + 10k pullup

   Protection diode:
   1N4001  (Fan GND) ->|- (Fan +12V)

   
*/

#include "TimerOne.h"
/*
 https://playground.arduino.cc/Code/Timer1/

initialize(period [us])
setPeriod(period [us])
pwm(pin, duty [0-1023], period [us])
setPwmDuty(pin, duty [0-1023])
disablePwm(pin)
 
void initialize(long microseconds=1000000);
void start();
void stop();
void restart();
unsigned long read();
void setPeriod(long microseconds);
void pwm(char pin, int duty, long microseconds=-1);
void setPwmDuty(char pin, int duty);
void disablePwm(char pin);
void attachInterrupt(void (*isr)(), long microseconds=-1);
void detachInterrupt();
*/


#define DEBUG

const int PWM1  = 9;  // PWM motor control
const int EN1   = 8;  // ON/OFF
const int LED_0 = 13; // on board led
const int SW1   = 2;  // switch

byte myspeed = 0;  // 255=max; 0=min
int  currentState;
bool incr;

void myanalogWrite(char pin, byte myspeed){
  int myduty;
  myspeed = ~myspeed;  // inverting because of external NMOSFET 2N7000
  myduty = (int)myspeed * 4; // duty on 10-bits
  Timer1.pwm(pin, myduty, 40); // 25 kHz (40us)
  #ifdef DEBUG
  Serial.println(myduty);
  #endif
}

void setup() {

  Serial.begin(9600);

  pinMode(PWM1, OUTPUT);
  pinMode(EN1, OUTPUT);
  pinMode(LED_0, OUTPUT);
  pinMode(SW1, INPUT_PULLUP);

  currentState = 0;
  incr=1;
  myspeed = 0;
  digitalWrite(EN1, LOW);

  Timer1.initialize();
  //Timer1.pwm(PWM1, 512, 100000);
}

void loop() {  
  
 if (currentState == 0){
   if (digitalRead(SW1) == LOW){
     if (incr){
        myspeed += 10;
        if (myspeed>240) incr = 0;
     }
     else {
        myspeed -= 10;
        if (myspeed<15) incr = 1;       
     }
     
     if (myspeed==0){
       digitalWrite(EN1, LOW);
       //Timer1.stop();
     }
     else {
       digitalWrite(EN1, HIGH);  
       //myanalogWrite(PWM1, myspeed);
     }     
     myanalogWrite(PWM1, myspeed);
     
     Serial.write(myspeed);
     currentState = 1;
   }
 }
 else if (currentState == 1){    
    if (digitalRead(SW1) == HIGH){
      currentState = 0;
    }
 }

// Control speed from Serial
  if (Serial.available() > 0) {
     myspeed = Serial.read();

     if (myspeed==0) {
       digitalWrite(EN1, LOW);
       Timer1.pwm(PWM1, 0, 0);
       //Timer1.stop(); // .stop seems to stop output PWM1 in either states LOW OR HIGH. I need it to be LOW.
     }
     else {
       digitalWrite(EN1, HIGH);  
       myanalogWrite(PWM1, myspeed);
     }
     //myanalogWrite(PWM1, myspeed);
     //Serial.write(myspeed);

  }


}
