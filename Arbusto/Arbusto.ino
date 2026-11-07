const int TRIG = 2;
const int ECHO = 3;

const int IN1 = 8;
const int IN2 = 7;

const int DISTANCIA_DETECCAO = 50;

const unsigned long TEMPO_ABRIR = 5000;
const unsigned long TEMPO_FECHAR = 5000;
const unsigned long TEMPO_ABERTO = 3000;

bool portaoAberto = false;

long medirDistancia() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duracao = pulseIn(ECHO, HIGH, 30000);

  if (duracao == 0) {
    return 999;
  }

  return duracao * 0.034 / 2;
}

void pararMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void abrirPortao() {
  Serial.println("Abrindo portao...");

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  delay(TEMPO_ABRIR);

  pararMotor();
  portaoAberto = true;

  Serial.println("Portao aberto!");
}

void fecharPortao() {

  delay(TEMPO_ABERTO);

  while (true) {

    while (medirDistancia() <= DISTANCIA_DETECCAO) {
      pararMotor();
      Serial.println("Objeto detectado! Aguardando...");
      delay(200);
    }

    Serial.println("Fechando portao...");

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

    unsigned long inicio = millis();
    bool obstaculo = false;

    while (millis() - inicio < TEMPO_FECHAR) {

      long distancia = medirDistancia();

      Serial.print("Distancia: ");
      Serial.print(distancia);
      Serial.println(" cm");

      if (distancia <= DISTANCIA_DETECCAO) {

        pararMotor();

        Serial.println("Obstaculo! Reabrindo...");

        abrirPortao();

        delay(TEMPO_ABERTO);

        obstaculo = true;
        break;
      }

      delay(100);
    }

    pararMotor();

    if (obstaculo) {
      continue;
    }

    break;
  }

  portaoAberto = false;

  Serial.println("Portao fechado!");
}

void setup() {

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  Serial.begin(9600);

  pararMotor();

  Serial.println("Sistema iniciado!");
}

void loop() {

  long distancia = medirDistancia();

  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");

  if (distancia <= DISTANCIA_DETECCAO && !portaoAberto) {

    abrirPortao();

    fecharPortao();
  }

  delay(100);
}
