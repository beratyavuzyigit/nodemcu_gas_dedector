#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// WiFi Ağı Bilgilerini Giriniz
const char* ssid = "wifi_adi";
const char* password = "wifi_sifresi";

// Initialize Telegram BOT
#define BOTtoken "XXXX:XXXXXXXX"  // Botfather'dan oluşturduğunuz botun Token'ı

// @myidbot'u oluşturduğunuz gruba davet edin.
// Bot komut ile gurubun ID'sini alın ve aşağıya yazın.
#define CHAT_ID "-XXXXXX"

int deger = 0;
int esikDegeri = 350;
int beklemeSuresi = 500;
int buzzerPin = 13;

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

void setup() {
  Serial.begin(115200);
  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org

  // Kontrol amaçlı aşağıdaki kodları kullanabilirsiniz.
  // Serial.print("WiFi Ağına Bağlanılıyor: ");
  // Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    // Serial.print(".");
    delay(500);
  }

  // Kontrol amaçlı aşağıdaki kodları kullanabilirsiniz.
  // Serial.println("");
  // Serial.println("WiFi Bağlantısı Başarılı.");
  // Serial.print("IP Adresi: ");
  // Serial.println(WiFi.localIP());

  bot.sendMessage(CHAT_ID, "Bot çalıştırıldı!", ""); // Çalıştığını gösteren bir mesaj yollar.
}

void loop() {
  deger = analogRead(A0); // Sensörden analog değer okuyoruz.
  delay(beklemeSuresi); // Programı çalışmadan önce atanan değer kadar bekletir.
  if (deger > esikDegeri) { //Sensörden okunan değer eşik değerinden büyükse çalışır.
    bot.sendMessage(CHAT_ID, "Gaz Algılandı!", ""); // Gazın algılandığını gösteren bir emsaj atar.
    beklemeSuresi = 10000; // Ard arda mesaj atmasını önlemek için 10 saniyelik bekleme süresine girer (1000 = 1 saniye).
    digitalWrite(buzzerPin, HIGH); // Buzzer'ı aktif eder.
    delay(10000); // Buzzer'ın 10 saniye boyunca ötmesini sağlar.
    digitalWrite(buzzerPin, LOW); // Belirtilen süre sonrasında pasifleşir.
  } else { //Sensörden okunan değer eşik değerinin altındaysa çalışır.
    beklemeSuresi = 500; // Eğer gaz algılanmazsa bekleme süresini yarım saniyeye çeker.
  }
}
