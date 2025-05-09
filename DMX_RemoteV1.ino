#include "Button2.h"
#include "SSD1306Wire.h"
#include "OLEDDisplayUi.h"
#include <AsyncDelay.h>

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

#define TRIGGER_PIN 0

// OTA Includes
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>

#define WiFi_Logo_width 60
#define WiFi_Logo_height 36
const uint8_t WiFi_Logo_bits[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x07, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xE0, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFF,
  0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0xFF, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
  0xFF, 0x03, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
  0x00, 0xFF, 0xFF, 0xFF, 0x07, 0xC0, 0x83, 0x01, 0x80, 0xFF, 0xFF, 0xFF,
  0x01, 0x00, 0x07, 0x00, 0xC0, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x0C, 0x00,
  0xC0, 0xFF, 0xFF, 0x7C, 0x00, 0x60, 0x0C, 0x00, 0xC0, 0x31, 0x46, 0x7C,
  0xFC, 0x77, 0x08, 0x00, 0xE0, 0x23, 0xC6, 0x3C, 0xFC, 0x67, 0x18, 0x00,
  0xE0, 0x23, 0xE4, 0x3F, 0x1C, 0x00, 0x18, 0x00, 0xE0, 0x23, 0x60, 0x3C,
  0x1C, 0x70, 0x18, 0x00, 0xE0, 0x03, 0x60, 0x3C, 0x1C, 0x70, 0x18, 0x00,
  0xE0, 0x07, 0x60, 0x3C, 0xFC, 0x73, 0x18, 0x00, 0xE0, 0x87, 0x70, 0x3C,
  0xFC, 0x73, 0x18, 0x00, 0xE0, 0x87, 0x70, 0x3C, 0x1C, 0x70, 0x18, 0x00,
  0xE0, 0x87, 0x70, 0x3C, 0x1C, 0x70, 0x18, 0x00, 0xE0, 0x8F, 0x71, 0x3C,
  0x1C, 0x70, 0x18, 0x00, 0xC0, 0xFF, 0xFF, 0x3F, 0x00, 0x00, 0x08, 0x00,
  0xC0, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0x0C, 0x00, 0x80, 0xFF, 0xFF, 0x1F,
  0x00, 0x00, 0x06, 0x00, 0x80, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x07, 0x00,
  0x00, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0xF8, 0xFF, 0xFF,
  0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x01, 0x00, 0x00,
  0x00, 0x00, 0xFC, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFF,
  0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0x1F, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x80, 0xFF, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  };




// Please include ArtnetWiFi.h to use Artnet on the platform
// which can use both WiFi and Ethernet
#include <ArtnetWiFi.h>
// this is also valid for other platforms which can use only WiFi
// #include <Artnet.h>

// WiFi stuff
// wifimanager can run in a blocking mode or a non blocking mode
// Be sure to know how to process loops with no delay() if using non blocking
bool wm_nonblocking = false; // change to true to use non blocking

WiFiManager wm; // global wm instance
WiFiManagerParameter custom_field; // global param ( for non blocking w params )

const char* ssid = "honeypot_2.4ghz";
const char* pwd = "fun3wlanVDSLmd";
const IPAddress ip(192, 168, 111, 201);
const IPAddress gateway(192, 168, 111, 254);
const IPAddress subnet(255, 255, 255, 0);

ArtnetWiFi artnet;
const String target_ip = "192.168.111.80";
uint8_t universe = 1;  // 0 - 15

const uint16_t size = 512;
uint8_t data[size];
uint8_t value = 0;

//OLED
SSD1306Wire  display(0x3c, SDA, SCL);
OLEDDisplayUi ui     ( &display );

/*
D3 = configportal
D2 = SDA
D1 = SCL
D5
D6 = 12
RX = 3
TX = 1
CLK = 6
SD0 = 

*/


//Buttons
#define BUTTON_PIN_1 2 
#define BUTTON_PIN_2 14
#define BUTTON_PIN_3 13
#define BUTTON_PIN_4 12 //12
#define BUTTON_PIN_5 9


Button2 button_1;
Button2 button_2;
Button2 button_3;
Button2 button_4;
Button2 button_5;

AsyncDelay delay_10s;

char *btn_names[]={"none", "blue", "white", "green"};
int btn_last=0;

struct ButtonData
    {
        uint8_t b1;
        uint8_t b2;
        uint8_t b3;
        uint8_t b4;
        uint8_t b5;
        uint8_t filler[507];
    };

void handleTap(Button2& btn) {
    display.displayOn();
    // check for really long clicks
    ButtonData r;
    if (btn == button_1) {
      Serial.println("1 clicked");
      data[0] = 255;
    } else if (btn == button_2) {
      Serial.println("2 clicked");
      btn_last=2;
      data[1] = 255;
    } else if (btn == button_3) {
      Serial.println("3 clicked");
      btn_last=3;
      data[2] = 255;
    } else if (btn == button_4) {
      Serial.println("4 clicked");
      data[3] = 255;
    } else if (btn == button_5) {
      Serial.println("5 clicked");
      data[4] = 255;
    }

    
    artnet.setArtDmxData(data, size);
    artnet.streamArtDmxTo(target_ip, universe);
    delay_10s.restart();
}

void handleRelease(Button2& btn) {
    display.displayOn();
    // check for really long clicks
    ButtonData r;
    if (btn == button_1) {
      Serial.println("1 released");
      data[0] = 0;
    } else if (btn == button_2) {
      Serial.println("2 released");
      btn_last=2;
      data[1] = 0;
    } else if (btn == button_3) {
      Serial.println("3 released");
      btn_last=3;
      data[2] = 0;
    } else if (btn == button_4) {
      Serial.println("4 released");
      data[3] = 0;
    } else if (btn == button_5) {
      Serial.println("5 released");
      data[4] = 0;
    }    
    artnet.setArtDmxData(data, size);
    artnet.streamArtDmxTo(target_ip, universe);
    delay_10s.restart();
}

void msOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->setFont(ArialMT_Plain_10);
  display->drawString(128, 0, "S: " + wm.getWLStatusString() + "Q: " + String(wm.getRSSIasQuality(WiFi.RSSI())));
}

void drawFrame3(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  // Text alignment demo
  display->setFont(ArialMT_Plain_16);

  // The coordinates define the left starting point of the text
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(0 + x, 11 + y, String(data[0]) + " " +String(data[1]) + " " +String(data[2]) + " " +String(data[3]) + " " +String(data[4]));
}

// This array keeps function pointers to all frames
// frames are the single views that slide in
FrameCallback frames[] = { drawFrame3 };
// how many frames are there?
int frameCount = 1;

OverlayCallback overlays[] = { msOverlay };
int overlaysCount = 1;

void setup() {
    Serial.begin(115200);
    Serial.setDebugOutput(true);  
    ui.setTargetFPS(10);
    ui.setIndicatorPosition(BOTTOM);

    // Defines where the first frame is located in the bar.
    ui.setIndicatorDirection(LEFT_RIGHT);
    ui.setFrameAnimation(SLIDE_LEFT);
    ui.disableAutoTransition();
    ui.setFrames(frames, frameCount);
    ui.setOverlays(overlays, overlaysCount);
    ui.init();
    display.flipScreenVertically();
    display.drawXbm(0 + 34, 0 + 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
    display.display();

    // WiFi stuff
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP  
    pinMode(TRIGGER_PIN, INPUT);
    
    // wm.resetSettings(); // wipe settings

    if(wm_nonblocking) wm.setConfigPortalBlocking(false);

    // add a custom input field
    int customFieldLength = 40;

    // new (&custom_field) WiFiManagerParameter("customfieldid", "Custom Field Label", "Custom Field Value", customFieldLength,"placeholder=\"Custom Field Placeholder\"");
    
    // test custom html input type(checkbox)
    // new (&custom_field) WiFiManagerParameter("customfieldid", "Custom Field Label", "Custom Field Value", customFieldLength,"placeholder=\"Custom Field Placeholder\" type=\"checkbox\""); // custom html type
    
    // test custom html(radio)
    const char* custom_radio_str = "<br/><label for='customfieldid'>Custom Field Label</label><input type='radio' name='customfieldid' value='1' checked> One<br><input type='radio' name='customfieldid' value='2'> Two<br><input type='radio' name='customfieldid' value='3'> Three";
    new (&custom_field) WiFiManagerParameter(custom_radio_str); // custom html input

    WiFiManagerParameter custom_artnet_target("artnet_ip", "Artnet Target", "", 15);
    WiFiManagerParameter custom_artnet_uni("artnet_uni", "Universe", "", 6);
    WiFiManagerParameter custom_btn1("btn1", "btn1 addr", "", 6);
    WiFiManagerParameter custom_btn2("btn2", "btn2 addr", "", 6);
    WiFiManagerParameter custom_btn3("btn3", "btn3 addr", "", 6);
    WiFiManagerParameter custom_btn4("btn4", "btn4 addr", "", 6);

    
    wm.addParameter(&custom_field);
    wm.addParameter(&custom_artnet_target);
    wm.addParameter(&custom_artnet_uni);
    wm.addParameter(&custom_btn1);
    wm.addParameter(&custom_btn2);
    wm.addParameter(&custom_btn3);
    wm.addParameter(&custom_btn4);
    wm.setSaveParamsCallback(saveParamCallback);

    // custom menu via array or vector
    // 
    // menu tokens, "wifi","wifinoscan","info","param","close","sep","erase","restart","exit" (sep is seperator) (if param is in menu, params will not show up in wifi page!)
    // const char* menu[] = {"wifi","info","param","sep","restart","exit"}; 
    // wm.setMenu(menu,6);
    std::vector<const char *> menu = {"wifi","info","param","sep","restart","exit"};
    wm.setMenu(menu);

    // set dark theme
    wm.setClass("invert");

    //set static ip
    // wm.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0)); // set static ip,gw,sn
    wm.setShowStaticFields(true); // force show static ip fields
    wm.setConfigPortalTimeout(120); // auto close configportal after n seconds
    bool res;
    res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

    if(!res) {
      Serial.println("Failed to connect or hit timeout");
      display.clear();
      display.setTextAlignment(TEXT_ALIGN_CENTER);
      display.drawString(64, 15, "Failed to connect or hit timeout");
      display.display();
      delay(1000);
      // ESP.restart();
    } 
    else {
      //if you get here you have connected to the WiFi    
      Serial.println("connected...yeey :)");
      display.clear();
      display.setTextAlignment(TEXT_ALIGN_CENTER);
      display.drawString(64, 15, "connected...yeey :)");
      display.display();
      delay(1000);
    }

    Serial.println(WiFi.localIP());
    ArduinoOTA.setPassword("geheimesPasswort");
    ArduinoOTA.begin();  
    ArduinoOTA.onStart([]() {
      display.displayOn();
      display.clear();
      display.setFont(ArialMT_Plain_10);
      display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
      display.drawString(display.getWidth()/2, display.getHeight()/2 - 10, "OTA Update");
      display.display();
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      display.displayOn();
      display.drawProgressBar(4, 32, 120, 8, progress / (total / 100) );
      display.display();
    });

    ArduinoOTA.onEnd([]() {
      display.displayOn();
      display.clear();
      display.setFont(ArialMT_Plain_10);
      display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
      display.drawString(display.getWidth()/2, display.getHeight()/2, "Restart");
      display.display();
    });


    artnet.begin();
    //button.setDebounceTime(100);

    // if Artnet packet comes to this universe, this function is called
    artnet.subscribeArtDmxUniverse(universe, [&](const uint8_t *data, uint16_t size, const ArtDmxMetadata& metadata, const ArtNetRemoteInfo& remote) {
        Serial.print("lambda : artnet data from ");
        Serial.print(remote.ip);
        Serial.print(":");
        Serial.print(remote.port);
        Serial.print(", universe = ");
        Serial.print(universe);
        Serial.print(", size = ");
        Serial.print(size);
        Serial.print(") :");
        for (size_t i = 0; i < size; ++i) {
            Serial.print(data[i]);
            Serial.print(",");
        }
        Serial.println();
    });

    // if Artnet packet comes, this function is called to every universe
    artnet.subscribeArtDmx([&](const uint8_t *data, uint16_t size, const ArtDmxMetadata& metadata, const ArtNetRemoteInfo& remote) {
        Serial.print("received ArtNet data from ");
        Serial.print(remote.ip);
        Serial.print(":");
        Serial.print(remote.port);
        Serial.print(", net = ");
        Serial.print(metadata.net);
        Serial.print(", subnet = ");
        Serial.print(metadata.subnet);
        Serial.print(", universe = ");
        Serial.print(metadata.universe);
        Serial.print(", sequence = ");
        Serial.print(metadata.sequence);
        Serial.print(", size = ");
        Serial.print(size);
        Serial.println(")");
    });

    button_1.begin(BUTTON_PIN_1);
    button_1.setPressedHandler(handleTap);
    button_1.setReleasedHandler(handleRelease);
    button_2.begin(BUTTON_PIN_2);
    button_2.setPressedHandler(handleTap);
    button_2.setReleasedHandler(handleRelease);
    button_3.begin(BUTTON_PIN_3);
    button_3.setPressedHandler(handleTap);
    button_3.setReleasedHandler(handleRelease);
    button_4.begin(BUTTON_PIN_4);
    button_4.setPressedHandler(handleTap);
    button_4.setReleasedHandler(handleRelease);
    button_5.begin(BUTTON_PIN_5);
    button_5.setPressedHandler(handleTap);
    button_5.setReleasedHandler(handleRelease);

    wifi_set_sleep_type(LIGHT_SLEEP_T);
    delay_10s.start(3000, AsyncDelay::MILLIS);
}

void checkButton(){
  // check for button press
  if ( digitalRead(TRIGGER_PIN) == LOW ) {
    // poor mans debounce/press-hold, code not ideal for production
    delay(50);
    if( digitalRead(TRIGGER_PIN) == LOW ){
      Serial.println("Button Pressed");
      // still holding button for 3000 ms, reset settings, code not ideaa for production
      delay(3000); // reset delay hold
      if( digitalRead(TRIGGER_PIN) == LOW ){
        Serial.println("Button Held");
        Serial.println("Erasing Config, restarting");
        wm.resetSettings();
        ESP.restart();
      }
      
      // start portal w delay
      Serial.println("Starting config portal");
      display.displayOn();
      display.clear();
      display.setTextAlignment(TEXT_ALIGN_CENTER);
      display.drawString(64, 15, "Starting config portal");
      display.display();
      delay(1000);
      wm.setConfigPortalTimeout(120);
      
      if (!wm.startConfigPortal("OnDemandAP","password")) {
        Serial.println("failed to connect or hit timeout");
        display.clear();
        display.setTextAlignment(TEXT_ALIGN_CENTER);
        display.drawString(64, 15, "failed to connect or hit timeout");
        display.display();
        delay(3000);
        // ESP.restart();
      } else {
        //if you get here you have connected to the WiFi
        Serial.println("connected...yeey :)");
        display.clear();
        display.setTextAlignment(TEXT_ALIGN_CENTER);
        display.drawString(64, 15, "connected...yeey :)");
        display.display();
      }
    }
  }
}

String getParam(String name){
  //read parameter from server, for customhmtl input
  String value;
  if(wm.server->hasArg(name)) {
    value = wm.server->arg(name);
  }
  return value;
}

void saveParamCallback(){
  Serial.println("[CALLBACK] saveParamCallback fired");
  Serial.println("PARAM customfieldid = " + getParam("customfieldid"));
}

void loop() {
    if(wm_nonblocking) wm.process(); // avoid delays() in loop when non-blocking and other long running code  
    ArduinoOTA.handle();
    checkButton();
    int remainingTimeBudget = ui.update();
    artnet.parse();  // check if artnet packet has come and execute callback
    button_1.loop();
    button_2.loop();
    button_3.loop();
    button_4.loop();
    button_5.loop();
    //value = (millis() / 4) % 256;
    //memset(data, value, size);

    if(delay_10s.isExpired())
    {
      display.displayOff();
    }

    artnet.setArtDmxData(data, size);
    artnet.streamArtDmxTo(target_ip, universe);  // automatically send set data in 40fps
    // artnet.streamArtDmxTo(target_ip, net, subnet, univ);  // or you can set net, subnet, and universe
}
