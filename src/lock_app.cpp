/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "d:/Git-repos/Key-iot-app/src/lock_app.ino"
/*
 * Project lock_app
 * Description:
 * Author:
 * Date:
 */

void setup();
void loop();
int servoAngle(String command);
int master_command(const char *data);
int readSensor(String command);
#line 8 "d:/Git-repos/Key-iot-app/src/lock_app.ino"
# include "Particle.h"

// create servo object to control a servo
// a maximum of eight servo objects can be created
Servo myservo;

// variable to store the servo position
int pos = 0;

// variable for actuator power 
int power = A5;

// sensor for sensing door closing
// has to be an ADC channel!!
int analogvalue;
int photoresistor = A0;
int lightSensorLED = D3;

// diodes for showing door status
int greenLED = D4;
int redLED = D6;

// pre declaration
bool doorIsOpen();

// setup() runs once, when the device is first turned on.
void setup() {
    //setup cloud API
    Particle.function("master_command",master_command);
    Particle.function("sensor",readSensor);
    Particle.variable("analogvalue", &analogvalue, INT);
    
    // setup actuator
    myservo.attach(D2); 

    // setup pins
    pinMode(photoresistor, INPUT);
    pinMode(power, OUTPUT);
    pinMode(lightSensorLED, OUTPUT);
    pinMode(greenLED, OUTPUT);
    pinMode(redLED, OUTPUT);
}


void loop() 
{

}

/* 
servo control function

commands:
"toggle" = test's the actuator
"lock" = locks the door
"unlock" = unlocks the door
*/
int servoAngle(String command) {

    if(command == "toggle") {
        myservo.write(180);
        delay(3000);
        myservo.write(0);
    } 

    else if(command == "lock") {
        myservo.write(0);
    }
    else if(command == "unlock") {
        myservo.write(180);
    } 
    else {
       myservo.write(command.toInt()); 
    }


    return 0;

}

// jan: 
//      device id: e00fce6864ea86ea7bb98251
//      token: 6c8b51fd99f54209516d578e06384b56cc0a9113
//
// Actuator call function from https:
// Curl string (without -, spaces and in one string):
//    curl https://api.particle.io/v1/devices/<DEVICE-ID>/Master? -
//    access_token=<ACCESS-TOKEN> -d "arg={access:<deny or allow>}"
//
//    replace string in <>
//    in windows prompt, it might be neccessary to write curl.exe instead of
//    curl
int master_command(const char *data)
{
  //insert JSON string into JSON object, and itterate through object
  JSONValue master_msg = JSONValue::parseCopy(data);
  JSONObjectIterator iter(master_msg);

  while(iter.next())
  {
    //val for loop
    int i = 0;

    // make it possible to read JSON string
    String command = iter.value().toString().data();

    // go through JSON string
    if (iter.name() == "access")
    {
      if(command == "Accept") {
          // open door
          digitalWrite(power,HIGH);
          digitalWrite(lightSensorLED, HIGH);
          delay(200);
          if(!doorIsOpen()) {
            Particle.publish("lock_hook", "1", PRIVATE);
            Particle.publish("door is open", PRIVATE);
            digitalWrite(greenLED, HIGH);
            myservo.write(180);
            delay(10000);
            while(doorIsOpen())
                delay(10);
            myservo.write(0);
            Particle.publish("door is closed", PRIVATE);
            digitalWrite(greenLED, LOW); 
            digitalWrite(lightSensorLED, LOW);
            digitalWrite(power,LOW);
            return 0;
          }
          else {
            Particle.publish("door is already open", PRIVATE);
            Particle.publish("lock_hook", "-2", PRIVATE);
            for(i=0;i<=4;i++)
            {
              digitalWrite(greenLED, HIGH);
              delay(500);
              digitalWrite(greenLED, LOW);
              digitalWrite(redLED, HIGH);
              delay(500);
              digitalWrite(redLED, LOW);
            }
            digitalWrite(lightSensorLED, LOW);
            digitalWrite(power,LOW);
            return -2;
          }
          return 1;
      } 
      else if(command == "Deny") {
          Particle.publish("lock_hook", "2", PRIVATE);
          digitalWrite(redLED, HIGH);
          delay(2000);
          digitalWrite(redLED, LOW);
          return 2;
      }
      else
      {
        // indicate on nthingspeak, an error has been recieved
        Particle.publish("lock_hook", "-1", PRIVATE);
        // publish to particle event log, so owner can see error msg.  
        Particle.publish("master command error", command, PRIVATE);
        return -1;
      }
    }
  }
  return -2;
}

// function for reading door status
bool doorIsOpen() {
    if(analogRead(photoresistor)>100)
        return true;
    else
        return false;
}

// function for reading sensor value
// cloud API function
int readSensor(String command){
    digitalWrite(power,HIGH);
    digitalWrite(lightSensorLED, HIGH);
    delay(200);
    analogvalue = analogRead(photoresistor);
    digitalWrite(power,LOW);
    digitalWrite(lightSensorLED, LOW);
    return analogvalue;
}

/*
Unlock request has been removed, so it will run through thingspeak's api instead.
The thingspeak command will be:
https://api.thingspeak.com/update?api_key=<write API key>&field1=0
*/