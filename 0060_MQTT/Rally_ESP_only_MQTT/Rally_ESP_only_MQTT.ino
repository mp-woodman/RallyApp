#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

#define PN532_SCK  (14)
#define PN532_MOSI (13)
#define PN532_SS   (15)
#define PN532_MISO (12)

Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);


boolean success;
  uint8_t apdubuffer[255] = {}, apdulen;
  uint8_t ppse[] = {0x8E, 0x52, 0x41, 0x4c, 0x4c, 0x59, 0x5f, 0x45, 0x53, 0x50}; // RALLY_ESP in ascii to thell the Tag reader, thas it is an Esp for this application
  
void setup(void) {
  Serial.begin(115200);
  Serial.println("Application: Emulatet NFC tag!");

  //--------------- setup NFC board with PN532 Chip ---------------
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  nfc.setPassiveActivationRetries(0xFF);
  
  // configure board to read RFID tags
  nfc.SAMConfig();

  //--------------- setup PN532 finished ---------------

  Serial.println("Init finished");
  delay(200);
}

void loop(void) {
  nfc.AsTarget();
  success = nfc.getDataTarget(apdubuffer, &apdulen); //Read initial APDU
  nfc.setDataTarget(ppse, sizeof(ppse));   // tell the reader that it is as Rally ESP
  nfc.getDataTarget(apdubuffer, &apdulen); // get the topic back
  
  if (apdulen>0){
    for (uint8_t i = 0; i < apdulen; i++){
      Serial.print(" 0x"); Serial.print(apdubuffer[i], HEX);
    }
    Serial.println("");
    
    Serial.println("get the topic from the nfc reader/weither. now go into infifity loop");
    //in this application only one nfc connection ist unsed. ==>
    while(true); //forever loop
  }
}
