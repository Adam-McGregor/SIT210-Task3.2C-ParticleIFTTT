#include <Wire.h>

int BH1750address = 0x23; // i2c Address
byte buff[2];

bool state = false; // is light on

int led = D7;

void setup()
{
    Wire.begin();
    pinMode(led, OUTPUT);
    delay(2000);
}

void loop()
{
    uint16_t value=0;
    BH1750_Init(BH1750address);
    delay(200);
        
    if(2==BH1750_Read(BH1750address))
    {
        value=((buff[0]<<8)|buff[1])/1.2; // light level in LUX
        if (!state && value > 0) // if off and there is light, state has been switched
        {
            state = true;
            Particle.publish("Light", "ON");
            delay(1000); // Do not spam IFTTT
        }
        else if (state && value == 0) // if on and there is no light, state has been switched
        {
            state = false;
            Particle.publish("Light", "OFF");
            delay(1000); // Do not spam IFTTT
        }
        
    }
    delay(150);
}

int BH1750_Read(int address) 
{
    int i=0;
    Wire.beginTransmission(address);
    Wire.requestFrom(address, 2);
    while(Wire.available()) 
    {
        buff[i] = Wire.read();
        i++;
    }
    Wire.endTransmission();  
    return i;
}
 
void BH1750_Init(int address) 
{
    Wire.beginTransmission(address);
    Wire.write(0x10);
    Wire.endTransmission();
}