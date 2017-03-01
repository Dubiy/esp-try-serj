#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define BUTTON_PIN 0
#define LED_PIN 2
int delay_blink = 200;
volatile int ledStatus = 1;


// HTTP server will listen at port 80
ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), pressedButtonInt, FALLING); // CHANGE, RISING, FALLING

  WiFi.begin("uWorld333", "12345676");
  int tryCount = 40;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(F("WIFI status: "));
    Serial.println(WiFi.status());
    delay(500);

    if (tryCount-- <= 0) {
      break;
    }
  }
  Serial.print(F("WIFI status: "));
  Serial.println(WiFi.status());
  Serial.println(WiFi.localIP());



  server.on("/", []() {
    server.send(200, "text/html", form_html());
  });

  server.begin();


}

void loop() {
  digitalWrite(LED_PIN, ledStatus);   // turn the LED on (HIGH is the voltage level)
  server.handleClient();
}

String form_html() {
  String status = server.arg("status");
  Serial.println(status);
  if( status == "on" ) {
    ledStatus = 0;
  } else {
    ledStatus = 1;
  }

  return
  "<html><head><title>WiFi Weather-station :On/Off led</title></head><body>"
  "<center>"
  "<h2>Welcome to WiFi Led On/Off: v0.0.1</h2>"
  "<div>"
  "<form action='/'>"
  "<input id='on' type='submit' name='status' maxlength=64 value='on'><br />"
  "<input id='off' type='submit' name='status' maxlength=32 value='off'><br />"
  "</form>"
  "</div>"
  "</center>"
  "<style>*{font-size:120%;font-family: monospace;}form * {display: block; width: 100%; margin: 5px 0;}div{padding: 20px; margin: 5px 20px; border: 1px solid gray;}</style>"
  "</body></html>"
  ;
}


void pressedButtonInt() {
  //http://www.esp8266.com/viewtopic.php?f=32&t=4694&sid=4940a5d2d4b3658c7f8ff8b3e52c1595&start=4#sthash.2nZ3VJGH.dpuf
  // interrupt service routine (ISR) can ONLY modify VOLATILE variables

    ledStatus = -ledStatus + 1;
    Serial.print(F("Key pressed! Status: "));
    Serial.println(ledStatus);
}
