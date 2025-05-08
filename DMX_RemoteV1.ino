#include "Button2.h"
#include "SSD1306Wire.h" 
#include "OLEDDisplayUi.h"

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



//Buttons
#define BUTTON_PIN_D3 14
#define BUTTON_PIN_D8 12
#define BUTTON_PIN_D7 13
#define BUTTON_PIN_D6 9
#define BUTTON_PIN_SCL 15


Button2 button_d3;
Button2 button_d8;
Button2 button_d7;
Button2 button_d6;
Button2 button_scl;

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
    // check for really long clicks
    ButtonData r;
    memset(data, 0, size);
    if (btn == button_d3) {
      Serial.println("BLUE clicked");
      memset(data, 50, size);
    } else if (btn == button_d8) {
      Serial.println("White clicked");
      btn_last=2;
      memset(data, 255, size);
    } else if (btn == button_d7) {
      Serial.println("GREEN clicked");
      btn_last=3;
      memset(data, 100, size);
    } else if (btn == button_d6) {
      Serial.println("RED clicked");
      memset(data, 150, size);
    } else if (btn == button_scl) {
      Serial.println("Yellow clicked");
      memset(data, 200, size);
    }

    
    artnet.setArtDmxData(data, size);
    artnet.streamArtDmxTo(target_ip, universe);
}

void drawFrame3(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  // Text alignment demo
  display->setFont(ArialMT_Plain_24);

  // The coordinates define the left starting point of the text
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(0 + x, 11 + y, "Btn: "+String(btn_names[btn_last]));
}

// This array keeps function pointers to all frames
// frames are the single views that slide in
FrameCallback frames[] = { drawFrame3 };

// how many frames are there?
int frameCount = 1;

void setup() {
    Serial.begin(115200);
    ui.setTargetFPS(10);
    ui.setIndicatorPosition(BOTTOM);

    // Defines where the first frame is located in the bar.
    ui.setIndicatorDirection(LEFT_RIGHT);
    ui.setFrameAnimation(SLIDE_LEFT);
    ui.disableAutoTransition();
    ui.setFrames(frames, frameCount);
    ui.init();
    display.flipScreenVertically();
    display.drawXbm(0 + 34, 0 + 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
    display.display();

    // WiFi stuff
    WiFi.begin(ssid, pwd);
    WiFi.config(ip, gateway, subnet);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.print("WiFi connected, IP = ");

    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 15, "WiFi connected");
    display.display();
    delay(500);

    Serial.println(WiFi.localIP());

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

    button_d3.begin(BUTTON_PIN_D3);
    button_d3.setTapHandler(handleTap);
    button_d8.begin(BUTTON_PIN_D8);
    button_d8.setTapHandler(handleTap);
    button_d7.begin(BUTTON_PIN_D7);
    button_d7.setTapHandler(handleTap);
    button_d6.begin(BUTTON_PIN_D6);
    button_d6.setTapHandler(handleTap);
    button_scl.begin(BUTTON_PIN_SCL);
    button_scl.setTapHandler(handleTap);
}

void loop() {
    int remainingTimeBudget = ui.update();
    artnet.parse();  // check if artnet packet has come and execute callback
    button_d3.loop();
    button_d8.loop();
    button_d7.loop();
    button_d6.loop();
    button_scl.loop();
    //value = (millis() / 4) % 256;
    //memset(data, value, size);

    artnet.setArtDmxData(data, size);
    artnet.streamArtDmxTo(target_ip, universe);  // automatically send set data in 40fps
    // artnet.streamArtDmxTo(target_ip, net, subnet, univ);  // or you can set net, subnet, and universe
}
