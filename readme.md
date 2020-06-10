### Description
P07_00_00 Raindrop Bob  
Raindrop Bob is a domotics project for watering and monitoring plant moisture for house plants. The project incorporates aspects of soil moisture sensing, automatic irrigation, water level sensing and data logging. This project is designed to address issues of maintaining plant health while I’m away. 
 * Soil moisture sensing with homebrew sensors
 * watering managed with 3x DC peristaltic pumps
 * Reservoir monitoring (tbd)
 * Data logging (tbd) 

### Notes to user
 * Project documented at https://kionokitse.wordpress.com/raindrop-bob/

#### Folders
 * P07_00_00-Main
	* Main program with all the functions
	* Not fully operational yet
 * P07_00_00-TestSD
	* Testing SD card functions. Write a message to the txt file
 * P07_00_00-TestSensor
	* Test diy soil moisture sensor
 * P07_00_00-CalibrateSensors
	* Every hour read sensors and save data to SD card in CSV format	
 * P07_00_00-TestSensorsLive
	* Live stream data from the soil moisture sensor
	* Save data to SD card in CSV format
	* Transmit data over serial connection

### Hardware
 * Arduino Pro Mini clone ATmega328 5V
 * FTDI Basic
 * 3x 2N4400 NPN Transistor
 
### Progress log 
 * 2020-05-23 Got most of the hardware for the project 
 * 2020-05-23 Testing blink sketch successful
 * 2020-05-23 Test relay output (Successful) 
 * 2020-05-23 Test diy soil moisture sensor (Successful)
 * 2020-05-23 Search for a way to detect reservoir volume
 * 2020-05-29 Made soil moisture sensors
 * 2020-05-29 Building capacitive water tank
 * 2020-06-03 Discovered that I didn't do a great job on soldering the SD Card so that's why it wasn't working
 * 2020-06-04 Need to supply 5V not 3.3V power supply to SD card
 * 2020-06-05 Wired up the full setup without motors and sensors
 * 2020-06-05 Discovered that getting 3.3V with 1kohm resistors and 2.2kohm resistors doesn't work 100ohm and 220ohm resistors works well remember CS pin 10
 * 2020-06-06 Testing motor control: 40*500ms = 1Tb of water
 * 2020-06-06 Testing soil moisture sensors all seem to work needs calibration
 * 2020-06-07 Building capacitive reservoir level sensor
 * 2020-06-07 Shielding reservoir sensor wires it's a pia
 * 2020-06-07 Capacitive reservoir sensor is working calibration needed 
 * 2020-06-08 Add project to GitHub
 * 2020-06-09 Added TestSD project 
 * 2020-06-09 Added TestSensor project
 

pseudo code P08_00_00
Setup SD card and global variables
 1. Read soil moisture sensors
 1. Read reservoir sensor
 1. Record data to SD card
 1. Check if values are within thresholds
 1. Activate pumps if required
 1. Delay for 1 hour
 
### Notebook 
 * DIY Soil moisture sensor https://www.instructables.com/id/DIY-SOIL-MOISTURE-SENSOR-CHEAP-YET-ACCURATE-/
 * DIY Capacitive water level sensor https://www.instructables.com/id/Building-a-Capacitive-Liquid-Sensor/
 * DIY Arduino SD card reader https://www.instructables.com/id/Cheap-Arduino-SD-Card-Reader/
 * DIY SD card Reader https://nathan.chantrell.net/20111128/diy-micro-sd-shield-for-arduino/
 

 

