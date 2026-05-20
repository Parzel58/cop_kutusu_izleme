#include <ESP8266WiFi.h>

const char* ssid = "WIFI_ADINIZ";
const char* password = "WIFI_SIFRENIZ";

String apiKey = "THINGSPEAK_WRITE_API_KEY";

WiFiClient client;

#define trigPin D5
#define echoPin D6

#define yesilLED D1
#define kirmiziLED D2

const char* server = "api.thingspeak.com";

unsigned long sonGondermeZamani = 0;
const unsigned long gondermeAraligi = 15000; // 15 saniye

long mesafeOlc() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long sure = pulseIn(echoPin, HIGH, 30000);

  if (sure == 0) {
    return -1;
  }

  long mesafe = sure / 58;
  return mesafe;
}

void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(yesilLED, OUTPUT);
  pinMode(kirmiziLED, OUTPUT);

  WiFi.begin(ssid, password);

  Serial.println("WiFi baglaniyor...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi baglandi");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  long mesafe = mesafeOlc();
  int trashLevel = 0;

  if (mesafe > 0 && mesafe < 4) {
    trashLevel = 1;
    digitalWrite(kirmiziLED, HIGH);
    digitalWrite(yesilLED, LOW);
  } else {
    trashLevel = 0;
    digitalWrite(kirmiziLED, LOW);
    digitalWrite(yesilLED, HIGH);
  }

  Serial.print("Trash Level: ");
  Serial.print(trashLevel);
  Serial.print(" | Distance: ");
  Serial.print(mesafe);
  Serial.println(" cm");

  if (millis() - sonGondermeZamani >= gondermeAraligi) {
    sonGondermeZamani = millis();

    if (client.connect(server, 80)) {
      String url = "/update?api_key=";
      url += apiKey;
      url += "&field1=";
      url += trashLevel;
      url += "&field2=";
      url += mesafe;

      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                   "Host: api.thingspeak.com\r\n" +
                   "Connection: close\r\n\r\n");

      Serial.println("ThingSpeak'e veri gonderildi");
    } else {
      Serial.println("ThingSpeak baglantisi basarisiz");
    }

    client.stop();
  }

  delay(200);
}
