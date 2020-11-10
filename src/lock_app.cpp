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
int actuator_test(String command);
int servoAngle(String command);
int master_command(const char *data);
#line 8 "d:/Git-repos/Key-iot-app/src/lock_app.ino"
# include "Particle.h"

// diodes for showing door status
int green_diode = D6;
int red_diode = D5;
int green_diode_val;
int red_diode_val;

// servo pin D2
Servo myservo; 
int pos = 0;

// sensor for sensing door closing
// has to be an ADC channel!!
int sensor = A0;
int sensor_val;

// setup() runs once, when the device is first turned on.
void setup() {
  // Set up diode pin's
  pinMode(green_diode, OUTPUT);
  digitalWrite(green_diode, LOW);
  pinMode(red_diode, OUTPUT);
  digitalWrite(red_diode, LOW);

  // Set up sensor pin
  pinMode(sensor, INPUT_PULLUP);

  // For manual actuator testing
  Particle.variable("Actuator value", &green_diode_val, INT);
  Particle.variable("Sensor value", &sensor_val, INT);
  Particle.function("Actuator test", actuator_test);
  Particle.function("Master", master_command);

  // set up servo
  Particle.function("servo",servoAngle);
  myservo.attach(D2);  // attaches the servo on the D0 pin to the servo object
}


void loop() {
  green_diode_val = analogRead(green_diode);
  sensor_val = analogRead(sensor);
}

// Actuator manual testing function
int actuator_test(String command)
{
  if (command == "on")
  {
    digitalWrite(green_diode, HIGH);
    Particle.publish("Actuator activated", PRIVATE);
    return 1;
  }
  else if (command == "off")
  {
    digitalWrite(green_diode, LOW);
    Particle.publish("Actuator deactivated", PRIVATE);
    return 0;
  }
  else
  {
    Particle.publish("Failure in command", PRIVATE);
    return -1;
  } 
}

/* 
servo control function

commands:
"toggle" = ?
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
    // make it possible to read JSON string
    String val = iter.value().toString().data();

    // go through JSON string
    if (iter.name() == "access")
    {
      if (val == "allow")
      {
        // publish to thingspeak, so guest can recieve ack msg.
        Particle.publish("Lock_hook", "master allow", PRIVATE);
        // open door
        Particle.publish("door is open", PRIVATE);
        digitalWrite(green_diode, HIGH);
        servoAngle("unlock");

        // door gets closed sensing
        delay(10000); //given time to open the door
        sensor_val = analogRead(sensor);
        if (sensor_val >= 50) // door is closed
        {
          digitalWrite(green_diode, LOW);
          servoAngle("lock");
          Particle.publish("door is closed", PRIVATE);
          return 1;
        }
        else
        {
          while(sensor_val <= 50) // door is open
          {
            sensor_val = analogRead(sensor);
          }
          digitalWrite(green_diode, LOW);
          servoAngle("lock");
          Particle.publish("door is closed", PRIVATE);
          return 1;
        }
        
      }
      else if (val == "deny")
      {
        // publish to thingspeak, so guest can recieve deny msg.
        Particle.publish("Lock_hook", "master deny", PRIVATE);
        // be sure door is locked, just for good measure
        
        servoAngle("lock");
        digitalWrite(red_diode, HIGH);
        delay(3000);
        digitalWrite(red_diode, LOW);
        return 0;
      }
      else
      {
        // publish to thingspeak, so guest can recieve error msg.
        Particle.publish("Lock_hook", "master command error:", PRIVATE);
        // publish to particle event log, so owner can see error msg.  
        Particle.publish("master command error", val, PRIVATE);
        return -1;
      }
    }
  }
  return -2;
}

/*
Unlock request has been removed, so it will run through thingspeak's api instead.
The thingspeak command will be:
https://api.thingspeak.com/update?api_key=<write API key>&field1=0
*/