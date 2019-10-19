#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <FastLED.h>
#include <gto.h>
#include <LightShow.h>
#include <BLEDevice.h>

// The amount of LEDs in the setup
#define NUM_LEDS 60
#define LED_TYPE WS2812B
//The pin that controls the LEDs
#define LED_PIN 12

// constsants //
#define MILLI_AMPERE	500
const char* SSID = "winstonia";
const char* PASS = "Polycom1";
const int internal_led = 2;

// WiFiServer server(80);
AsyncWebServer server(80);

// BLEclient

// static BLEUUID serviceUUID("6a4e2401-667b-11e3-949a-0800200c9a66");//("6a4e3e10-667b-11e3-949a-0800200c9a66"); //Service UUID 
// static BLEUUID    charUUID("6a4e4c80-667b-11e3-949a-0800200c9a66"); //Characteristic  UUID
// String My_BLE_Address =  "ff:a9:72:2a:ee:21";// "FF:A9:72:2A:EE:21"; // 
// static BLERemoteCharacteristic* pRemoteCharacteristic;
// BLEScan* pBLEScan; //Name the scanning device as pBLEScan
// BLEScanResults foundDevices;
// static BLEAddress *Server_BLE_Address;
// String Scaned_BLE_Address;
// boolean paired = false; //boolean variable to toggLe light

// ble connect to server //

// bool connectToserver (BLEAddress pAddress){
    
//     BLEClient*  pClient  = BLEDevice::createClient();
//     Serial.println(" - Created client");

//     // Connect to the BLE Server.
//     pClient->connect(pAddress);
//     Serial.println(" - Connected to fitnessband");

//     // Obtain a reference to the service we are after in the remote BLE server.
//     BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
//     if (pRemoteService != nullptr)
//     {
//       Serial.println(" - Found our service");
//       return true;
//     }
//     else
//     return false;

//     // Obtain a reference to the characteristic in the service of the remote BLE server.
//     pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
//     if (pRemoteCharacteristic != nullptr)
//       Serial.println(" - Found our characteristic");

//       return true;
// }

// creating class for advertising devices //

// class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks 
// {
//     void onResult(BLEAdvertisedDevice advertisedDevice) {
//       Serial.printf("Scan Result: %s \n", advertisedDevice.toString().c_str());
//       Server_BLE_Address = new BLEAddress(advertisedDevice.getAddress());
      
//       Scaned_BLE_Address = Server_BLE_Address->toString().c_str();
      
//     }
// };

// get state request from website content //

String led_state = "off";
String ledState;

// led variables
CRGB leds[NUM_LEDS];
uint8_t gHue = 0;
bool light_switch = false;
int mode = 0;
int effect = 0;

struct color {
	int r = 200;
	int g = 200;
	int b = 200;
};

struct color Color;

String processor(const String& var){
    Serial.println(var);
    if(var == "STATE"){
        //ledState = "on";
        return led_state;
    return String();
    }
}

// ArduinoOTA setup //

void SetupOTA() {
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}


void setup() {
    Serial.begin(115200);
    pinMode(internal_led,OUTPUT);
    digitalWrite(internal_led,LOW);

    // importent maximum power for project!!!
	  FastLED.setMaxPowerInVoltsAndMilliamps(5,MILLI_AMPERE);

    //start spiffs filesystem
    if(!SPIFFS.begin()){
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }
    
    Serial.print("connecting to: ");
    Serial.println(SSID);
    WiFi.begin(SSID, PASS);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Trying to connect!");
    }
    SetupOTA();
    Serial.println("");
    Serial.println("Managed to Connect");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

// initialize BLE scan //

    // BLEDevice::init("");
    // pBLEScan = BLEDevice::getScan(); //create new scan
    // pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks()); //Call the class that is defined above 
    // pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster

    // initiate LED array and show setup light

    FastLED.addLeds<LED_TYPE, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    fill_solid(leds, NUM_LEDS, CRGB(0,0,100));
    FastLED.show();
    delay(1000);
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();

    // Route to load site index file
    server.on("/html", HTTP_GET, [](AsyncWebServerRequest *request){
        if (request->hasParam("led_on")) {
            digitalWrite(internal_led,HIGH);
            // fill_solid(leds, NUM_LEDS, CRGB(0,0,100));
            // FastLED.show();
            mode = 1;
            led_state = "on";
        };
        if (request->hasParam("led_off")) {
            digitalWrite(internal_led,LOW);
            // fill_solid(leds, NUM_LEDS, CRGB::Black);
            // FastLED.show();
            mode = 0;
            led_state = "off";
        }
        if (request->hasParam("light")) {
            digitalWrite(internal_led,LOW);
            effect=0;
            led_state = "Light";
        }
        if (request->hasParam("jiggle")) {
            digitalWrite(internal_led,LOW);
            effect=1;
            led_state = "Jiggle";
        }
        if (request->hasParam("glitter")) {
            digitalWrite(internal_led,LOW);
            effect=2;
            led_state = "Rainbow Glitter";
        }
        if (request->hasParam("march")) {
            digitalWrite(internal_led,LOW);
            effect=3;
            led_state = "Rainbow March";
        }
        if (request->hasParam("noise_16")) {
            digitalWrite(internal_led,LOW);
            effect=4;
            led_state = "Noise_16";
        }
        if (request->hasParam("blendwave")) {
            digitalWrite(internal_led,LOW);
            effect=5;
            led_state = "Blendwave";
        }
        if (request->hasParam("learning_p")) {
            digitalWrite(internal_led,LOW);
            effect=6;
            led_state = "Learning";
        }
        if (request->hasParam("up_down")) {
            digitalWrite(internal_led,LOW);
            effect=7;
            led_state = "Up_down";
        }
        if (request->hasParam("learning_p2")) {
            digitalWrite(internal_led,LOW);
            effect=8;
            led_state = "learning_p2";
        }

        request->send(SPIFFS, "/test2.html", String(), false, processor);
        //request->send(SPIFFS, "/test.html", "text/html");
    });

    // Route to load style.css file
    server.on("/style2.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/style2.css", "text/css");
    });

    server.begin();

}

void loop() {
    ArduinoOTA.handle();

    // // search for BLE devices every 3 seconds //
    // foundDevices = pBLEScan->start(3); //Scan for 3 seconds to find the Fitness band 

    // handle found devices //

  //   while (foundDevices.getCount() >= 1)
  // {
  //   if (Scaned_BLE_Address == My_BLE_Address && paired == false)
  //   {
  //     Serial.println("Found Device :-)... connecting to Server as client");
  //      if (connectToserver(*Server_BLE_Address))
  //     {
  //       paired = true;
  //       Serial.println("********************LED turned ON************************");
  //       digitalWrite (internal_led,HIGH);
  //       break;
  //     }
  //     else
  //     {
  //     Serial.println("Pairing failed");
  //     break;
  //     }
  //   }
    
  //   if (Scaned_BLE_Address == My_BLE_Address && paired == true)
  //   {
  //     Serial.println("Our device went out of range");
  //     paired = false;
  //     Serial.println("********************LED OOOFFFFF************************");
  //     digitalWrite (internal_led,LOW);
  //   //   ESP.restart();
  //     break;
  //   }
  //   else
  //   {
  //   Serial.println("We have some other BLe device in range");
  //   break;
  //   }
  // } 

    switch(mode) {
        case 0:
            fill_solid(leds, NUM_LEDS, CRGB::Black);
            FastLED.show();
            break;
            default:
            break;
        case 1:
            switch(effect) {
                case 0: // color fill
                    fill_solid(leds, NUM_LEDS, CRGB(Color.r,Color.g,Color.b));
                    FastLED.show();
                    break;
                    default:
                    break;
                case 1: // juggle
                    juggle_lib(leds, NUM_LEDS);
                    FastLED.show();
                    break;
                case 2: // rainbow glitter
                    rainbowWithGlitter_lib(leds, NUM_LEDS, gHue);
                    FastLED.show();
                    break;
                case 3: //rainbow march
                    rainbow_march(leds, NUM_LEDS, 200, 10);
                    FastLED.show();
                    break;
                case 4: //noise_16
                    noise16_2(leds, NUM_LEDS);
                    FastLED.show();
                    break;
                case 5: //blendwave
                    blendwave(leds, NUM_LEDS);
                    FastLED.show();
                    break;
                case 6: //learning
                    learning_p(leds, NUM_LEDS);
                    FastLED.show();
                    break;
                case 7: //up_down
                    up_down(leds, NUM_LEDS);
                    // FastLED.show();
                    break;
                case 8: //learning_p2
                    learning_p2(leds, NUM_LEDS);
                    FastLED.show();
                    break;
            } 

    }
}