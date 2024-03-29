//==============================
void setup() {
  //Set up button1
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(BLUELED, OUTPUT);                  // Built-in LED on GPIOP16
  digitalWrite(BLUELED, LEDOFF);             // Turn the LED off

  irsend.begin();                   //Why??
  //Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  Serial.begin(115200);
  Serial.println(SKETCH_NAME);

  digitalWrite(BLUELED, LEDON);

  setup_wifi();

  // Call the setServer method on the PubSubClient object, passing as first argument the
  // address and as second the port.
  client.setServer(mqttServer, mqttPort);
  mqttConnect();

  //Show the topics:
  Serial.print(F("cmndTopic= "));
  Serial.println(cmndTopic);

  digitalWrite(BLUELED, LEDOFF);


}
