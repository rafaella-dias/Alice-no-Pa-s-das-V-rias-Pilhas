const int ENA = 3;
const int ENB = 5;
const int IN1 = 2;
const int IN2 = 4;
const int IN3 = 6;
const int IN4 = 7;
const int SENSOR_DIR = A0;
const int SENSOR_ESQ = A1;

const int VELOCIDADE_CRUZEIRO = 70;
const int VELOCIDADE_CURVA = 100;

const float CORRECAO_MOTOR_ESQ = 1.00;
const float CORRECAO_MOTOR_DIR = 1.00;

int corteCalibracaoEsq = 500;
int corteCalibracaoDir = 500;

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(SENSOR_DIR, INPUT);
  pinMode(SENSOR_ESQ, INPUT);

  Serial.begin(9600);

  parar();

  Serial.println("Calibrando sensores...");
  Serial.println("Passe os DOIS sensores no PRETO e no BRANCO!");

  int valorMinimoEsq = 1023;
  int valorMaximoEsq = 0;
  int valorMinimoDir = 1023;
  int valorMaximoDir = 0;

  unsigned long tempoInicial = millis();

  while (millis() - tempoInicial < 5000) {
    int leituraEsq = analogRead(SENSOR_ESQ);
    int leituraDir = analogRead(SENSOR_DIR);

    if (leituraEsq < valorMinimoEsq) {
      valorMinimoEsq = leituraEsq;
    }
    if (leituraEsq > valorMaximoEsq) {
      valorMaximoEsq = leituraEsq;
    }

    if (leituraDir < valorMinimoDir) {
      valorMinimoDir = leituraDir;
    }
    if (leituraDir > valorMaximoDir) {
      valorMaximoDir = leituraDir;
    }

    delay(10);
  }

  corteCalibracaoEsq = (valorMinimoEsq + valorMaximoEsq) / 2;
  corteCalibracaoDir = (valorMinimoDir + valorMaximoDir) / 2;

  Serial.println();
  Serial.println("--- SENSORES CALIBRADOS! ---");
  Serial.print("Sensor ESQUERDO - Minimo: ");
  Serial.print(valorMinimoEsq);
  Serial.print(" | Maximo: ");
  Serial.print(valorMaximoEsq);
  Serial.print(" | Corte: ");
  Serial.println(corteCalibracaoEsq);
  Serial.print("Sensor DIREITO - Minimo: ");
  Serial.print(valorMinimoDir);
  Serial.print(" | Maximo: ");
  Serial.print(valorMaximoDir);
  Serial.print(" | Corte: ");
  Serial.println(corteCalibracaoDir);
  Serial.println("-----------------------------");

  delay(2000);
}

void loop() {
  int leituraDir = analogRead(SENSOR_DIR);
  int leituraEsq = analogRead(SENSOR_ESQ);

  bool naLinhaEsq = (leituraEsq < corteCalibracaoEsq);
  bool naLinhaDir = (leituraDir < corteCalibracaoDir);

  if (!naLinhaEsq && !naLinhaDir) {
    frente();
  }
  else if (naLinhaEsq && !naLinhaDir) {
    virarEsquerda();
  }
  else if (!naLinhaEsq && naLinhaDir) {
    virarDireita();
  }
  else {
    parar();
  }

  delay(10);
}

void frente() {
  analogWrite(ENA, VELOCIDADE_CRUZEIRO * CORRECAO_MOTOR_ESQ);
  analogWrite(ENB, VELOCIDADE_CRUZEIRO * CORRECAO_MOTOR_DIR);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void virarEsquerda() {
  analogWrite(ENA, VELOCIDADE_CURVA * CORRECAO_MOTOR_ESQ);
  analogWrite(ENB, VELOCIDADE_CURVA * CORRECAO_MOTOR_DIR);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void virarDireita() {
  analogWrite(ENA, VELOCIDADE_CURVA * CORRECAO_MOTOR_ESQ);
  analogWrite(ENB, VELOCIDADE_CURVA * CORRECAO_MOTOR_DIR);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void parar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
