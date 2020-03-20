#include <Wire.h>
#include <math.h>


//#define ADDRESS 0x76 // Address of Parallax - Pressure Sensor MS5607 REV A
#define ADDRESS 0x77 // Address of Far Horizons - Pressure Sensor MS5607 REV B

uint32_t D1 = 0;
uint32_t D2 = 0;
float dT = 0;
int32_t TEMP = 0;
float OFF = 0; 
float SENS = 0; 
float P = 0;
float T2  = 0;
float OFF2  = 0;
float SENS2 = 0;
uint16_t C[7];
//int count = 0;

float Temperature;
float Pressure;




void setup() {
  


// Disable internal pullups, 10Kohms are on the breakout
 PORTC |= (1 << 4);
 PORTC |= (1 << 5);

  Wire.begin();
  Serial.begin(57600); //9600 changed 'cos of timing?
  delay(1000); //Give the open log a second to get in gear. 
  initial(ADDRESS);
}

void loop()
{
//count = count + 1;
 
/*******************************************************************************/
/**************                PRESSURE                   **********************/
/*******************************************************************************/
  
  D1 = getVal(ADDRESS, 0x48); // Pressure raw
  D2 = getVal(ADDRESS, 0x58);// Temperature raw

  dT   = (float)D2 - ((uint32_t)C[5] * 256);
  OFF  = ((float)C[2] * 131072) + ((dT * C[4]) / 64);
  SENS = ((float)C[1] * 65536) + (dT * C[3] / 128);

  TEMP = (int64_t)dT * (int64_t)C[6] / 8388608 + 2000;
  
 if(TEMP < 2000) // if temperature lower than 20 Celsius 
  {
    
    T2=pow(dT,2)/2147483648;
    OFF2=61*pow((TEMP-2000),2)/16;
    SENS2=2*pow((TEMP-2000),2);
    
   if(TEMP < -1500) // if temperature lower than -15 Celsius 
    {
     OFF2=OFF2+15*pow((TEMP+1500),2);
     SENS2=SENS2+8*pow((TEMP+1500),2);
    }

    TEMP = TEMP - T2;
    OFF = OFF - OFF2; 
    SENS = SENS - SENS2;   
  }
  
  Temperature = (float)TEMP / 100;  
  P  = (D1 * SENS / 2097152 - OFF) / 32768;
  Pressure = (float)P / 100;
  
//  Serial.print("$FHDAT");
//  Serial.print(",");
//  Serial.print(count);
  Serial.print("\t");
  Serial.print(Temperature);
  Serial.print("\t");
  Serial.println(Pressure);
  
  /* RESET THE CORECTION FACTORS */
  
    T2 = 0;
    OFF2 = 0;
    SENS2 = 0;

  delay(100);
} /* END OF ARDUINO LOOP */

long getVal(int address, byte code)
{
  unsigned long ret = 0;
  Wire.beginTransmission(address);
  Wire.write(code);
  Wire.endTransmission();
  delay(10);
  // start read sequence
  Wire.beginTransmission(address);
  Wire.write((byte) 0x00);
  Wire.endTransmission();
  Wire.beginTransmission(address);
  Wire.requestFrom(address, (int)3);
  if (Wire.available() >= 3)
  {
    ret = Wire.read() * (unsigned long)65536 + Wire.read() * (unsigned long)256 + Wire.read();
  }
  else {
    ret = -1;
  }
  Wire.endTransmission();
  return ret;
}

void initial(uint8_t address)
{
  //Serial.println();
  //Serial.println("PRESSURE SENSOR PROM COEFFICIENTS");

  Wire.beginTransmission(address);
  Wire.write(0x1E); // reset
  Wire.endTransmission();
  delay(10);


  for (int i=0; i<6  ; i++) {

    Wire.beginTransmission(address);
    Wire.write(0xA2 + (i * 2));
    Wire.endTransmission();

    Wire.beginTransmission(address);
    Wire.requestFrom(address, (uint8_t) 6);
    delay(1);
    if(Wire.available())
    {
       C[i+1] = Wire.read() << 8 | Wire.read();
    }
    else {
      Serial.println("Error reading PROM 1"); // error reading the PROM or communicating with the device
    }
    //Serial.println(C[i+1]);   // Prints out the coefficients.
  }
  //Serial.println();
}
