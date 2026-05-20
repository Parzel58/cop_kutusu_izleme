  #include <SoftwareSerial.h>

  SoftwareSerial esp8266(6, 7); // RX, TX

  #define trigPin 13
  #define echoPin 12

  #define redLed 11
  #define greenLed 10

  long sure;
  int mesafe;
  String durum;

  void setup() {
    Serial.begin(9600);
    esp8266.begin(9600);

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    pinMode(redLed, OUTPUT);
    pinMode(greenLed, OUTPUT);

    Serial.println("Cop kutusu izleme sistemi baslatiliyor...");

    sendCommand("AT+RST", 3000);
    sendCommand("AT+CWMODE=2", 1000);
    sendCommand("AT+CIFSR", 1000);
    sendCommand("AT+CIPMUX=1", 1000);
    sendCommand("AT+CIPSERVER=1,80", 1000);

    Serial.println("Web sunucusu hazir.");  
  }

  void loop() {
    mesafeOku();

    if (mesafe < 4) {
      durum = "DOLU";
      digitalWrite(redLed, HIGH);
      digitalWrite(greenLed, LOW);
    } else {
      durum = "BOS";
      digitalWrite(redLed, LOW);
      digitalWrite(greenLed, HIGH);
    }

    Serial.print("Mesafe: ");
    Serial.print(mesafe);
    Serial.print(" cm | Durum: ");
    Serial.println(durum);

    if (esp8266.available()) {
      if (esp8266.find("+IPD,")) {
        delay(500);

        int connectionId = esp8266.read() - 48;

        String webPage = "";
        webPage += "HTTP/1.1 200 OK\r\n";
        webPage += "Content-Type: text/html\r\n\r\n";
        webPage += "<!DOCTYPE HTML>";
        webPage += "<html>";
        webPage += "<head>";
        webPage += "<meta charset='UTF-8'>";
        webPage += "<meta http-equiv='refresh' content='2'>";
        webPage += "<title>Cop Kutusu Izleme</title>";
        webPage += "</head>";
        webPage += "<body style='text-align:center; font-family:Arial;'>";
        webPage += "<h1>NIT Tabanli Cop Kutusu Izleme Sistemi</h1>";
        webPage += "<h2>Mesafe: ";
        webPage += mesafe;
        webPage += " cm</h2>";
        webPage += "<h2>Cop Kutusu: ";
        webPage += durum;
        webPage += "</h2>";
        webPage += "</body>";
        webPage += "</html>";

        String cipSend = "AT+CIPSEND=";
        cipSend += connectionId;
        cipSend += ",";
        cipSend += webPage.length();

        sendCommand(cipSend, 1000);
        esp8266.print(webPage);
        delay(1000);

        String closeCommand = "AT+CIPCLOSE=";
        closeCommand += connectionId;
        sendCommand(closeCommand, 1000);
      }
    }

    delay(500);
  }

  void mesafeOku() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    sure = pulseIn(echoPin, HIGH);
    mesafe = sure * 0.034 / 2;
  }

  void sendCommand(String command, int bekleme) {
    esp8266.println(command);
    delay(bekleme);

    while (esp8266.available()) {
      Serial.write(esp8266.read());
    }
  }