/*
  Board: adruino rp2040 connect
*/
// set pin 10 as the slave select for the digital pot:
#define slaveSelectPinA  9
#define slaveSelectPinB  8
#define mux_sel 6
#define relay_reg_OE 7

// inslude the SPI library:
#include <SPI.h>


String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
// Other global variables
#define textBuffSize 16 //length of longest command string plus two spaces for CR + LF
char textBuff[textBuffSize]; //someplace to put received text
int charsReceived = 0;
// 12-bit conversion, assume max value == ADC_VREF == 3.3 V
const float CONVERSION_FACTOR = 3.3f / (1 << 12);

void setup() {
  // set the slaveSelectPin as an output:
  pinMode(slaveSelectPinA, OUTPUT);
  pinMode(slaveSelectPinB, OUTPUT);
  pinMode(mux_sel, OUTPUT);
  digitalWrite(mux_sel, HIGH);
  pinMode(relay_reg_OE, OUTPUT);
  digitalWrite(relay_reg_OE, LOW);
  
  // initialize SPI:
  SPI.begin();
    // initialize serial:
  Serial.begin(9600);
    // reserve 200 bytes for the inputString:
  inputString.reserve(200);

  charsReceived = 0;
  adc_init();
  adc_set_temp_sensor_enabled (true);

    while (!Serial)
    ; // Serial is via USB; wait for enumeration
  Serial.print("starting  ");
}

void loop() {

//  for (int i = 0; i < 32; i++) {
//    SwitchAWrite(1<<i);
//    delay(300);
//  }

//  for (int i = 0; i < 32; i++) {
 //   SwitchBWrite(1<<i);
 //   delay(300);
 // }

  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
   // Serial.println(inChar);
    // add it to the inputString:
    textBuff[charsReceived] = inChar;
    charsReceived++;
    // if the incoming character is a newline, parse text
    // do something about it:
    if (inChar == '\n') {
      parseReceivedText();
      charsReceived = 0;
    }
  }
}






    

 

void SwitchAWrite(uint32_t value) {
  SPI.transfer((byte)((value>>24) & 0xFF)); 
  SPI.transfer((byte)((value>>16) & 0xFF));
  SPI.transfer((byte)((value>>8) & 0xFF));
  SPI.transfer((byte)(value & 0xFF));
  digitalWrite(slaveSelectPinA, HIGH);
  digitalWrite(slaveSelectPinA, LOW);
}

void SwitchBWrite(uint32_t value) {
  // take the SS pin low to select the chip:
  SPI.transfer((byte)((value>>24) & 0xFF)); 
  SPI.transfer((byte)((value>>16) & 0xFF));
  SPI.transfer((byte)((value>>8) & 0xFF));
  SPI.transfer((byte)(value & 0xFF));
  digitalWrite(slaveSelectPinB, HIGH);
  digitalWrite(slaveSelectPinB, LOW);
}






//-------------------------------------- commands decoding ---------------------------------
void parseReceivedText()
{


    
     // look at first character and decide what to do
  
  switch (textBuff[0]) {
    case 'v' : doAnalogCommand();        break;
    case 't' : doTemperatureCommand();   break;
    case 'd' : doDigitalCommand();       break;
    case '?' : printHelpMessage();       break;
    case 0x0d :                          break;  //ignore a carriage return
    default: printErrorMessage();        break;
  }
 }


void doDigitalCommand()
  // if we got here, textBuff[0] = 'd'
{
  switch (textBuff[1]) {
    case 'r' : readDigitalPins(); break;
    case 'w' : writeSPIA(); break;
    case 'u' : writeSPIB(); break;
    default: printErrorMessage(); break;
  }
}

//read muxes settings
void readDigitalPins()
  // if we got here, textBuff[0] = 'd' and textBuff[1] = 'r'
{
    
      Serial.print("not implemented yet ");

      Serial.print("\n");
    

}  


// write muxes

void writeSPIA()
  // if we got here, textBuff[0] = 'd' and textBuff[1] = 'w'
{
/*
     Serial.print(textBuff[2], HEX);
     Serial.print(" ");
     Serial.print(textBuff[3], HEX);
     Serial.print(" ");
     Serial.print(textBuff[4], HEX);
     Serial.print(" ");
     Serial.print(textBuff[5], HEX);
     Serial.print(" ");
     Serial.print("\n");
*/
  //int pin = -1;
  //int pinSetting = -1;
  if (textBuff[2] >= '0' && textBuff[2] <= 'F') 
     {
     if (textBuff[3] >= '0' && textBuff[3] <= 'F')
        {
        
     //   SwitchAWrite(1<<(parseDigit(textBuff[2]) *10+parseDigit(textBuff[3])));
     //   SwitchBWrite(1<<(parseDigit(textBuff[2]) *10+parseDigit(textBuff[3])));
    uint32_t val;
    val= h2d(textBuff[9]) | h2d(textBuff[8])<<4 | h2d(textBuff[7])<<8 | h2d(textBuff[6])<<12  | h2d(textBuff[5]) <<16 | h2d(textBuff[4])<<20 | h2d(textBuff[3])<<24 | h2d(textBuff[2])<<28 ;
    SwitchAWrite(val);
    Serial.print(val,HEX); 
        }
        else printErrorMessage();
     }
   else printErrorMessage();
}



void writeSPIB()
  // if we got here, textBuff[0] = 'd' and textBuff[1] = 'w'
{
/*
     Serial.print(textBuff[2], HEX);
     Serial.print(" ");
     Serial.print(textBuff[3], HEX);
     Serial.print(" ");
     Serial.print(textBuff[4], HEX);
     Serial.print(" ");
     Serial.print(textBuff[5], HEX);
     Serial.print(" ");
     Serial.print("\n");
*/

  if (textBuff[2] >= '0' && textBuff[2] <= 'F') 
     {
     if (textBuff[3] >= '0' && textBuff[3] <= 'F')
        {
         
        uint32_t val= h2d(textBuff[9]) | h2d(textBuff[8])<<4 | h2d(textBuff[7])<<8 | h2d(textBuff[6])<<12  | h2d(textBuff[5]) <<16 | h2d(textBuff[4])<<20 | h2d(textBuff[3])<<24 | h2d(textBuff[2])<<28 ;
        SwitchBWrite(val);
       // Serial.print(val,HEX); 
        }
        else printErrorMessage();
     }
   else printErrorMessage();
}




//read supply rails

void doAnalogCommand()
  // if we got here, textBuff[0] = 'v'
{
            Serial.print("12V rail: ");
            //float sensorReading = (analogRead(0)*9.375)/1000.0;
            float sensorReading = (analogRead(0)*36)/1000.0;
            Serial.print(sensorReading);
            Serial.print("\n\r");
            Serial.print("3.3V rail: ");
            sensorReading = (analogRead(1)*7.52)/1000.0;
            Serial.print(sensorReading);
            Serial.print("\n\r");
            Serial.print("3.3VMP rail: ");
            sensorReading = (analogRead(2)*7.62)/1000.0;
            Serial.print(sensorReading);
            Serial.print("\n\r");
}


void doTemperatureCommand()
{
    // Select ADC input 4 for internal temperature sensor
  adc_select_input(4);
  uint16_t adc = adc_read();
  float ADC_Voltage = float(adc) * CONVERSION_FACTOR;
  float T = 27 - (ADC_Voltage - 0.706)/0.001721;        // formula found on page 71 (section 4.1.1. hardware_adc) of the Raspberry Pi Pico C/C++ SDK documentation
  // The returned temperature is in degrees Celsius.
  Serial.print("CPU temperature: ");
  Serial.println("Temp: " + String(T, 2) + "C"); 
  Serial.print("\n\r");
}



unsigned char h2d(unsigned char hex)
{
        if(hex > 0x39) hex -= 7; // adjust for hex letters upper or lower case
        return(hex & 0xf);
         Serial.print(hex & 0xf,HEX);
}

void printErrorMessage()
{
  Serial.println("Unrecognized command.  ? for help.");
}



void printHelpMessage()
{
  Serial.println("\n Supported commands:\n");
  Serial.println("  dr         -digital read:   returns state of switches");
  Serial.println("  v          -3V and 5V rail value");
  Serial.println("  dwxxxxxxxx       -SPI A write,  xxxxxxxx hex)");
  Serial.println("  duxxxxxxxx       -SPI B write,  xxxxxxxx hex)");  
  Serial.println("  t          -approx CPU temperature ");
  Serial.println("  ?          -print this help message");
}


 

