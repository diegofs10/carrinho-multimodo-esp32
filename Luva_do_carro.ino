#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

// Configurações da rede Wi-Fi criada pelo carrinho
const char* ssid = "Turbo_Baby";
const char* password = "TurboBaby";
const String urlCarrinho = "http://192.168.4.1/joy";

// Controle de fluxo (Limite de 20 pacotes por segundo)
unsigned long ultimoEnvio = 0;
const int intervaloEnvio = 50; 

void setup() {
  Serial.begin(115200);

  // 1. Inicializa o Wi-Fi no modo Cliente (Station)
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Procurando a rede do Turbo Baby...");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado ao Chassi com sucesso!");

  // 2. Inicializa a comunicação I2C com o MPU6050
  if (!mpu.begin()) {
    Serial.println("Falha ao encontrar o chip MPU6050. Verifique os fios SDA e SCL.");
    while (1) { delay(10); } // Trava o sistema por segurança
  }
  Serial.println("MPU6050 detectado e calibrado!");
  
  // Ajuste fino para estabilidade do giroscópio
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
  // Dispara pacotes a cada 50ms para evitar sobrecarga na rede
  if (millis() - ultimoEnvio >= intervaloEnvio) {
    ultimoEnvio = millis();

    // Captura os dados analógicos da gravidade no pulso
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // Converte a força G (-9.8 a 9.8) para a escala de porcentagem (-100 a 100)
    // O eixo Y controla Frente/Ré e o eixo X controla a direção
    int eixoY = map(a.acceleration.y * 100, -1000, 1000, -100, 100);
    int eixoX = map(a.acceleration.x * 100, -1000, 1000, -100, 100);

    // Trava os valores no limite matemático do chassi
    eixoY = constrain(eixoY, -100, 100);
    eixoX = constrain(eixoX, -100, 100);

    // Zona Morta: Ignora tremores naturais da mão para evitar que o robô engasgue
    if (abs(eixoY) < 15) eixoY = 0;
    if (abs(eixoX) < 15) eixoX = 0;

    // Se estiver conectado à rede do chassi, atira o pacote HTTP
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      
      // Constrói a rota /joy com as variáveis que o carrinho espera
      String pacoteDados = urlCarrinho + "?x=" + String(eixoX) + "&y=" + String(eixoY);
      
      http.begin(pacoteDados);
      int httpCode = http.GET(); 
      
      if (httpCode > 0) {
        Serial.printf("Enviado: X=%d Y=%d | Resposta Chassi: %d\n", eixoX, eixoY, httpCode);
      } else {
        Serial.println("Falha ao entregar o pacote via Wi-Fi");
      }
      
      http.end(); // Encerra a conexão para liberar memória
    }
  }
}