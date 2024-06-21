void processSerialCommand() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove leading and trailing whitespaces

    if (input.startsWith("start#")) {
      int channelID = extractChannelID(input);
      testCount++;
      startResumeChannel(false, selectedChannel, "Sample", takeFirstReading(channelID - 1), getCurrentEpoch() );
    } else if (input.startsWith("stop#")) {
      int channelID = extractChannelID(input);
      stopChannel(channelID);
    } else if (input.startsWith("white#")) {
      int channelID = extractChannelID(input);
      if (channelID >= 1 && channelID <= numChannels) {
        takeWhite(channelID - 1);
      } else if (channelID == 100) {
        for (int i = 0; i < numChannels; i++) {
          takeWhite(i);
        }
      } else {
        Serial.println("Invalid channel ID. Please use a number between 1 and " + String(numChannels) + " or 100.");
      }
    } else if (input.startsWith("raw")) {
      String channelStr = input.substring(3); // Extract the numeric part after "raw"
      int i = channelStr.toInt(); // Convert the numeric part to an integer
      Serial.print(String(i) + ": ");
      if (i >= 1 && i <= 16) {
        Serial.print(sensorRead(2 * (i - 1)) );
        Serial.print(". ");
        Serial.println(sensorRead(2 * (i - 1) + 1) );
      }
    } else if (input.startsWith("setgain")) {
      String channelStr = input.substring(7); // Extract the numeric part after "setgain"
      int i = channelStr.toInt(); // Convert the numeric part to an integer
      Serial.println("setting gain for :" + String(i));
      if (i >= 1 && i <= 16) {
        determineBestGainForLED(2 * (i - 1)); // Set gain for the first LED in the pair
        determineBestGainForLED(2 * (i - 1) + 1); // Set gain for the second LED in the pair
        takeWhite(i - 1);
      }
    }
    else if (input == "setall") {
      for (int i = 1; i <= 16; i++)
      {
        determineBestGainForLED(2 * (i - 1)); // Set gain for the first LED in the pair
        determineBestGainForLED(2 * (i - 1) + 1); // Set gain for the second LED in the pair
        takeWhite(i - 1);
      }
    }
    else if (input == "blueall") {
      for (int i = 1; i <= 16; i++)
      {
        int blue1 = sensorRead(2 * (i - 1));
        int contrast1 = whiteIntensity[2 * (i - 1)] - blue1;

        Serial.println(String(i) + ": Blue1: " + String(blue1) + " white1: " + whiteIntensity[2 * (i - 1)] + " contrast1: " + String (contrast1) );
        //        Serial.println( "Blue2: " + String(sensorRead(2 * (i - 1) + 1)) + " white2: " + whiteIntensity[2 * (i - 1) + 1] );


        if (contrast1 < 2500) Serial.println("Low contrast! ");
      }
    }
    else if (input.startsWith("blue")) {
      String channelStr = input.substring(4); // Extract the numeric part after "setgain"
      int i = channelStr.toInt(); // Convert the numeric part to an integer
      Serial.println("checking blue for :" + String(i));
      if (i >= 1 && i <= 16) {
        Serial.println( "Blue1: " + String(sensorRead(2 * (i - 1))) + " white1: " + whiteIntensity[2 * (i - 1)] );
        Serial.println( "Blue2: " + String(sensorRead(2 * (i - 1) + 1)) + " white2: " + whiteIntensity[2 * (i - 1) + 1] );
      }
    }

    else if (input == "temp") {
      Serial.print("Temperature: ");
      Serial.println(temperature, 1); //1 decimal
    } else if (input == "setserial") {
      setSerial();
    } else if (input == "setssid") {
      setSSID();
      connectToWiFi();
    } else if (input == "setpassword") {
      setPassword();
      connectToWiFi();
    } else if (input == "setuser") {
      setUser();
    } else if (input == "setuserpassword") {
      setUserPass();
    } else if (input == "setrotation") {
      setRotationInterval();
    } else if (input == "all") {
      Serial.print(temperature);
      Serial.print(",");
    } else if (input == "rawdata") {
      Serial.println("rawdata");
      rawdata();
    } else if (input == "printwhite") {
      printwhite();
    } else if (input == "complete") {
      completedChannel(true, 6, 0);
    } else if (input == "serial") {
      Serial.println(serialNum);
    } else if (input == "first") {
      for (int i = 0; i < numChannels; i++) {
        Serial.print(String(firstReading[i]) + ",");
      }
      Serial.println();
    } else if (input == "rotate") {
      rotate_();
    }
    else if (input == "list") {
      updateTestCount();
    }
    else if (input == "readAll") {
      readSpiffData();
    } else if (input == "memory") {
      displaySpiffsUsage();
    } else if (input == "clear") {
      clearSpiffData();
    } else if (input == "synctime") {
      syncRtcTime();
    } else if (input == "settemp") {
      setTemperature();
    }
    else if (input == "restart") {
      ESP.restart(); // Restart the ESP32 board
    }
    else if (input == "format") {
      SPIFFS.format();
    }
    else if (input == "time") {
      Serial.println(getCurrentEpoch());
    }
    else if (input == "testsensor") {
      testSensor();
    }

    else if (input == "adjusttemp") {
      settempDiff();
    }

    else if (input.startsWith("angle")) {
      String angleStr = input.substring(5); // Extract the numeric part after "angle"
      int angleValue = angleStr.toInt(); // Convert the numeric part to an integer
      rotationAngle = angleValue;
      Serial.println("Rotation angle set to " + String(angleValue) + ".");
      // Save the rotationAngle value in EEPROM at address 301
      EEPROM.put(301, rotationAngle);
      EEPROM.commit(); // Save the changes to EEPROM
    }

    else if (input == "time") {
      Serial.println(getCurrentEpoch());
    }
    else if (input == "mac") {
      fetch_mac();
    }
    else if (input == "logs") {
      logflag = true;
    }

    else if (input == "limit") {
      if (digitalRead(LIMIT_PIN) == LOW) Serial.println("Limit Switch Pressed");
      else Serial.println("Limit Switch NOT Pressed");
    }

    //    else if (input.startsWith("testcount")) {
    //      String testcountStr = input.substring(9); // Extract the numeric part after "angle"
    //      int testcount = testcountStr.toInt(); // Convert the numeric part to an integer
    //      rotationAngle = angleValue;
    //      Serial.println("testcount set to " + String(testcount) + ".");
    //      // Save the rotationAngle value in EEPROM at address 301
    //      EEPROM.put(301, rotationAngle);
    //      EEPROM.commit(); // Save the changes to EEPROM
    //    }

    else {
      Serial.println("Invalid command: " + input);
    }
  }
}
