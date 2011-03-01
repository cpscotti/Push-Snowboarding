/*
 * Copyright (c) 2011 Nokia Corporation
 *
 * This file is part of the Push Snowboarding Project, More info at:
 * http://pushsnowboarding.com/tech/developer-centre/
 *
 * This file is under the GPLv3 License, more details at:
 * http://www.gnu.org/licenses/
 *
 */

//GSR, arduino pro mini 3.3v connected to analog in port 3 with a 300K resistor tied to ground talking to bluetooth at 115200 Baud on a bluetooth mate.

void setup()
{
  Serial.begin(115200);
}


void loop() { 

  if(Serial.available() > 0){
    char inByte = Serial.read();

    if (inByte == 'A'){
        int a=analogRead(3);
        Serial.print("GSR,");
        Serial.print(a);
        Serial.println(",END;");
    }
    Serial.flush();
    
  }
}

