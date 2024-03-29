// wifi
// ============================= Connect the ESP to the router =============================
// Connect to WiFi network so we can reach the MQTT broker and publish messages to topics.

/*
  Make sure you include at the start of the sketch:
  #define hostPrefix "CGM-"
  #include "ESP8266WiFi.h"   // Not needed if also using the Arduino OTA Library...
  #include "D:\River Documents\Arduino\libraries\Kaywinnet.h"  \\ WiFi credentials

  If using the OTA Library, put these at the start of the sketch.
  // setup_wifi vars
  char macBuffer[24];       // Holds the last three digits of the MAC, in hex.
  char hostNamePrefix[] = hostPrefix;
  char hostName[24];        // Holds hostNamePrefix + the last three bytes of the MAC address.
*/

void setup_wifi() {
#ifndef Kaywinnet
#include <Kaywinnet.h>              // WiFi credentials
#endif

  byte mac[6];                      // the MAC address of your Wifi shield
  int ledState = HIGH;              // LED off

  Serial.println(F("\n"));
  Serial.print(F("Connecting to "));
  Serial.println(MY_SSID);


  WiFi.mode(WIFI_STA);
  WiFi.begin(MY_SSID, MY_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    //Toggle the blue LED
    digitalWrite(BLUELED, ledState);
    ledState = !ledState;
    delay(400);
    Serial.print(WiFi.status()); Serial.print(F(" "));
  }
  Serial.println(F("\nWiFi connected, "));
  Serial.print(F("MAC Address: "));
  Serial.println(WiFi.macAddress());
  Serial.print(F("IP address: "));
  Serial.println(WiFi.localIP());
  //Turn blue LED on, indicating "connected"
  digitalWrite(BLUELED, LOW);


  // Get the last three numbers of the mac address.
  // "4C:11:AE:0D:83:86" becomes "0D8386" in macBuffer.
  WiFi.macAddress(mac);
  snprintf(macBuffer, sizeof(macBuffer), "%02X%02X%02X", mac[3], mac[4], mac[5]);

  // Build hostNamePrefix + last three bytes of the MAC address.
  strcpy(hostName, hostNamePrefix);
  strcat(hostName, macBuffer);

  Serial.print(F("hostName = \""));
  Serial.print(hostName);
  Serial.println(F("\""));

}
