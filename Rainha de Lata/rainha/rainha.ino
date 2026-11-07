#include <Servo.h>

Servo meusOlhos; Servo minhasPalpebras; Servo meuBraco;

const int pinoServoOlhos = 2;   const int pinoServoPalpebras = 3;   const int pinoBraco = 7;
const int pinoSensor = 8;       
const int PUL1_DIR = 10;        const int DIR1_DIR = 11;
const int PUL2_ESQ = 12;        const int DIR2_ESQ = 13;

const int posicaoCentral = 90;  const int posicaoAberta = 70;
const int posicaoBrava = 90;    const int posicaoFechada = 180;


float freqHz = 800; 

int anguloBraco = 110;
int direcaoBraco = -1; 

bool interromperCaca = false;

void setup() {
  meusOlhos.attach(pinoServoOlhos); minhasPalpebras.attach(pinoServoPalpebras); meuBraco.attach(pinoBraco);
  meusOlhos.write(posicaoCentral);  minhasPalpebras.write(posicaoAberta);       meuBraco.write(110);
  delay(1000);

  pinMode(pinoSensor, INPUT); 
  pinMode(PUL1_DIR, OUTPUT); pinMode(DIR1_DIR, OUTPUT);
  pinMode(PUL2_ESQ, OUTPUT); pinMode(DIR2_ESQ, OUTPUT);
}

void loop() {
  int estadoSensor = digitalRead(pinoSensor);

  if (estadoSensor == HIGH) {
    interromperCaca = false; 
    
    digitalWrite(DIR1_DIR, LOW);
    digitalWrite(DIR2_ESQ, HIGH);
    
    olhar_procurando_brava();
  } 
  else {
    parar_motores();
    meuBraco.write(110); 
    olhar_simples();
    piscada_simples();
    delay(100); 
  }
}

void esperar_e_mover(int tempoMs) {
  unsigned long inicio = millis();
  int meioPeriodo = (1000000 / freqHz) / 2;
  static unsigned long ultimoMovimentoBraco = 0;

  while (millis() - inicio < tempoMs) {
    if (digitalRead(pinoSensor) == LOW) {
      parar_motores();
      interromperCaca = true; 
      return; 
    }


    digitalWrite(PUL1_DIR, HIGH); digitalWrite(PUL2_ESQ, HIGH);
    delayMicroseconds(meioPeriodo);
    digitalWrite(PUL1_DIR, LOW);  digitalWrite(PUL2_ESQ, LOW);
    delayMicroseconds(meioPeriodo);


    if (millis() - ultimoMovimentoBraco >= 15) {
      ultimoMovimentoBraco = millis();
      anguloBraco += (direcaoBraco * 4); 
      
      if (anguloBraco <= 0) {
        anguloBraco = 0;
        direcaoBraco = 1; 
      }
      else if (anguloBraco >= 110) {
        anguloBraco = 110;
        direcaoBraco = -1; 
      }
      meuBraco.write(anguloBraco);
    }
  }
}

void parar_motores() {
  digitalWrite(PUL1_DIR, LOW); digitalWrite(PUL2_ESQ, LOW);
}

void olhar_simples() {
  int alvoDireita = posicaoCentral + 60;
  for (int pos = posicaoCentral; pos <= alvoDireita; pos++) { meusOlhos.write(pos); delay(10); }
  delay(400);
  for (int pos = alvoDireita; pos >= posicaoCentral; pos--) { meusOlhos.write(pos); delay(10); }
  int alvoEsquerda = posicaoCentral - 60;
  for (int pos = posicaoCentral; pos >= alvoEsquerda; pos--) { meusOlhos.write(pos); delay(10); }
  delay(400);
  for (int pos = alvoEsquerda; pos <= posicaoCentral; pos++) { meusOlhos.write(pos); delay(10); }
}

void piscada_simples() {
  minhasPalpebras.write(posicaoFechada); delay(120);
  minhasPalpebras.write(posicaoAberta);  delay(200);
}

void olhar_procurando_brava() {
  for (int repeticao = 0; repeticao < 2; repeticao++) {
    if (interromperCaca) return;

    minhasPalpebras.write(posicaoBrava);
    esperar_e_mover(100); if (interromperCaca) return;

    int alvoDireita = posicaoCentral + 60;
    for (int pos = posicaoCentral; pos <= alvoDireita; pos += 3) { 
      meusOlhos.write(pos); 
      esperar_e_mover(10); 
      if (interromperCaca) return;
    }
    esperar_e_mover(80); if (interromperCaca) return;

    int alvoEsquerda = posicaoCentral - 60;
    for (int pos = alvoDireita; pos >= alvoEsquerda; pos -= 4) { 
      meusOlhos.write(pos); 
      esperar_e_mover(2); 
      if (interromperCaca) return;
    }
    esperar_e_mover(80); if (interromperCaca) return;

    for (int pos = alvoEsquerda; pos <= posicaoCentral; pos += 3) { 
      meusOlhos.write(pos); 
      esperar_e_mover(10); 
      if (interromperCaca) return;
    }
    esperar_e_mover(50); if (interromperCaca) return;

    minhasPalpebras.write(posicaoFechada); esperar_e_mover(60); if (interromperCaca) return; minhasPalpebras.write(posicaoBrava); esperar_e_mover(50); if (interromperCaca) return;
    minhasPalpebras.write(posicaoFechada); esperar_e_mover(60); if (interromperCaca) return; minhasPalpebras.write(posicaoBrava); esperar_e_mover(50); if (interromperCaca) return;
    minhasPalpebras.write(posicaoFechada); esperar_e_mover(60); if (interromperCaca) return; minhasPalpebras.write(posicaoBrava); esperar_e_mover(50); if (interromperCaca) return;
    
    if (repeticao == 0) { esperar_e_mover(300); if (interromperCaca) return; }
  }
}
