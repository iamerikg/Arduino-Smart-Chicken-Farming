# Smart Chicken Farming: Ammonia and Feeds Monitoring with Remote Temperature and Humidity Regulator

This project implements a smart farming system for monitoring ammonia levels, chicken feeder weight, temperature, and humidity in a chicken farm. The system utilizes Arduino boards, various sensors, and GSM communication to provide real-time data and control over the environment.

## Features

- **Ammonia Monitoring:** The MQ137 gas sensor is used to measure ammonia (NH3) levels in the chicken farm. The ammonia concentration is calculated and reported.
  
- **Chicken Feeder Weight Monitoring:** An HX711 load cell is employed to measure the weight of the chicken feeder, providing insights into feed consumption.

- **Temperature and Humidity Sensing:** The DHT22 sensor is used to measure temperature and humidity levels within the chicken farm.

- **Remote Control:** The system can send and receive SMS messages using a GSM module. It can remotely regulate the environment by turning on/off a humidifier and a fan heater based on predefined temperature and humidity thresholds.

## Components Used

- Arduino board
- DHT22 temperature and humidity sensor
- MQ137 gas sensor
- HX711 load cell amplifier
- GSM module (SIM800L, SIM900, etc.)
- Relays for controlling humidifier and fan heater

## Installation and Setup

1. Connect the sensors, load cell, and relays to the Arduino board based on the provided pin configuration.

2. Upload the provided Arduino code to the board using the Arduino IDE.

3. Insert a SIM card into the GSM module and connect it to the Arduino board.

4. Power up the system and make sure all components are working correctly.

## Usage

1. The system will monitor ammonia levels, chicken feeder weight, temperature, and humidity at predefined intervals.

2. The system will regulate the environment by turning on/off the humidifier and fan heater based on temperature and humidity thresholds.

3. When the "STATUS" command is received via SMS, the system will send back real-time data including temperature, humidity, ammonia levels, and feeder weight.


## Acknowledgements

- The project was inspired by the need to improve chicken farming efficiency and animal welfare.
- Thanks to the open-source community for providing libraries and resources used in this project.

## Contributors

- [Erick Garcia](https://www.linkedin.com/in/erickmanalastasgarcia/)
  
Feel free to contribute, improve, and adapt this project to your specific needs.
