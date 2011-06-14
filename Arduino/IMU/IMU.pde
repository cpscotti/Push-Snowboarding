

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
 * IMU,<gyro's x>,<gyro's y>,<gyro's z>,<acc's x>,<acc's y>,<acc's z>,<mag's x>,<mag's y>,<mag's z>,END;
 */

#include <Wire.h>

int XGyro, YGyro, ZGyro;
int XAcc, YAcc, ZAcc;
int XMag, YMag, ZMag;
volatile boolean inService = false;

void setup()
{
  Wire.begin();            // join i2c bus (address optional for master)
  Serial.begin(115200);      // sets the serial port to 9600

  // Setup pins
  pinMode(2, INPUT);
  digitalWrite(2, LOW);

  // Light pin 13 when ready
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  // Setup hmc5843
  Wire.beginTransmission(30);
  Wire.send(0x02);
  Wire.send(0x00);
  Wire.endTransmission();

  // Setup Gyro
  Wire.beginTransmission(0x69);
 Wire.send(0x3E);
 Wire.send(0x80);  //send a reset to the device
 Wire.endTransmission(); //end transmission

 Wire.beginTransmission(0x69);
 Wire.send(0x15);
 Wire.send(0x00);   //sample rate divider
 Wire.endTransmission(); //end transmission

 Wire.beginTransmission(0x69);
 Wire.send(0x16);
 Wire.send(0x18); // ¬±2000 degrees/s (default value)
 Wire.endTransmission(); //end transmission

 Wire.beginTransmission(0x69);
 Wire.send(0x17);
 Wire.send(0x05);   // enable send raw values
 Wire.endTransmission(); //end transmission


  // Take first readings
  delay(500);
  readAcc();
  readhmc5843();
  readGyro();
  
  // Using interrupt 0 on digital pin 2.
  attachInterrupt(0, serialInterrupt, CHANGE);
  
  // Ready to go!
  digitalWrite(13, HIGH);
}

void loop() 
{
  readAcc();
  readhmc5843();
  readGyro();
}

void readAcc(){
  XAcc = analogRead(0);       // read analog input pin 0
  YAcc = analogRead(1);       // read analog input pin 1
  ZAcc = analogRead(2);       // read analog input pin 1
}

void readhmc5843(){
  
  Wire.beginTransmission(30);
  Wire.send(0x03);
  Wire.endTransmission();
  Wire.requestFrom(30, 6);    // request 6 bytes from slave device #2

    // Get X

  while(!Wire.available()){
  } 
    
    int xH = Wire.receive();
  
   while(!Wire.available()){
    
  } 
    int xL = Wire.receive();
    
    XMag = xH << 8 | xL;
    
    // Get Y

    while(!Wire.available()){
  } 
    
    int yH = Wire.receive();
  
   while(!Wire.available()){
    
  } 
    int yL = Wire.receive();
    
    YMag = yH << 8 | yL;
    
    // Get Z

    while(!Wire.available()){
  } 
    
    int zH = Wire.receive();
  
   while(!Wire.available()){
    
  } 
    int zL = Wire.receive();
    
    ZMag = zH << 8 | zL;
}

void readGyro(){
  
  Wire.beginTransmission(0x69);
  Wire.send(0x1D);
  Wire.endTransmission();
  Wire.requestFrom(0x69, 6);    // request 6 bytes from slave device #2

  // Get X

  while(!Wire.available()){
  } 
    
    int xH = Wire.receive();
  
   while(!Wire.available()){
    
  } 
    int xL = Wire.receive();
    
    XGyro = xH << 8 | xL;
    
    // Get Y

    while(!Wire.available()){
  } 
    
    int yH = Wire.receive();
  
   while(!Wire.available()){
    
  } 
    int yL = Wire.receive();
    
    YGyro = yH << 8 | yL;
    
    // Get Z

    while(!Wire.available()){
  } 
    
    int zH = Wire.receive();
  
   while(!Wire.available()){
    
  } 
    int zL = Wire.receive();
    
    ZGyro = zH << 8 | zL;
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
    Serial.print("IMU,");
    Serial.print(XGyro);
    Serial.print(",");
    Serial.print(YGyro);
    Serial.print(",");
    Serial.print(ZGyro);
    Serial.print(",");
    Serial.print(XAcc);
    Serial.print(",");
    Serial.print(YAcc);
    Serial.print(",");
    Serial.print(ZAcc);
    Serial.print(",");
    Serial.print(XMag);
    Serial.print(",");
    Serial.print(YMag);
    Serial.print(",");
    Serial.print(ZMag);
    Serial.println(",END;");

  }
  
    Serial.flush();

  // Job done.
  inService = false;
}

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

