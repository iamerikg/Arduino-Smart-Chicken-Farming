## Including necessary libraries for working with the DHT22 sensor.
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

## Define the pin where the DHT22 sensor is connected.
#define DHTPIN 7
## Specify the type of DHT sensor (DHT22 in this case).
#define DHTTYPE DHT22

## Create an instance of the DHT_Unified class with the specified pin and type.
DHT_Unified dht(DHTPIN, DHTTYPE);

## Variable to hold the delay time between sensor readings.
uint32_t delayMS;

## Initialization code that runs once when the Arduino starts.
void setup() {
  ## Begin communication with the serial monitor at a baud rate of 9600.
  Serial.begin(9600);

  ## Initialize the DHT sensor.
  dht.begin();

  ## Print a message to indicate the type of sensor being used.
  Serial.println(F("Temperature and Humidity Sensor"));

  ## Get sensor details to determine the minimum delay between readings.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);

  ## Convert the minimum delay to milliseconds.
  delayMS = sensor.min_delay / 1000;
}

## Main loop that continuously reads and prints sensor data.
void loop() {
  ## Introduce a delay between sensor readings.
  delay(delayMS);

  ## Create a structure to hold sensor event data.
  sensors_event_t event;

  ## Read temperature sensor event and store it in the event structure.
  dht.temperature().getEvent(&event);

  ## Check if the temperature reading is valid.
  if (isnan(event.temperature)) {
    ## Print an error message if the temperature reading is invalid.
    Serial.println(F("Error reading temperature!"));
  } else {
    ## Print the temperature reading to the serial monitor.
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }

  ## Read humidity sensor event and store it in the event structure.
  dht.humidity().getEvent(&event);

  ## Check if the humidity reading is valid.
  if (isnan(event.relative_humidity)) {
    ## Print an error message if the humidity reading is invalid.
    Serial.println(F("Error reading humidity!"));
  } else {
    ## Print the humidity reading to the serial monitor.
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));

    ## Print a visual separator.
    Serial.println("--------------------------------------------------------------------------------------------------");
  }
}
