ESPTelnet telnet;

void onTelnetConnect(String ip) {
  Serial.print("- Telnet: ");
  Serial.print(ip);
  Serial.println(" connected");
  
  telnet.println("\nWelcome " + telnet.getIP());
  telnet.println("(Use ^] + q  to disconnect.)");
}

void onTelnetInput(String str) {
  // checks for a certain command
  if (str == "ping") {
    telnet.println("> pong");
    Serial.println("- Telnet: pong");
  // disconnect the client
  } else if (str == "bye") {
    telnet.println("> disconnecting you...");
    telnet.disconnectClient();
  }
   else if (str == "capture")
   {
     telnet.println("Capturing photo for upload.");
     if (uploadPhoto() == false)
     {
        telnet.println("Photo not captured or too dark.");
     }
     else
     {
        telnet.println("Success.");       
     }
   }
   else if (str == "status")
   {  
      unsigned int lastStreamMins =  ((millis() - lastStreamMillis)/1000)/60;
      telnet.print("Last streaming occured "); telnet.print(String(lastStreamMins)); telnet.println(" minutes ago.");

      if ( (millis() - lastStreamMillis) < 6000 )
      {
        telnet.println("Somebody is probably streaming right now.");
      }

       unsigned long lastCapture = millis() - lastCaptureMillis;

       telnet.print("Last photo capture upload taken ");
       telnet.print(String((lastCapture/1000/60)));
       telnet.println(" minutes ago.");

   } 
   else if (str == "uptime")
   {
      int64_t uptime = (esp_timer_get_time() - bootMicroSecond);

      int uptime_days = uptime / 1000000 / 60 / 60 / 24;
       telnet.print("Uptime: ");
       telnet.print(String(uptime_days));
       telnet.println(" days");

   } 


  }


void setupTelnet() {  
  // passing on functions for various telnet events
  telnet.onConnect(onTelnetConnect);
//  telnet.onConnectionAttempt(onTelnetConnectionAttempt);
//  telnet.onReconnect(onTelnetReconnect);
//  telnet.onDisconnect(onTelnetDisconnect);
  telnet.onInputReceived(onTelnetInput);

  Serial.print("- Telnet: ");
  if (telnet.begin(23)) {
    Serial.println("running");
  } else {
    Serial.println("error.");
   // errorMsg("Will reboot...");
  }
}  