# lock_app

A Particle project named lock_app.
The idea is to have an app on you and your friends phone, which can open the lock on your and theirs door.
To give a friend permission to send a request from their app, thye will need a device id, and a security token.
The token can be deleted and remade in case of "security breaches"

If your at your friends house, you can send a request from the app to the argon device,
and the house owner will then get a notification that a friend would like to enter your home.
When the owner gets a notifaction, he would be able to accept or decline.

all these request's and response's work through curl commands to the argon device, and through the thingspeak API.


+------------------------------------------------------------------------------------------------------------------------------------------------------------+

             +-----------+                 +---------+                    +----------------+                  +-----------+                     +------------+
             |   Argon   |                 | webhook |                    | Thingspeak API |                  | Guest app |                     | Master app |
             +----+------+                 +---+-----+                    +-------+--------+                  +-----+-----+                     +------+-----+
                  |                            |                                  |                                 |                                  |
                  |                            | unlock door curl request (guest) |                                 |                                  |
                  +<------------------------------------------------------------------------------------------------+                                  |
                  |    Create request e^ent    |                                  |                                 |                                  |
                  +--------------------------->+                                  |                                 |                                  |
                  |                            |        Send to thingspeak        |                                 |                                  |
                  |                            +--------------------------------->+                                 |                                  |
                  |                            |                                  |   Send to master app (owner)    |                                  |
                  |                            |                                  +------------------------------------------------------------------->+
+-------------+   |                            |                                  |                                 |  Curl command <accept|decline>   |
| unlock door +<---<-----------------------------------------------------------------------------------------------------------------------------------+
+-------------+   |   Create Event <status>    |                                  |                                 |                                  |
                  +--------------------------->+                                  |                                 |                                  |
                  |                            |                                  |                                 |                                  |
                  +                            +                                  +                                 +                                  +

+------------------------------------------------------------------------------------------------------------------------------------------------------------+

               +----------------------------------------+  +---------------------+  +---------------------------------------+
               |               Argon                    |  |      Weebhook       |  |                APP                    |
               |                                        |  |                     |  |                                       |
               |   Generel setup                        |  |act upon "lock_hook" |  | Guest functions                       |
               |    *create lock actuator var. (output) |  |e^ent's              |  |  *send request to friends door lock,  |
               |    *create door sensor var. (input)    |  +---------------------+  |   through curl command                |
               |                                        |  +---------------------+  |                                       |
               |   Master curl command                  |  |   Thingspeak API    |  | Master functions                      |
               |    *open door                          |  |                     |  |  *accept|decline request's            |
               |    *door status sensor check           |  |log "lock_hook"      |  |  *open own door                       |
               |    *lock door                          |  |event's              |  |                                       |
               |                                        |  |                     |  +---------------------------------------+
               |   Request curl command                 |  |generate URL for     |
               |    *create request event               |  |data                 |
               |                                        |  +---------------------+
               +----------------------------------------+

+------------------------------------------------------------------------------------------------------------------------------------------------------------+




## Project folders

#### ```/src``` folder:  
This is the source folder that contains the firmware files for your project. It should *not* be renamed. 
Anything that is in this folder when you compile your project will be sent to our compile service and compiled into a firmware binary for the Particle device that you have targeted.

If your application contains multiple files, they should all be included in the `src` folder. If your firmware depends on Particle libraries, those dependencies are specified in the `project.properties` file referenced below.

#### ```.ino``` file:
This file is the firmware that will run as the primary application on your Particle device. It contains a `setup()` and `loop()` function, and can be written in Wiring or C/C++. For more information about using the Particle firmware API to create firmware for your Particle device, refer to the [Firmware Reference](https://docs.particle.io/reference/firmware/) section of the Particle documentation.

#### ```project.properties``` file:  
This is the file that specifies the name and version number of the libraries that your project depends on. Dependencies are added automatically to your `project.properties` file when you add a library to a project using the `particle library add` command in the CLI or add a library in the Desktop IDE.

