#define SKETCH_NAME "IRSend.ino"
#define SKETCH_VERSION "Version 2.2 3/8/2021"
#define hostPrefix "ffBox-"

/* This sketch sends discrete IR codes from a NodeMCU to a Broadlink Mini 3.

   D:\River Documents\Arduino\Arduino Projects\IR\NodeMCU\IRsend
   Board: NodeMCU 1.0

   Arduino IDE settings for NodeMCU:
     Board: NodeMCU 1.0 (ESP12-E Module)

   8/30/2018- Added FlashLed()
   9/24/2018- Added second button
   10/24/2020- Added MQTT publish to send FF or REW codes to the Broadlink IR sender.
   10/25/2020- V2.1 Temproarily commented out the IR and just rely on MQTT.
   03/08/2021- V2.2 Flash LED_BUILTIN while connecting to WiFi.
                    Flash LED_BUILTIN with each FF pulse.
                    Extended delay between FF pulses.
                    Fixed error where we were only sending five FF pulses.


*/

//#include "D:\River Documents\Arduino\libraries\Kaywinnet.h"  \\ WiFi credentials
#include <Kaywinnet.h>          // WiFi credentials
#include <ESP8266WiFi.h>        // Connect (and reconnect) an ESP8266 to the a WiFi network.
#include <PubSubClient.h>       // connect to a MQTT broker and publish/subscribe messages in topics.

#define DEBUG true  //set to true for debug output, false for no debug ouput
#define Serial if(DEBUG)Serial
#define DBUG

// Declare an object of class WiFiClient, which allows to establish a connection to a specific IP and port
// Declare an object of class PubSubClient, which receives as input of the constructor the previously defined WiFiClient.
// The constructor MUST be unique on the network.
WiFiClient ffBox;
PubSubClient client(ffBox);

char macBuffer[24];             // Holds the last three digits of the MAC, in hex.
char hostNamePrefix[] = hostPrefix;
char hostName[24];              // Holds hostNamePrefix + the last three bytes of the MAC address.
IPAddress myIP;                 // Holds the IP address
int rssi;

#define NODENAME "ffBox"                                    // Give this node a name
const char *cmndTopic = NODENAME "/cmnd";                   // Incoming commands, payload is a command.
const char *connectName =  NODENAME "1";                    // Must be unique on the network
const char *mqttServer = MQTT_SERVER;                       // Local broker defined in Kaywinnet.h
const int mqttPort = 1883;




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
  digitalWrite(D0, HIGH);     //Turn the LED off
  delay(60);
  digitalWrite(D0, LOW);      //Turn the LED on
}


//main() calls setup() and loop()
