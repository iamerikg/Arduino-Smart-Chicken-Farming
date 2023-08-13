## Include the HX711_ADC library for working with the load cell.
#include <HX711_ADC.h>

## Define the pins for the HX711 connections.
const int HX711_dout = 8; 
const int HX711_sck = 9; 

## Create an instance of HX711_ADC with the specified pins.
HX711_ADC LoadCell(HX711_dout, HX711_sck);

## Initialize a variable to store time.
unsigned long t = 0;

## Initialization code that runs once when the Arduino starts.
void setup() {
  Serial.begin(57600);
  delay(10);
  Serial.println();
  Serial.println("Starting...");

  ## Initialize the load cell and perform initialization.
  LoadCell.begin();
  unsigned long stabilizingtime = 2000;
  boolean _tare = true;
  LoadCell.start(stabilizingtime, _tare);

  ## Check for timeout during initialization.
  if (LoadCell.getTareTimeoutFlag() || LoadCell.getSignalTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  } else {
    LoadCell.setCalFactor(1.0);
    Serial.println("Startup is complete");
  }

  ## Wait for initial tare operation to complete.
  while (!LoadCell.update());
  calibrate(); 
}

## Main loop that continuously reads load cell data and responds to commands.
void loop() {
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0;

  ## Check for new data from the load cell.
  if (LoadCell.update()) newDataReady = true;

  ## If new data is available, print it at specified intervals.
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float i = LoadCell.getData();
      Serial.print("Load_cell output val: ");
      Serial.println(i);
      newDataReady = 0;
      t = millis();
    }
  }

  ## Check for incoming commands from the serial monitor.
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') LoadCell.tareNoDelay(); 
    else if (inByte == 'r') calibrate(); 
    else if (inByte == 'c') changeSavedCalFactor(); 
  }

  ## Print a message when tare operation is complete.
  if (LoadCell.getTareStatus() == true) {
    Serial.println("Tare complete");
  }
}

## Function for performing calibration.
void calibrate() {
  Serial.println("***");
  Serial.println("Start calibration:");
  ## Instructions for taring the load cell.
  Serial.println("Place the load cell on a level stable surface.");
  Serial.println("Remove any load applied to the load cell.");
  Serial.println("Send 't' from the serial monitor to set the tare offset.");

  boolean _resume = false;
  while (_resume == false) {
    LoadCell.update();
    if (Serial.available() > 0) {
      char inByte = Serial.read();
      if (inByte == 't') LoadCell.tareNoDelay();
    }
    if (LoadCell.getTareStatus() == true) {
      Serial.println("Tare complete");
      _resume = true;
    }
  }

  ## Instructions for providing the known mass for calibration.
  Serial.println("Now, place a known mass on the load cell.");
  Serial.println("Then send the weight of this mass (e.g., 100.0) from the serial monitor.");

  float known_mass = 0;
  _resume = false;
  while (_resume == false) {
    LoadCell.update();
    if (Serial.available() > 0) {
      known_mass = Serial.parseFloat();
      if (known_mass != 0) {
        Serial.print("Known mass is: ");
        Serial.println(known_mass);
        _resume = true;
      }
    }
  }

  LoadCell.refreshDataSet();
  float newCalibrationValue = LoadCell.getNewCalibration(known_mass);

  ## Wait for user input before proceeding.
  _resume = false;
  while (_resume == false) {
    if (Serial.available() > 0) {
      char inByte = Serial.read();
      ## Add handling for user input if needed.
    }
  }

  Serial.println("End calibration");
  Serial.println("***");
  Serial.println("To re-calibrate, send 'r' from the serial monitor.");
  Serial.println("For manual edit of the calibration value, send 'c' from the serial monitor.");
  Serial.println("***");
}

## Function to change the saved calibration factor.
void changeSavedCalFactor() {
  float oldCalibrationValue = LoadCell.getCalFactor();
  boolean _resume = false;
  Serial.println("***");
  Serial.print("Current value is: ");
  Serial.println(oldCalibrationValue);
  Serial.println("Now, send the new value from the serial monitor (e.g., 696.0)");
  float newCalibrationValue;
  while (_resume == false) {
    if (Serial.available() > 0) {
      newCalibrationValue = Serial.parseFloat();
      if (newCalibrationValue != 0) {
        Serial.print("New calibration value is: ");
        Serial.println(newCalibrationValue);
        LoadCell.setCalFactor(newCalibrationValue);
        _resume = true;
      }
    }
  }
  ## Wait for user input before proceeding.
  _resume = false;
  Serial.println("End change calibration value");
  Serial.println("***");
}
