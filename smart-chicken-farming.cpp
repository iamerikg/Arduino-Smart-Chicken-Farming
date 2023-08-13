## Include necessary libraries.
#include <SoftwareSerial.h>
#include <Wire.h>
#include "HX711.h"
#include "DHT.h"

## Define DHT22 pin and type.
#define DHTPIN 7                 
#define DHTTYPE DHT22 

## Define pins for HX711 and MQ137.
#define DOUT  8
#define CLK  9
#define MQ_sensor A5

## Define resistor value for gas sensor.
#define RL 47
## Define linear calibration parameters for MQ137.
#define m -0.263
#define b 0.42
## Define reference resistance for MQ137.
#define Ro 82.12

## Define pins for relay control.
#define RELAY_FAN_PIN A1
#define RELAY_HUMIDIFIER_PIN A0

## Define temperature and humidity thresholds.
const int TEMP_THRESHOLD_UPPER = 33;
const int TEMP_THRESHOLD_LOWER = 30;
const int HUMID_THRESHOLD_UPPER = 60;
const int HUMID_THRESHOLD_LOWER = 50;

## Global variables.
char Received_SMS;
char call;
short STATUS_OK = -1;
String Data_SMS;

## Create a SoftwareSerial instance named "mySerial" using pins 3 (RX) and 2 (TX).
SoftwareSerial mySerial(3, 2);

## Create DHT instance and HX711 instance.
DHT dht(DHTPIN, DHTTYPE);
HX711 scale(DOUT, CLK);

## Initialization code that runs once when the Arduino starts.
void setup() {
  ## Begin communication with hardware and software serial ports.
  mySerial.begin(9600);
  Serial.begin(9600);

  ## Initialize scale for weight measurements.
  scale.set_scale(199.30);
  scale.tare();

  ## Initialize DHT sensor.
  dht.begin();

  Serial.println("Starting ...");
  delay(3000);
  
  ## Update and receive initial SMS.
  updateSerial();
  ReceiveMode();
}

## Main loop that continuously runs.
void loop() {
  ## String to hold received SMS.
  String RSMS;

  ## Read incoming characters from software serial port.
  while (mySerial.available() > 0) {
    Received_SMS = mySerial.read();
    Serial.print(Received_SMS);
    RSMS.concat(Received_SMS);
    STATUS_OK = RSMS.indexOf("STATUS");
  }

  ## Check for "STATUS" command in SMS.
  if (STATUS_OK != -1) {
    Serial.println("found DHT22, MQ137, HX711");

    ## Read temperature and humidity from DHT22.
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    ## Read MQ137 sensor values.
    float VRL = analogRead(MQ_sensor) * (5.0 / 1023.0);
    float Rs = ((5.0 * RL) / VRL) - RL;
    float ratio = Rs / Ro;
    float ppm = pow(10, ((log10(ratio) - b) / m));

    ## Read weight from HX711.
    float Feeder = scale.get_units();

    ## Control humidifier and fan heater based on thresholds.
    controlDevices(temperature, humidity);

    ## Print sensor values to Serial monitor.
    Serial.print("DHT22 Temperature = ");
    Serial.print(temperature);
    Serial.print("*C      DHT22 Humidity = ");
    Serial.print(humidity);
    Serial.println(" %");
    Serial.print("NH3 = ");
    Serial.print(ppm);
    Serial.println(" ppm");
    Serial.print("Chicken Feeder Weight: ");
    Serial.print(scale.get_units(10), 2);
    Serial.println(" grams");

    ## Prepare SMS message.
    Data_SMS = "Smart Chicken Farming\nTemperature: " + String(temperature, 1) + "*C \nHumidity: " + String(humidity, 1) + " % \nNH3: " + String(ppm, 2) + " ppm \nChicken Feeder Weight: " + String(Feeder, 2) + " grams";
    
    ## Update Serial and send data.
    updateSerial();
    Send_Data();
    ReceiveMode();
    
    STATUS_OK = -1;
  }
}

## Function to control humidifier and fan heater based on thresholds.
void controlDevices(float temperature, float humidity) {
  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT22 sensor!");
  } else {
    if (temperature > TEMP_THRESHOLD_UPPER) {
      Serial.println("The humidifier is turned on");
      digitalWrite(RELAY_HUMIDIFIER_PIN, HIGH);
    } else if (temperature < TEMP_THRESHOLD_LOWER) {
      Serial.println("The humidifier is turned off");
      digitalWrite(RELAY_HUMIDIFIER_PIN, LOW);
    }
  }

  if (isnan(humidity)) {
    Serial.println("Failed to read from DHT22 sensor!");
  } else {
    if (humidity > HUMID_THRESHOLD_UPPER) {
      Serial.println("The fan heater is turned on");
      digitalWrite(RELAY_FAN_PIN, HIGH);
    } else if (humidity < HUMID_THRESHOLD_LOWER) {
      Serial.println("The fan heater is turned off");
      digitalWrite(RELAY_FAN_PIN, LOW);
    }
  }
}

## Function to update both hardware and software serial ports.
void updateSerial() {
  delay(500);
  while (Serial.available()) {
    mySerial.write(Serial.read());
  }
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }
}

## Function to receive incoming call details.
void get_Call() {
  mySerial.println("AT");
  updateSerial();
  mySerial.println("AT+CMGF=1");
  updateSerial();
  mySerial.println("AT+CNMI=2,2,0,0,0");
  updateSerial();
}

## Function for serial communication.
void Serialcom() {
  delay(500);
  while (Serial.available()) {
    mySerial.write(Serial.read());
  }
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }
}

## Function to set up the receive mode for SMS.
void ReceiveMode() {
  mySerial.println("AT");
  Serialcom();
  mySerial.println("AT+CMGF=1");
  Serialcom();
  mySerial.println("AT+CNMI=2,2,0,0,0");
  Serialcom();
}

## Function to send collected data via SMS.
void Send_Data() {
  Serial.println("Sending Data...");
  mySerial.print("AT+CMGF=1\r");
  delay(100);
  mySerial.print("AT+CMGS=\"+639XXXXXXXXX\"\r");  // Replace XXXXXXXXXX with recipient's phone number
  mySerial.print(Data_SMS);
  delay(500);
  mySerial.print((char)26);  // Send Ctrl+Z to indicate end of message
  delay(500);
  mySerial.println();
  Serial.println("Data Sent.");
  delay(500);
}
