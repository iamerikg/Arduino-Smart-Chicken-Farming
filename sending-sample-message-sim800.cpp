#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); 
void setup()
{
  Serial.begin(9600); 
  mySerial.begin(9600);

  Serial.println("Initializing..."); 
  delay(1000);

  mySerial.println("AT"); 
  updateSerial();

  mySerial.println("AT+CMGF=1"); 
  updateSerial();

  ## Define the country code based on your location.
  String countryCode = "+63";  // Replace with your country code
  
  mySerial.print("AT+CMGS=\"");
  mySerial.print(countryCode);
  mySerial.print("XXXXXXXXXX\"");  // Replace XXXXXXXXXX with the recipient's phone number
  mySerial.println();
  updateSerial();

  mySerial.print("This is a sample message only."); 
  updateSerial();
  mySerial.write(26);
}

void loop()
{
}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());
  }
}
