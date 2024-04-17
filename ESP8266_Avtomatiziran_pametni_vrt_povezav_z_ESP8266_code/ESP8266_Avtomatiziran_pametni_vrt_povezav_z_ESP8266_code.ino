#include <ESP8266WiFi.h> //knjiznica za povezovanje na wifi esp8266
#include <WiFiClientSecure.h> //knjiznica za povezovanje na wifi esp8266
#include <UniversalTelegramBot.h> //knjiznica za telegram bota
#include<SoftwareSerial.h> // knjiznica za serijsko komunikacijo (z arduinom)

const char* ssid = "wifi_name";// ssid oz. Ime wifija
const char* password = "wifi_password"; // geslo od wifija

#define BOTtoken "token" // Bottoken, ki smo ga dobili od botfather-ja
#define CHAT_ID "id" // chatID, ki smo ga dobili od chatid bota

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
SoftwareSerial NodeMcu_SoftSerial(D1, D2);  //SRX = D1 povezano na 11, D2 povezano na 10 na Arduinu

String mojNiz; // niz z imenom dataIn, ki bo vseboval podatke, ki jih dobi od Arduina
String kombiniran;

void setup()
{
  Serial.begin(115200); // inicializiramo serijsko komunikacijo na vrednosti 115200
  NodeMcu_SoftSerial.begin(9600);

  configTime(0, 0, "pool.ntp.org");      
  client.setTrustAnchors(&cert);
  // Povezava na wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int a = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    a++;
  }
  // Serijski izpisi v programu, ter izpisi za Bota v telegramu.
  Serial.println("");
  Serial.println("WiFi je povezan");
  Serial.print("IP naslov: ");
  Serial.println(WiFi.localIP());
  bot.sendMessage(CHAT_ID, "Wifi je povezan!", "");
  bot.sendMessage(CHAT_ID, "Program je zagnan!", "");
}

void loop() {
  static unsigned long prejsniMillis = 0;
  const unsigned long interval = 500; // Prilagodi interval po potrebi

  unsigned long trenutniMillis = millis();

  // Preveri, ali je čas za branje podatkov iz ESP8266
  if (trenutniMillis - prejsniMillis >= interval) {
    prejsniMillis = trenutniMillis;

    // Preberi znake iz ESP8266 oz. Arduina in jih združi
    while (NodeMcu_SoftSerial.available() > 0) {
      delay(10);
      char prejetZnak = NodeMcu_SoftSerial.read();
      String mojNiz(prejetZnak);
      
      // Prezri prazne nize
      if (mojNiz != "") {
        kombiniran += mojNiz;
      }
    }

    // Obdelaj prejete podatke, če niso prazni
    if (kombiniran != "") {
      Serial.println(kombiniran);  
      bot.sendMessage(CHAT_ID, kombiniran);
      kombiniran = ""; // Počisti združen niz za naslednji cikel
    }
  }
}
