#include <Servo.h>
#include <math.h>

const int PIN_TRIG   = 9;  
const int PIN_ECHO   = 8;  
const int PIN_SERVO  = 11; 
const int PIN_LEDS_1 = 7;  // Olho Esquerdo (Porta PWM ~)
const int PIN_LEDS_2 = 6;  // Olho Direito (Porta PWM ~)

Servo meuServo;

// Variáveis para controlar o brilho atual de cada olho em tempo real
int brilhoOlho1 = 0;
int brilhoOlho2 = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_LEDS_1, OUTPUT);
  pinMode(PIN_LEDS_2, OUTPUT);
  
  meuServo.attach(PIN_SERVO);
  meuServo.write(90); // Mantém o rabo centralizado
  
  analogWrite(PIN_LEDS_1, 0);
  analogWrite(PIN_LEDS_2, 0);
}

float lerDistancia() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  
  long duracao = pulseIn(PIN_ECHO, HIGH, 25000); 
  if (duracao == 0) return -1.0;
  
  float resultado = duracao * 0.0343 / 2.0;
  if (resultado < 2.0 || resultado > 400.0) return -1.0;
  return resultado;
}

void loop() {
  float cm = lerDistancia();
  float tempo = millis() / 1000.0;

  Serial.print("Distancia: ");
  if (cm < 0) Serial.println("Ninguem na floresta...");
  else { Serial.print(cm, 1); Serial.println(" cm"); }

  // Alvos baseados na presença do usuário
  int brilhoAlvo1 = 0;
  int brilhoAlvo2 = 0;
  bool moverRabo = false;

  // =========================================================================
  // MÁQUINA DE ESTADOS: Define o destino de brilho baseado na distância
  // =========================================================================
  if (cm > 40.0 || cm == -1.0) {
    // CASO 1: Ninguém perto -> Os dois devem tender a apagar (0)
    brilhoAlvo1 = 0;
    brilhoAlvo2 = 0;
    moverRabo = false;
  }
  else if (cm > 10.0 && cm <= 40.0) {
    // CASO 2: Entre 40cm e 10cm -> Ficam acesos completamente no máximo (255)
    brilhoAlvo1 = 255;
    brilhoAlvo2 = 255;
    moverRabo = false;
  }
  else if (cm > 0.0 && cm <= 10.0) {
    // CASO 3: Menos de 10cm -> Ficam acesos completamente no máximo (255) + move o rabo
    brilhoAlvo1 = 255;
    brilhoAlvo2 = 255;
    moverRabo = true;
  }

  // =========================================================================
  // GERENCIADOR DE ANIMAÇÃO DOS LEDS (Suave, não-bloqueante e sequencial)
  // =========================================================================
  
  // SE O OBJETIVO É ACENDER (Brilho alvo é maior que o brilho atual)
  if (brilhoAlvo1 == 255) {
    // 1. Olho Esquerdo acende primeiro lentamente (+5 por ciclo)
    if (brilhoOlho1 < 255) {
      brilhoOlho1 += 5;
      if (brilhoOlho1 > 255) brilhoOlho1 = 255;
    }
    // 2. Olho Direito SÓ começa a acender se o esquerdo já terminou de ligar completamente
    if (brilhoOlho1 == 255 && brilhoOlho2 < 255) {
      brilhoOlho2 += 5;
      if (brilhoOlho2 > 255) brilhoOlho2 = 255;
    }
  }
  
  // SE O OBJETIVO É APAGAR (Brilho alvo é zero)
  else if (brilhoAlvo1 == 0) {
    // 1. Olho Esquerdo apaga primeiro e mais rápido (-15 por ciclo)
    if (brilhoOlho1 > 0) {
      brilhoOlho1 -= 15;
      if (brilhoOlho1 < 0) brilhoOlho1 = 0;
    }
    // 2. Olho Direito SÓ começa a apagar depois que o esquerdo apagou completamente
    if (brilhoOlho1 == 0 && brilhoOlho2 > 0) {
      brilhoOlho2 -= 15;
      if (brilhoOlho2 < 0) brilhoOlho2 = 0;
    }
  }

  // Aplica fisicamente os brilhos calculados
  analogWrite(PIN_LEDS_1, brilhoOlho1);
  analogWrite(PIN_LEDS_2, brilhoOlho2);

  // =========================================================================
  // CONTROLE DO SERVO MOTOR (RABO)
  // =========================================================================
  if (moverRabo) {
    // Movimento lento, lúdico e cínico para a cena da floresta
    int anguloCenico = 90 + 45 * sin(tempo * 1.2); 
    meuServo.write(anguloCenico);
  } else {
    meuServo.write(90); // Centralizado e imóvel fora dos 10cm
  }

  delay(40); // Sincroniza a velocidade do fade dos LEDs e estabiliza o sensor
}
