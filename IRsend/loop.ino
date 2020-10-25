//==============================
void loop() {
  //Make sure we stay connected to the mqtt broker
  if (!client.connected()) {
    mqttConnect();
  }
  if (!client.loop()) {
    client.connect(connectName);
  }

  //Button 1, FF
  button1State = digitalRead(button1Pin);
  if (button1State == LOW)  //Sends the code once every time the button1 is pressed.
  {
    Serial.println("Sending data");
    irsend.sendRaw(Fios_FF, 39, 38);  // Send raw data.
    client.publish(cmndTopic, "ff");
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
      client.publish(cmndTopic, "ff x6");
      button2State = HIGH;
      flashLed();
      delay(250);
    }
  }

}
