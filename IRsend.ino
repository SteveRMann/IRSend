/* This sketch sends discrete IR codes from a NodeMCU.
 *  
   D:\River Documents\Arduino\Arduino Projects\IR\NodeMCU\IRsend
   Board: NodeMCU 1.0

   Arduino IDE settings for NodeMCU:
     Board: NodeMCU 1.0 (ESP12-E Module)

   8/30/2018- Added FlashLed()
   9/24/2018- Added second button


*/


#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRsend.h>


IRsend irsend(14);  // An IR LED is controlled by GPIO14 (D5)


// Data captured by IRrecvDumpV2.ino
//
uint16_t Vzio_Power_Toggle[71] = {8978, 4548,  554, 578,  526, 606,  524, 1732,  530, 606,
                                  524, 606,  526, 604,  524, 606,  524, 606,  552, 1706,
                                  554, 1706,  528, 604,  524, 1732,  554, 1706,  528, 1736,
                                  552, 1706,  552, 1710,  552, 582,  550, 582,  522, 606,
                                  524, 1732,  558, 578,  550, 580,  524, 606,  524, 606,
                                  552, 1642,  620, 1706,  554, 1708,  554, 580,  524, 1734,
                                  554, 1708,  552, 1706,  558, 1698,  558, 39228,  8980,
                                  2294,  528
                                 };  // NEC 20DF10EF

uint16_t Vizio_Info_button[71] = {9036, 4486,  532, 598,  588, 542,  532, 1728,  532, 598,  586,
                                  544,  532, 598,  588, 540,  586, 542,  532, 1728,  590, 1670,
                                  532, 598,  588, 1672,  586, 1674,  588, 1672,  586, 1676,  586,
                                  1672,  588, 1674,  588, 1672,  586, 542,  590, 1672,  586, 1674,
                                  532, 598,  532, 598,  588, 540,  588, 542,  530, 600,  586, 1672,
                                  532, 598,  588, 542,  586, 1672,  590, 1670,  588, 1668,  586,
                                  39188,  8978, 2290,  586
                                 };  // NEC 20DFD827


uint16_t Kenmore_Fan_Power_button[99] = {8948, 4590,  528, 1754,  556, 594,  588, 560,  500, 622,  588, 558,
                                         588, 560,  526, 630,  552, 562,  586, 562,  586, 1692,  502, 1752,
                                         586, 1690,  530, 1750,  500, 1752,  528, 1748,  588, 1692,  500, 1754,
                                         526, 1748,  530, 618,  526, 600,  526, 618,  586, 560,  528, 618,  500,
                                         622,  584, 562,  586, 558,  586, 1690,  502, 1750,  528, 1750,  584,
                                         1692,  500, 1752,  586, 1688,  530, 1746,  500, 1752,  526, 618,  586,
                                         560,  500, 622,  584, 560,  584, 562,  534, 612,  500, 622,  586, 558,
                                         528, 1746,  524, 1728,  534, 1740,  586, 1688,  526, 1728,  582,
                                         1688,  530
                                        };  // UNKNOWN 265BCC7E

uint16_t Fios_FF[39] = {8860, 4524,  424, 4522,  450, 4500,  478, 4470,  448, 4498,  448, 4500,  478,
                        4466,  450, 2270,  446, 2270,  448, 2268,  424, 2296,  476, 2242,  422,
                        2294,  424, 2294,  422, 4524,  448, 4500,  446, 4500,  480, 21718,
                        8862, 2286,  422
                       };  // UNKNOWN 9E787D5



// An IR LED is controlled by GPIO4 (D2)


const int button1Pin = 5;        //GPIO5 (D1)
const int button2Pin = 4;        //GPIO4 (D2)
int button1State = 0;
int button2State = 0;

void flashLed() {
  digitalWrite(D0, LOW);     //Turn the LED on
  delay(60);
  digitalWrite(D0, HIGH);    //Turn the LED off
}

//==============================
void setup() {
  //Set up button1
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(D0, OUTPUT);              // Built-in LED
  digitalWrite(D0, HIGH);           //Turn the LED off

  irsend.begin();
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
}



//==============================
void loop() {

  //Button 1, FF
  button1State = digitalRead(button1Pin);
  if (button1State == LOW)  //Sends the code once every time the button1 is pressed.
  {
    Serial.println("Sending data");
    irsend.sendRaw(Fios_FF, 39, 38);  // Send raw data.
    button1State = HIGH;
    flashLed();
    delay(150);
  }

  //Button 2, FF * 6
  button2State = digitalRead(button2Pin);
  if (button2State == LOW)  //Sends the code six times every time the button1 is pressed.
  {
    Serial.println("Sending data 6X");
    for (int i = 0; i < 5 ; i++) {
      irsend.sendRaw(Fios_FF, 39, 38);  // Send raw data.
      button2State = HIGH;
      flashLed();
      delay(150);
    }
  }

}
