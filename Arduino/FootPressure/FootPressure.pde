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

/*
 * Arduino responds to char 'A' from the serial port.
 * Replies with:
 * FOOT,<value heel>,<value toes>,END;
 */

//FOOT, arduino pro mini 3.3v connected to FSRs in divider with 1.5K resistor talking to bluetooth at 115200 Baud on a bluetooth mate.

void setup()
{
  Serial.begin(115200);
}


void loop() { 

  if(Serial.available() > 0){
    char inByte = Serial.read();

    if (inByte == 'A'){
        int a=analogRead(3);
        int b=analogRead(2);
        Serial.print("FOOT,");
        Serial.print(a);
        Serial.print(",");
        Serial.print(b);
        Serial.println(",END;");
    }
      Serial.flush();
  }
}


