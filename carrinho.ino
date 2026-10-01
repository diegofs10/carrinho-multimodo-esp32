#include <ESP32Servo.h>
#include <WiFi.h>
#include "pagina.h"
#include <WebServer.h>
#define IN1 26
#define IN2 27
#define IN3 25
#define IN4 33
#define ENA 14
#define ENB 12
#define MSERVO 32
#define ECHO 18
#define TRIG 5

Servo mservo;
String mensagem = "";
int velocidade = 60;
char modo_op = 'C';
unsigned long ult_men_temp = 0;
const int timeout = 150;
const char* ssid = "Turbo_Baby";
const char* password = "TurboBaby";
float correcao = 0.75;
WebServer server(80);

long lerDistancia() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duracao = pulseIn(ECHO, HIGH, 30000);

  if (duracao == 0) return 999;

  long distancia = duracao * 0.034 / 2;
  return distancia;
}
void pilotagemAnalogica(int x, int y) {
  float limiteY = (y * velocidade) / 100.0;
  
  float limiteX = ((x * velocidade) / 100.0) * 0.60; 

  float vel_A = limiteY + limiteX;
  float vel_B = limiteY - limiteX;

  if (vel_A > 100) vel_A = 100;
  if (vel_A < -100) vel_A = -100;
  if (vel_B > 100) vel_B = 100;
  if (vel_B < -100) vel_B = -100;

  int pwm_A = 0;
  int pwm_B = 0;

  if (abs(vel_A) > 5){
    pwm_A = map(abs(vel_A), 0, 100, 150, 255) * correcao;
  }
  if (abs(vel_B) > 5){
    pwm_B = map(abs(vel_B), 0, 100, 150, 255);
  }
  ledcWrite(ENA, pwm_A);
  ledcWrite(ENB, pwm_B);
  if (vel_A > 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else if (vel_A < 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }
  if (vel_B > 0) {
    digitalWrite(IN3, HIGH); 
    digitalWrite(IN4, LOW);
  } else if (vel_B < 0) {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }
}
void setup() {
  Serial.begin(115200);

  Serial.println("Iniciando Wi-Fi (Access Point)...");
  WiFi.softAP(ssid, password);
  Serial.print("IP para conectar no celular: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", []() {
    server.send(200, "text/html", paginaHTML);
  });

  server.on("/comando", []() {
    if (server.hasArg("cmd")) {
      String comando_recebido = server.arg("cmd");
      
      mensagem = comando_recebido;
      ult_men_temp = millis();
      
      Serial.print("Comando recebido: ");
      Serial.println(mensagem);
    }
    server.send(200, "text/plain", "OK");
  });

  server.on("/joy", []() {
    if (server.hasArg("x") && server.hasArg("y")) {
      int eixoX = server.arg("x").toInt();
      int eixoY = server.arg("y").toInt();
      ult_men_temp = millis();

      if (modo_op == 'C'){
        // Rastreador injetado na linha 116:
        Serial.println("--- SINAL DA LUVA RECEBIDO ---");
        Serial.print("Eixo X: ");
        Serial.println(eixoX);
        Serial.print("Eixo Y: ");
        Serial.println(eixoY);
      
        pilotagemAnalogica(eixoX, eixoY);
      }
    server.send(200, "text/plain", "OK");
    }
  });

  server.begin();

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  
  mservo.setPeriodHertz(50);
  mservo.attach(MSERVO);
  mservo.write(95);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  ledcAttach(ENA, 5000, 8);
  ledcAttach(ENB, 5000, 8);
  aplicarvelocidade();
  
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
}
void aplicarvelocidade(){
    int valorPWM = (velocidade * 255) / 100;
    int PWM_A = valorPWM * correcao;
    int PWM_B = valorPWM;
    ledcWrite(ENA, PWM_A);
    ledcWrite(ENB, PWM_B);
}
void frente(){
  if (lerDistancia() < 30) {
    Serial.println("Freio de emergencia");
    stop();
  }
  else {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
}
void tras(){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}
void esquerda(){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}
void esquerdapulso(){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    delay(200);
    stop();
}
void direita(){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}
void direitapulso(){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    delay(200);
    stop();
}
void stop(){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, HIGH);
}
void livre() {
    long dist = lerDistancia();
    if (dist > 35 || dist == 999) {
        digitalWrite(IN1, HIGH); digitalWrite(IN3, HIGH);
        digitalWrite(IN2, LOW); digitalWrite(IN4, LOW);
    }
    else{
        stop();

        mservo.write(175);
        delay(500);
        long distEsq = lerDistancia();

        mservo.write(15);
        delay(500);
        long distDir = lerDistancia();

        mservo.write(95);
        delay(400);

        if (distEsq > distDir) {
            esquerda();
            delay(400);
            stop();
        }
        else if (distDir > distEsq) {
            direita();
            delay(400);
            stop();
        }
        else {
            tras();
            delay(500);
            direita();
            delay(500);
            stop();
        }
    }
}
void pet() {
    long dist = lerDistancia();

    // 1. Filtro Anti-Ruído Wi-Fi: Se deu 999, tenta de novo rápido
    if (dist == 999 || dist > 150) {
        delay(15);
        dist = lerDistancia();
    }

    if (dist < 30 && dist != 999) {
        mservo.write(95);
        tras();        
    }
    else if (dist >= 30 && dist <= 50){
        mservo.write(95);
        stop();        
    }
    else if (dist > 50 && dist <= 120) {
        mservo.write(95);
        // 2. Injeta energia direto nas rodas, evitando o freio da função frente()
        digitalWrite(IN1, HIGH); digitalWrite(IN3, HIGH);
        digitalWrite(IN2, LOW);  digitalWrite(IN4, LOW);
    }
    else {
        stop(); 

        // 3. Scan Rápido nas Diagonais (Evita ler a própria roda)
        mservo.write(125);
        delay(250);
        long distEsq = lerDistancia();

        mservo.write(65);
        delay(300);
        long distDir = lerDistancia();

        mservo.write(95);
        delay(250);

        // 4. O Segredo: Reduzimos a caça para no máximo 90cm
        // Ele vai ignorar totalmente as paredes e focar no seu passo
        long distAlvo = 91; 
        int direcao = 0;

        if (distEsq < distAlvo && distEsq != 999) { distAlvo = distEsq; direcao = 1; }
        if (distDir < distAlvo && distDir != 999) { distAlvo = distDir; direcao = 2; }

        // Movimento rápido de correção do eixo
        if (direcao == 1) {
            esquerda(); delay(300); stop();
        } 
        else if (direcao == 2) {
            direita(); delay(300); stop();
        }
    }
}
void loop() {
    server.handleClient();

    if (mensagem != "") {

        if (mensagem == "C") {
            modo_op = 'C';
            aplicarvelocidade();
            stop();
        }
        else if (mensagem == "P") {
            modo_op = 'P';
            aplicarvelocidade();
            stop();
        }
        else if (mensagem == "L") {
            modo_op = 'L';
            aplicarvelocidade();
            stop();
        }
        else if (mensagem == "A") {
            modo_op = 'A';
            aplicarvelocidade();
            stop();
        }
        else if (mensagem == "F") {
            Serial.println("Comando Frente");
            frente();
        }
        else if (mensagem == "T") {
            Serial.println("Comando Ré");
            tras();
        }
        else if (mensagem == "D") {
            if (modo_op == 'P'){
                direita();                
            } else {
                direita();
            }
        }
        else if (mensagem == "E") {
            if (modo_op == 'P'){
                esquerda();
            } else {
                esquerda();
            }
        }
        else if (mensagem == "S") {
            Serial.println("Comando Frear");
            stop();
        }
        else if (mensagem.startsWith("V")) {
            velocidade = mensagem.substring(1).toInt();
            if (velocidade > 0 && velocidade < 60) {
                velocidade = 60;
            }
            aplicarvelocidade();
        }
        mensagem = "";
    }
    if (modo_op == 'C' && (millis() - ult_men_temp > timeout)){
        stop();
    }
    if (modo_op == 'A') {
        pet();
    }
    else if(modo_op == 'L') {
        livre();
    }
}