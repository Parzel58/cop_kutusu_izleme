# Akilli Cop Kutusu Izleme Sistemi

Bu proje, ESP8266 ve HC-SR04 ultrasonik mesafe sensoru kullanarak cop kutusunun doluluk durumunu takip eder. Sensor cop kutusunun icindeki mesafeyi olcer; mesafe belirlenen esigin altina dustugunde sistem cop kutusunu dolu kabul eder, kirmizi LED'i yakar ve ThingSpeak platformuna veri gonderir.

## Proje Ozeti

- Mesafe olcumu HC-SR04 ultrasonik sensor ile yapilir.
- ESP8266 Wi-Fi uzerinden internete baglanir.
- ThingSpeak'e iki veri gonderilir:
  - `field1`: Cop kutusu durumu (`0 = bos`, `1 = dolu`)
  - `field2`: Olculen mesafe degeri
- Yesil LED cop kutusu bosken, kirmizi LED cop kutusu doluyken yanar.

## Kullanilan Malzemeler

- NodeMCU ESP8266 gelistirme karti
- HC-SR04 ultrasonik mesafe sensoru
- 1 adet kirmizi LED
- 1 adet yesil LED
- 2 adet 220 ohm direnc
- Breadboard
- Jumper kablolar
- USB kablosu
- Bilgisayar ve Arduino IDE
- ThingSpeak hesabi

> Not: HC-SR04 sensorunun Echo pini 5V cikis verebilir. ESP8266 pinleri 3.3V seviyesinde calistigi icin Echo pini ile ESP8266 arasinda voltaj bolucu kullanmak daha guvenlidir.

## Baglanti Semasi

| Parca | ESP8266 Pini |
| --- | --- |
| HC-SR04 VCC | 5V / VIN |
| HC-SR04 GND | GND |
| HC-SR04 Trig | D5 |
| HC-SR04 Echo | D6 |
| Yesil LED | D1 |
| Kirmizi LED | D2 |

LED kullanirken her LED'e seri olarak 220 ohm direnc baglayin.

## Kurulum

1. Arduino IDE'yi acin.
2. ESP8266 kart destegini Arduino IDE'ye ekleyin.
3. Kart olarak `NodeMCU 1.0 (ESP-12E Module)` secin.
4. `cop_kutusu_izleme__1_/cop_kutusu_izleme__1_.ino` dosyasini acin.
5. Kodun basindaki Wi-Fi ve ThingSpeak bilgilerini kendinize gore duzenleyin:

```cpp
const char* ssid = "WIFI_ADINIZ";
const char* password = "WIFI_SIFRENIZ";
String apiKey = "THINGSPEAK_WRITE_API_KEY";
```

6. ThingSpeak'te yeni bir channel olusturun.
7. Channel ayarlarinda `Field 1` ve `Field 2` alanlarini aktif edin.
8. ThingSpeak `Write API Key` degerini koddaki `apiKey` alanina yazin.
9. Devre baglantilarini yapin.
10. Kodu ESP8266 kartina yukleyin.
11. Seri Monitor'u `115200 baud` hizinda acarak verileri kontrol edin.

## Calisma Mantigi

Kod once Wi-Fi agina baglanir. Daha sonra HC-SR04 sensoru ile surekli mesafe olcumu yapar. Olculen mesafe 4 cm'den kucukse cop kutusu dolu kabul edilir ve `trashLevel` degeri `1` olur. Mesafe daha buyukse cop kutusu bos kabul edilir ve `trashLevel` degeri `0` olur.

Belirlenen araliklarla ESP8266, ThingSpeak sunucusuna HTTP GET istegi gonderir. Boylece doluluk durumu ve mesafe degeri internet uzerinden izlenebilir.

## ThingSpeak Alanlari

| Alan | Anlam |
| --- | --- |
| Field 1 | Doluluk durumu: `0 = bos`, `1 = dolu` |
| Field 2 | Sensor tarafindan olculen mesafe |

## Dosyalar

- `cop_kutusu_izleme__1_/cop_kutusu_izleme__1_.ino`: ESP8266 ve ThingSpeak kullanan ana proje kodu.
- `cop_kutusu_izleme__1__copy_20260512233226/`: Aciklamali yedek surum.
- `sketch_may11a/`: ESP8266 AT komutlariyla yerel web sunucusu denemesi.

## Gelistirme Fikirleri

- Doluluk esigi cop kutusunun boyuna gore ayarlanabilir.
- ThingSpeak uzerinden grafikler daha detayli hale getirilebilir.
- Pil seviyesi takibi eklenebilir.
- Birden fazla cop kutusu ayni sistemden izlenebilir.
- Web veya mobil arayuz eklenerek durum daha kullanisli sekilde gosterilebilir.
