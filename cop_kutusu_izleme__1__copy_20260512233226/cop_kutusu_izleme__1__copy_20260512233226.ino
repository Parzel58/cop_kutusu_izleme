// ESP8266 WiFi kutuphanesi
#include <ESP8266WiFi.h>

// WiFi adi ve sifresi
const char* ssid = "WIFI_ADINIZ";
const char* password = "WIFI_SIFRENIZ";

// ThingSpeak API anahtari
String apiKey = "THINGSPEAK_WRITE_API_KEY";

// Internet baglantisi icin client nesnesi
WiFiClient client;

// Ultrasonik sensor pinleri
#define trigPin D5
#define echoPin D6

// LED pinleri
#define yesilLED D1
#define kirmiziLED D2

// ThingSpeak sunucu adresi
const char* server = "api.thingspeak.com";

// Veri gonderme zamani kontrolu
unsigned long sonGondermeZamani = 0;

// 10 saniyede bir veri gonder
const unsigned long gondermeAraligi = 10000; // 10 saniye

// Mesafe olcme fonksiyonu
long mesafeOlc() {

  // Trigger pinini sifirla
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sensoru tetikle
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Echo suresini olc
  long sure = pulseIn(echoPin, HIGH, 30000);

  // Veri gelmezse hata dondur
  if (sure == 0) {
    return -1;
  }

  // Mesafeyi cm cinsine cevir
  long mesafe = sure / 58;

  return mesafe;
}

// Arduino ilk acildiginda bir kez calisir
void setup() {

  // Seri haberlesmeyi baslat
  Serial.begin(115200);

  // Sensor pin ayarlari
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // LED pin ayarlari
  pinMode(yesilLED, OUTPUT);
  pinMode(kirmiziLED, OUTPUT);

  // WiFi baglantisini baslat
  WiFi.begin(ssid, password);

  Serial.println("WiFi baglaniyor...");

  // Baglanana kadar bekle
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Baglanti basarili
  Serial.println();
  Serial.println("WiFi baglandi");

  // IP adresini yazdir
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// Surekli tekrar eden kisim
void loop() {

  // Sensor mesafesini oku
  long mesafe = mesafeOlc();

  // Cop seviyesi
  int trashLevel = 0;

  // Mesafe 4 cm'den kucukse cop dolu kabul edilir
  if (mesafe > 0 && mesafe < 4) {

    // Dolu bilgisi
    trashLevel = 1;

    // Kirmizi LED yak
    digitalWrite(kirmiziLED, HIGH);

    // Yesil LED sondur
    digitalWrite(yesilLED, LOW);

  } else {

    // Bos bilgisi
    trashLevel = 0;

    // Kirmizi LED sondur
    digitalWrite(kirmiziLED, LOW);

    // Yesil LED yak
    digitalWrite(yesilLED, HIGH);
  }

  // Seri monitore veri yazdir
  Serial.print("Trash Level: ");
  Serial.print(trashLevel);

  Serial.print(" | Distance: ");
  Serial.print(mesafe);

  Serial.println(" cm");

  // Belirlenen sure doldu mu kontrol et
  if (millis() - sonGondermeZamani >= gondermeAraligi) {

    // Son gonderme zamanini guncelle
    sonGondermeZamani = millis();

    // ThingSpeak sunucusuna baglan
    if (client.connect(server, 80)) {

      // URL olustur
      String url = "/update?api_key=";

      // API key ekle
      url += apiKey;

      // Field1 = doluluk bilgisi
      url += "&field1=";
      url += trashLevel;

      // Field2 = mesafe bilgisi
      url += "&field2=";
      url += mesafe;

      // HTTP GET istegi gonder
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                   "Host: api.thingspeak.com\r\n" +
                   "Connection: close\r\n\r\n");

      // Basarili mesajı
      Serial.println("ThingSpeak'e veri gonderildi");

    } else {

      // Baglanti hatasi
      Serial.println("ThingSpeak baglantisi basarisiz");
    }

    // Baglantiyi kapat
    client.stop();
  }

  // Kisa bekleme
  delay(200);
}
