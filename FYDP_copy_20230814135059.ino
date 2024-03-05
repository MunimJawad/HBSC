#define AO_PIN A0  // Arduino's pin connected to AO pin of the MQ2 sensor

int light;

//sound Sensor
 const int sampleWindow = 50;                              // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
 
#define SENSOR_PIN A0

//temperature
#include <SimpleDHT.h>

int pinDHT11=2;
SimpleDHT11 dht11;

//water
int sensorPin = A0;

void setup() {
  // put your setup code here, to run once:
  //sound sensor 
  pinMode (SENSOR_PIN, INPUT); // Set the signal pin as input  

  Serial.begin(9600);
  Serial.println("Warming up the MQ2 sensor");
  delay(2000);  // wait for the MQ2 to warm up

}

void loop() {
  // put your main code here, to run repeatedly:
//Gas sensor
int gasValue = analogRead(AO_PIN);
  
  Serial.print("MQ2 sensor AO value: ");
  Serial.println(gasValue);
  delay(2000);

  //Light
  light = analogRead(A0);
 Serial.print("Light Intensity: ");
  Serial.print(light);
   Serial.print(" LUX \n");
  
  delay(1000);

  //sound
  
   unsigned long startMillis= millis();                   // Start of sample window
   float peakToPeak = 0;                                  // peak-to-peak level
 
   unsigned int signalMax = 0;                            //minimum value
   unsigned int signalMin = 1024;                         //maximum value
 
                                                          // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(SENSOR_PIN);                    //get reading from microphone
      if (sample < 1024)                                  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;                           // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;                           // save just the min levels
         }
      }
   }
 
   peakToPeak = signalMax - signalMin;                    // max - min = peak-peak amplitude
   int db = map(peakToPeak,20,900,49.5,90);             //calibrate for deciBels
 
  Serial.print("Loudness: ");
  Serial.print(db);
  Serial.println("dB");
   
  delay(2000); 

  //temperature

   byte temp=0;
  byte hum=0;
  int err=SimpleDHTErrSuccess;
  if((err=dht11.read(pinDHT11, &temp , &hum, NULL)!=SimpleDHTErrSuccess)){
    Serial.print("Read DHT11 failed, err=");Serial.print(err);delay(1000);
    return;
  }
  Serial.print("Temperature:");
  Serial.print((int)temp);Serial.print("°C\n");
   Serial.print("Humidity:");
  Serial.print((int)hum);Serial.print("H\n");
  delay(2000);

  //water

   int sensorValue = analogRead(sensorPin);
  int turbidity = map(sensorValue, 0,640, 100, 0);
   delay(1000);
   Serial.print(turbidity);
    if (turbidity < 20) {
   
    Serial.print(" its CLEAR\n ");
  }

   if ((turbidity > 20) && (turbidity < 50)) {
   
    Serial.print(" its CLOUDY \n");
  }

   if (turbidity > 50) {
    
    Serial.print(" its DIRTY \n");
  }

Serial.print("...........................\n");
}
