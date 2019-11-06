#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <FastLED.h>
#include <gto.h>
#include <LightShow.h>
#include <BLEDevice.h>

// color gradients for lamp //

DEFINE_GRADIENT_PALETTE( quagga_gp ) {
    0,   1,  9, 84,
   40,  42, 24, 72,
   84,   6, 58,  2,
  168,  88,169, 24,
  211,  42, 24, 72,
  255,   1,  9, 84};

  DEFINE_GRADIENT_PALETTE( aquamarinemermaid_gp ) {
    0,  23,  4, 32,
   63,  98, 31, 52,
  127, 224,138, 24,
  191,   7, 55,164,
  255,  23,  4, 32};

DEFINE_GRADIENT_PALETTE( catfairy_gp ) {
    0,  74,124, 85,
   76, 152,128, 44,
  127, 144, 97, 96,
  178, 100, 72,102,
  232,  78, 90,122,
  255,  78, 90,122};

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
long previousMillis = 0;
int interval = 1000;

// WiFiServer server(80);
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
AsyncWebSocketClient * globalClient = NULL;
AsyncWebParameter* p;
CRGBPalette16 palette = PartyColors_p;
String msg = "";

// websocket event //
void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
 
  if(type == WS_EVT_CONNECT){
 
    Serial.println("Websocket client connection received");
    globalClient = client;
 
  } else if(type == WS_EVT_DISCONNECT){
 
    Serial.println("Websocket client connection finished");
    globalClient = NULL;
 
  } else if(type == WS_EVT_DATA){
        for (size_t i = 0; i < len; i++) {
          msg += (char) data[i];
        }
  }
}

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

    //setup websocket event//
    ws.onEvent(onWsEvent);
    server.addHandler(&ws);

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
        if (request->hasParam("fire_dep")) {
            digitalWrite(internal_led,LOW);
            effect=8;
            led_state = "fire_dep";
        }
        if (request->hasParam("police_dep")) {
            digitalWrite(internal_led,LOW);
            effect=9;
            led_state = "police_dep";
        }
        if (request->hasParam("maayan_rainbow")) {
            digitalWrite(internal_led,LOW);
            effect=10;
            led_state = "maayan_rainbow";
        }
        if (request->hasParam("cubes")) {
            digitalWrite(internal_led,LOW);
            effect=11;
            led_state = "cubes";
        }
        if (request->hasParam("squares")) {
            digitalWrite(internal_led,LOW);
            effect=12;
            led_state = "squares";
        }
        if (request->hasParam("squares")) {
            digitalWrite(internal_led,LOW);
            effect=12;
            led_state = "squares";
        }
        if (request->hasParam("wild")) {
            digitalWrite(internal_led,LOW);
            effect=13;
            led_state = "wild";
        }
        if (request->hasParam("palette")) {
            p = request->getParam(1);
            if (p->name() == "HeatColors_p"){
                palette = HeatColors_p;
            }
            else if (p->name() == "RainbowStripesColors_p"){
                palette = RainbowStripesColors_p;
            }
            else if (p->name() == "OceanColors_p"){
                palette = OceanColors_p;
            }
            else if (p->name() == "ForestColors_p"){
                palette = ForestColors_p;
            }
            else if (p->name() == "PartyColors_p"){
                palette = PartyColors_p;
            }
            else if (p->name() == "quagga_gp"){
                palette = quagga_gp;
            }
            else if (p->name() == "aquamarinemermaid_gp"){
                palette = aquamarinemermaid_gp;
            }
            else if (p->name() == "catfairy_gp"){
                palette = catfairy_gp;
            }
        }

        request->send(SPIFFS, "/lamp.html", String(), false, processor);
        //request->send(SPIFFS, "/test.html", "text/html");
    });

    // Route to load style.css file
    server.on("/style_new.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/style_new.css", "text/css");
    });

        // Route to load style.css file
    server.on("/manifest.json", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/manifest.json", "text/json");
    });

    server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/favicon.ico", "image/x-icon");
    });

    server.on("/officelight.png", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/officelight.png", "image/png");
    });

    server.on("/officelight128.png", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/officelight128.png", "image/png");
    });

    server.on("/officelight256.png", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/officelight256.png", "image/png");
    });

    server.on("/officelight512.png", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/officelight512.png", "image/png");
    });

    server.begin();

}

void loop() {
    ArduinoOTA.handle();
    unsigned long currentMillis = millis();
    // websocket update lamp status //
    if (currentMillis - previousMillis > interval) { // publish every *interval* miliseconds
        if(globalClient != NULL && globalClient->status() == WS_CONNECTED){
                if(mode==1){globalClient->text("on");}
                else{globalClient->text("off");}
            }
    }
    // turn lamp on/off by websocket command //
    if(msg=="on"){mode=1;}
    else if(msg=="off"){mode=0;}
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
                    juggle_lib(leds, NUM_LEDS, palette);
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
                    learning_p(leds, NUM_LEDS, palette);
                    FastLED.show();
                    break;
                case 7: //up_down
                    up_down(leds, NUM_LEDS, palette, gHue);
                    gHue = gHue+10;
                    // FastLED.show();
                    break;
                case 8: //fire_dep
                    fire_dep(leds, NUM_LEDS);
                    FastLED.show();
                    break;
                case 9: //police_dep
                    police_dep(leds, NUM_LEDS);
                    FastLED.show();
                    break;
                case 10: //maayan rainbow
                    maayan_rainbow(leds, NUM_LEDS);
                    FastLED.show();
                    break;
                case 11: //cubes
                    cubes(leds, NUM_LEDS, palette, gHue);
                    FastLED.show();
                    gHue = gHue+90;
                    break;
                case 12: //squares
                    squares(leds, NUM_LEDS, palette);
                    FastLED.show();
                    break;
                case 13: //squares
                    wild(leds, NUM_LEDS, palette);
                    FastLED.show();
                    break;
            } 

    }
    if(gHue==255){gHue=0;}
    
}