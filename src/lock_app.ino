/*
 * Project lock_app
 * Description:
 * Author:
 * Date:
 */

# include "Particle.h"

// actuator for opening the door
int actuator = D6;
int actuator_val;

// sensor for sensing door closing
// has to be an ADC channel!!
int sensor = A0;
int sensor_val;

// setup() runs once, when the device is first turned on.
void setup() {
  // Set up actuator pin
  pinMode(actuator, OUTPUT);
  digitalWrite(actuator, LOW);

  // Set up sensor pin
  pinMode(sensor, INPUT_PULLUP);

  // For manual actuator testing
  Particle.variable("Actuator value", &actuator_val, INT);
  Particle.variable("Sensor value", &sensor_val, INT);
  Particle.function("Actuator test", actuator_test);
  Particle.function("Master", master_command);
  Particle.function("Request", unlock_request);
}


void loop() {
  actuator_val = analogRead(actuator);
  sensor_val = analogRead(sensor);
}

// Actuator manual testing function
int actuator_test(String command)
{
  if (command == "on")
  {
    digitalWrite(actuator, HIGH);
    Particle.publish("Actuator activated", PRIVATE);
    return 1;
  }
  else if (command == "off")
  {
    digitalWrite(actuator, LOW);
    Particle.publish("Actuator deactivated", PRIVATE);
    return 0;
  }
  else
  {
    Particle.publish("Failure in command", PRIVATE);
    return -1;
  } 
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
        digitalWrite(actuator, HIGH);

        // door gets closed sensing
        delay(10000); //given time to open the door
        sensor_val = analogRead(sensor);
        if (sensor_val >= 50) // door is closed
        {
          digitalWrite(actuator, LOW);
          Particle.publish("door is closed", PRIVATE);
          return 1;
        }
        else
        {
          while(sensor_val <= 50) // door is open
          {
            sensor_val = analogRead(sensor);
          }
          digitalWrite(actuator, LOW);
          Particle.publish("door is closed", PRIVATE);
          return 1;
        }
        
      }
      else if (val == "deny")
      {
        // publish to thingspeak, so guest can recieve deny msg.
        Particle.publish("Lock_hook", "master deny", PRIVATE);
        // be sure door is locked, just for good measure
        digitalWrite(actuator, LOW);
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

// jan: 
//      device id: e00fce6864ea86ea7bb98251
//      token: 6c8b51fd99f54209516d578e06384b56cc0a9113
//
// Access request call function from https:
// Curl string (without -, spaces and in one string):
//    curl https://api.particle.io/v1/devices/<DEVICE-ID>/Request? -
//    access_token=<ACCESS-TOKEN> -d "arg={access:request}"
//
//    in windows prompt, it might be neccessary to write curl.exe instead of
//    curl
int unlock_request(const char *data)
{
  //insert JSON string into JSON object, and itterate through object
  JSONValue guest_msg = JSONValue::parseCopy(data);
  JSONObjectIterator iter(guest_msg);

  while(iter.next())
  {
    // make it possible to read JSON string
    String val = iter.value().toString().data();

    // go through JSON string
    if (iter.name() == "access")
    {
      if (val == "request")
      {
      Particle.publish("lock_hook", "guest request access", PRIVATE);
      return 1;
      }
      else
      {
        Particle.publish("guest request error", val, PRIVATE);
        return -1;
      }
    }
  }
  return -2;
}