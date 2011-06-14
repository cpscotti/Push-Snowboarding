
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
 * HRM,<average last 32 beats>,<average last 16 beats>,<average last beat>,END;
 */

volatile boolean inService = false;
long timeArray[] ={
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
long lastBeat = 0;
long bpm1, bpm8, bpm32;

void setup()
{
  Serial.begin(115200);      // sets the serial port to 9600

    // Setup pins
  pinMode(2, INPUT);
  digitalWrite(2, LOW);
  pinMode(2, INPUT);
  digitalWrite(2, LOW);

  // Light pin 13 when ready
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  // Using interrupt 0 on digital pin 2.
  attachInterrupt(0, serialInterrupt, CHANGE);

}

void loop() 
{
  while(!digitalRead(3)){
  }
  digitalWrite(13,HIGH);
  int i = 0;
  while(i<31){
    timeArray[i] = timeArray[i+1];
    i++;
  }
  long nowBeat = millis();
  timeArray[31] = nowBeat - lastBeat;
  lastBeat = nowBeat;

  bpm1 = 60000 / timeArray[31];

  int num = 0;
  long total = 0;
  int b = 31;
  while(b!=23){
    if(timeArray[b]!=0){
      total += timeArray[b];
      num++;
    }
    b--;
  }

  bpm8 = 60000 / (total / num);
  while(true){
    if(timeArray[b]!=0){
      total += timeArray[b];
      num++;
    }
    if(b==0) break;
    b--;
  }
  bpm32 = 60000 / (total / num);
  while(digitalRead(3)){
  }
  digitalWrite(13,LOW);
}

void serialInterrupt()
{
  // Already in service? Do nothing.
  if (inService) return;

  // You was not in service. Now you are.
  inService = true;

  // Reenable interrupts, to allow Serial to work. We do this only if inService is false.
  interrupts();

  // Allow serial to read at least one byte.
while(!Serial.available());

  if(Serial.read()=='A'){

    // Send Buffered data to phone
    // TODO
    Serial.print("HRM,");
    Serial.print(bpm32);
    Serial.print(",");
    Serial.print(bpm8);
    Serial.print(",");
    Serial.print(bpm1);
    Serial.println(",END;");

  }
  
  Serial.flush();

  // Job done.
  inService = false;
}

