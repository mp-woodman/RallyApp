#include <Wire.h>
#include <SPI.h>

#include <WiFi.h>
#include <PubSubClient.h>

#include <Adafruit_PN532.h>

#include <Time.h>

#include <GxEPD.h>
#include <GxGDEH029A1/GxGDEH029A1.h>      // 2.9" b/w
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>

//--------------- pin defines ---------------
#define PN532_SCK    (14)
#define PN532_MOSI   (13)
#define PN532_SS     (15)
#define PN532_MISO   (12)

#define E_PAPER_SS   (5)
#define E_PAPER_DC   (17)
#define E_PAPER_RST  (16)
#define E_PAPER_BUSY (4)

#define BEEP (27)

//--------------- global ---------------
//statemachine
enum states
{
  NFC_INIT,
  NFC_REGISTRATION,
  WIFI_CONNECTED,
  WIFI_DISCONNECTED
}state;

//wifi settings
const char* ssid = "rallynet";
const char* password = "raspberry";
long lastReconnectAttempt = 0;
long lastSignalQualityPublish = 0;
WiFiClient wiClient;
  
//mqtt settings
const char* mqtt_server = "192.168.5.1";
PubSubClient client(wiClient);
char topicChar[16];
char wifiTopic[64];
char measurementNrTopic[64];
char differenceTopic[64];
char driveThroughtopic[64];
char beepTopic[64];
const char* willWifiMessage = "-1000";

//Rally Values
String topic = "not registered";
String topicPrefix ="rally/cars/";
//NFC
Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);
uint8_t apdubuffer[255] = {}, apdulen;
uint8_t ppse[] = {0x8E, 0x52, 0x41, 0x4c, 0x4c, 0x59, 0x5f, 0x45, 0x53, 0x50}; // RALLY_ESP in ascii to thell the Tag reader, thas it is an Esp for this application

//E-paper
GxIO_Class io(SPI, E_PAPER_SS, E_PAPER_DC, E_PAPER_RST);
GxEPD_Class display(io, E_PAPER_RST, E_PAPER_BUSY);
bool newDataAvailable= false;
int currentMeasurementNumber=0;
int currentDifference = 0;
struct microTime
{
  time_t t;
  int ms;
}currentDriveThroughT;
long lastEpaperUpdate = 0;

void setup(void) {
  Serial.begin(115200);
  Serial.println("Application: Rally ESP");
  //--------------- setup pins ---------------
  pinMode(BEEP, OUTPUT);
  //--------------- setup E-Paper ---------------
  display.init();
  display.eraseDisplay();
  showSetupScreen(&FreeMonoBold9pt7b);
  delay(1000);
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
  
  nfc.setPassiveActivationRetries(0xFF);
  nfc.SAMConfig();

  state = NFC_INIT;

  //--------------- setup wifi and MQTT ---------------
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(mqttTopicChangedCallback);
  
  Serial.println("Init finished");
  delay(200);
}

void loop(void) {
  //statemachine
  //Serial.print("State: ");
  //Serial.println(state);
  switch(state)
  {
    case NFC_INIT :
    {
      showSetupNFCScreen(&FreeMonoBold9pt7b);
      nfc.AsTarget();
      nfc.getDataTarget(apdubuffer, &apdulen); //Read initial APDU
      nfc.setDataTarget(ppse, sizeof(ppse));   // tell the reader that it is as Rally ESP
      nfc.getDataTarget(apdubuffer, &apdulen); // get the topic back
      state = NFC_REGISTRATION;
      break;
    }
    case NFC_REGISTRATION:
    {
      nfc.AsTarget();
      nfc.getDataTarget(apdubuffer, &apdulen); //Read initial APDU
      nfc.setDataTarget(ppse, sizeof(ppse));   // tell the reader that it is as Rally ESP
      nfc.getDataTarget(apdubuffer, &apdulen); // get the topic back
      if (apdulen>0){
        topic = "";
        for (uint8_t i = 0; i < apdulen; i++){
          Serial.print(" 0x"); Serial.print(apdubuffer[i], HEX);
          topic += char(apdubuffer[i]);
        }
        Serial.println("");
        
        Serial.print("get the topic: ");
        Serial.print(topic);
        Serial.println(" from the Rally Master");
        topic.toCharArray(topicChar,16);
        (topicPrefix+topic+"/wifi_signal_quality").toCharArray(wifiTopic,64);
        (topicPrefix+topic+"/measurement_number").toCharArray(measurementNrTopic,64);
        (topicPrefix+topic+"/difference").toCharArray(differenceTopic,64);
        (topicPrefix+topic+"/drive_through_timestamp").toCharArray(driveThroughtopic,64);
        (topicPrefix+topic+"/beep").toCharArray(beepTopic,64);

  
        showDefaultScreen(&FreeMonoBold12pt7b, topic);
        state = WIFI_CONNECTED;
      }
      break;
    }
    case WIFI_CONNECTED:
    {
      if (!client.connected()) {
        state = WIFI_DISCONNECTED;
      } else {
        // Client connected

        //publish the signal quality every second
        long now = millis();
        if (now - lastSignalQualityPublish > 1000) {
          lastSignalQualityPublish = now;
          char rssi[8];
          sprintf(rssi, "%ld", WiFi.RSSI());
          char wifitopic[64];
          (topicPrefix+topic+"/wifi_signal_quality").toCharArray(wifitopic,64);
          client.publish(wifiTopic, rssi);
          Serial.print("MQTT: signal quality published");
          Serial.println(rssi);
        }
        //update display if new content is available
        if (now - lastEpaperUpdate > 1000) {
          lastEpaperUpdate = now;
          if (newDataAvailable)
          {
            showMeasurementScreen(&FreeMonoBold12pt7b, topic, currentMeasurementNumber, currentDifference, currentDriveThroughT);
            newDataAvailable = false;
          }
        }
        client.loop();
      }
      break;
    }
    case WIFI_DISCONNECTED :
    {
      long now = millis();
      if (now - lastReconnectAttempt > 1000) {
        lastReconnectAttempt = now;
        // Attempt to reconnect
        if (reconnect()) {
          lastReconnectAttempt = 0;
          state = WIFI_CONNECTED;
        }
      }
      break;
    }
    default:
    Serial.print("Error unknown state in statemachine: ");
    Serial.println(state);
  }
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  client.setKeepAlive(5);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void mqttTopicChangedCallback(char* arrtopic, byte* msg, unsigned int length) {
  Serial.print("MQTT: Message arrived on topic: ");
  String arivedToppic = arrtopic;
  Serial.print(arivedToppic);
  
  char message[32];
  memset(message, 0, sizeof(message));
  for (int i=0;i<length;i++) {
    message[i]=(char)msg[i];
  }
  Serial.print(" msg: ");
  Serial.println(message);

  if (arivedToppic == beepTopic)
  {
    digitalWrite(BEEP,HIGH);
    delay(250);
    digitalWrite(BEEP,LOW);
    Serial.println("Beeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeep!");
  }
  else if(arivedToppic == measurementNrTopic)
  {
    newDataAvailable = true;
    sscanf(message, "%d", &currentMeasurementNumber);
    Serial.println(currentMeasurementNumber);
  }
  else if(arivedToppic == differenceTopic)
  {
    newDataAvailable = true;
    sscanf(message, "%d", &currentDifference);
    Serial.println(currentDifference);
  }
  else if(arivedToppic == driveThroughtopic)
  {
    newDataAvailable = true;
    uint64_t driveThroughTime;
    sscanf(message, "%llu", &driveThroughTime);
    Serial.println(driveThroughTime);
    currentDriveThroughT.t = driveThroughTime/1000.;
    currentDriveThroughT.ms = driveThroughTime-((driveThroughTime/1000)*1000);

  }
  else
  {
    Serial.println("not handeld Topic");
  }
}

boolean reconnect() {

  if (client.connect(topicChar, NULL, NULL, wifiTopic, 2,true, willWifiMessage,true)) { //todo last will
    // list of cubscribed toppics
    client.subscribe(measurementNrTopic);
    client.subscribe(differenceTopic);
    client.subscribe(driveThroughtopic);
    client.subscribe(beepTopic);
  }
  return client.connected();
}

void showSetupScreen(const GFXfont* f)
{
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(f);
  display.setCursor(0, 0);
  display.println();
  display.print("Rally ESP");
  display.println();
  display.println("startet...");
  display.update();
}

void showSetupNFCScreen(const GFXfont* f)
{
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(f);
  display.setCursor(0, 0);
  display.println();
  display.print("Rally ESP");
  display.println();
  display.println("Registrierung via NFC bei Rally Master ausstehend ...");
  display.update();
}

void showDefaultScreen(const GFXfont* f, String car)
{
  display.eraseDisplay();
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(f);
  display.setCursor(0, 0);
  display.println();
  display.println(car);
  display.println();
  display.println("Messung:");
  display.println("---");
  display.println();
  display.println("Dif:");
  display.println("---");
  display.println();
  display.println("um:");
  display.println("---"); 
  display.update();
}

void showMeasurementScreen(const GFXfont* f, String car, int measurementNbr, int dif, microTime drivethroughTime)
{
  display.eraseDisplay();
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(f);
  display.setCursor(0, 0);
  display.println();
  display.println(car);
  display.println();
  display.println("Messung:");
  display.println(measurementNbr);
  display.println();
  display.println("Dif:");
  display.print(dif/1000.0);
  display.print(" s");
  display.println();
  display.println("um:");
  char time_char[20];
  strftime(time_char, 20, "%H:%M:%S",  localtime(&drivethroughTime.t));
  display.print(time_char);
  display.print(",");
  if(drivethroughTime.ms < 100) display.print("0");
  display.println(drivethroughTime.ms); 
  display.update();
}
