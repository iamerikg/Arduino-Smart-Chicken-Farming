## Define the value of the load resistor (RL) used in the circuit.
#define RL 47 

## Initialization code that runs once when the Arduino starts.
void setup() {
  ## Begin communication with the serial monitor at a baud rate of 9600.
  Serial.begin(9600); 
}

## Main loop that continuously performs gas sensor measurements and calculations.
void loop() {
  float analog_value;
  float VRL;
  float Rs;
  float Ro;

  ## Initialize analog_value variable.
  analog_value = 0; 

  ## Loop to collect analog readings and sum them up.
  for (int test_cycle = 1; test_cycle <= 500; test_cycle++) {
    analog_value = analog_value + analogRead(A5); 
  }

  ## Calculate the average of the collected analog readings.
  analog_value = analog_value / 500.0;

  ## Convert the analog reading to voltage (VRL).
  VRL = analog_value * (5.0 / 1023.0);

  ## Calculate sensor resistance (Rs) using the voltage divider formula.
  Rs = ((5.0 / VRL) - 1) * RL;

  ## Calculate the sensor resistance in clean air (Ro).
  Ro = Rs / 3.6;

  ## Print the calculated values to the serial monitor.
  Serial.print("VRL = ");
  Serial.println(VRL);
  Serial.print("Rs = ");
  Serial.println(Rs);
  Serial.print("Ro at fresh air = ");
  Serial.println(Ro);
  Serial.println("-------------------------------------------------------------------------------");

  ## Introduce a delay before the next loop iteration.
  delay(1000); 
}
