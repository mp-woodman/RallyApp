#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

#define DEBUG true

#define PN532_SS   (5)                                             
Adafruit_PN532 nfc(PN532_SS);

uint8_t id = 0;
String topicPrefix;
uint8_t target_ppse[] = {0x8E, 0x52, 0x41, 0x4c, 0x4c, 0x59, 0x5f, 0x45, 0x53, 0x50}; // RALLY_ESP in ascii to thell the Tag reader, thas it is an Esp for this application

void setup() 
{
  Serial.begin(115200);
  !DEBUG?:Serial.println("Application: to wait for an ESP tag ang give it an uniq toppic in this Rally APP");

  //--------------- setup NFC board with PN532 Chip ---------------
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    ("Didn't find PN53x board");
    while (1); // halt
  }
  !DEBUG?:Serial.print("Found chip PN5"); 
  !DEBUG?:Serial.println((versiondata>>24) & 0xFF, HEX); 
  !DEBUG?:Serial.print("Firmware ver. "); 
  !DEBUG?:Serial.print((versiondata>>16) & 0xFF, DEC); 
  !DEBUG?:Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  // configure board to read RFID tags
  nfc.SAMConfig();
  //--------------- setup PN532 finished ---------------
  
  //--------------- Topic Prefix Via USB/UART ---------------
  !DEBUG?:Serial.println("Wait for topic prefix input.. ");
  while(!Serial.available());
  topicPrefix = Serial.readString();
  !DEBUG?:Serial.println("recived the Toppic Prefix: ");
  !DEBUG?:Serial.println(topicPrefix);
      
  !DEBUG?:Serial.println("Init finished");
  delay(200);
}

void loop() {
  uint8_t success;
  !DEBUG?:Serial.println("Waiting for Rally ESP");
  
  // set shield to inListPassiveTarget
  success = nfc.inListPassiveTarget();
  if(success) {
    //get next topic
    String topic = topicPrefix + "_" + id;
    !DEBUG?:Serial.print("currentToppic:");
    !DEBUG?:Serial.println(topic);
    uint8_t topicLen = topic.length();
    !DEBUG?:Serial.print("topic len: ");
    !DEBUG?:Serial.println(topicLen);
    //convert in asci array
    uint8_t apdu[topicLen];
    for(int i = 0; i<topicLen;i++)
    {
      apdu[i]= topic.charAt(i);
      !DEBUG?:Serial.println(apdu[i],HEX);
    }
    uint8_t apduLength=sizeof(apdu);

    //reset array preperation
    uint8_t response[255];
    memset(response, 0, sizeof(response));
    uint8_t responseLength=sizeof(response);
    
    success = nfc.inDataExchange(apdu, apduLength, response, &responseLength);
    if (success)
    {
      !DEBUG?:Serial.print("get response");
      nfc.PrintHexChar(response, responseLength);
      for(int i = 0; i< sizeof(target_ppse);i++)
      {
        if(target_ppse[i] != target_ppse[i])
        {
          success = 1;
          !DEBUG?:Serial.print("at: ");
          !DEBUG?:Serial.print(i);
          !DEBUG?:Serial.print(" targ: ");
          !DEBUG?:Serial.print(target_ppse[i]);
          !DEBUG?:Serial.print(" resp: ");
          !DEBUG?:Serial.print(target_ppse[i]);
        }
      }
      if(success)
      {
        !DEBUG?:Serial.println("ESP device sucessvuly detected. And Topic name transimittet");
        Serial.println("created Topic "+topic);
        id++;
      }
      delay(10000);
    }
  }
}
