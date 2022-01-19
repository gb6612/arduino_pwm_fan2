# arduino_pwm_fan2
Test of PC Fan "be-quiet pure wings 2"

   Target : Nano

   Description : 
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

