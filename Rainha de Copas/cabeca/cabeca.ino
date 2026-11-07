#include <Servo.h>

Servo meusOlhos;
Servo minhasPalpebras;

const int pinoServoOlhos = 2;
const int pinoServoPalpebras = 3;

const int posicaoCentral = 90;
const int posicaoAberta = 70;
const int posicaoBrava = 90;
const int posicaoFechada = 180;

void setup() {
  meusOlhos.attach(pinoServoOlhos);
  minhasPalpebras.attach(pinoServoPalpebras);

  meusOlhos.write(posicaoCentral);
  minhasPalpebras.write(posicaoAberta);
  delay(1000);
}

void loop() {
  olhar_simples();
  piscada_simples();
  delay(2000);
}

void olhar_simples() {
  int alvoDireita = posicaoCentral + 60;

  for (int pos = posicaoCentral; pos <= alvoDireita; pos++) {
    meusOlhos.write(pos);
    delay(10);
  }

  delay(400);

  for (int pos = alvoDireita; pos >= posicaoCentral; pos--) {
    meusOlhos.write(pos);
    delay(10);
  }




  int alvoEsquerda = posicaoCentral - 60;

  for (int pos = posicaoCentral; pos >= alvoEsquerda; pos--) {
    meusOlhos.write(pos);
    delay(10);
  }

  delay(400);

  for (int pos = alvoEsquerda; pos <= posicaoCentral; pos++) {
    meusOlhos.write(pos);
    delay(10);
  }
}


void piscada_simples() {
  minhasPalpebras.write(posicaoFechada);
  delay(120); // Aumentado de 20 para 120ms para o motor ter tempo de esmagar/fechar tudo
  minhasPalpebras.write(posicaoAberta); 
  delay(200);
}


void piscada_rapida() {
  minhasPalpebras.write(posicaoFechada);
  delay(80); 
  minhasPalpebras.write(posicaoAberta); 
  delay(80);
  
  minhasPalpebras.write(posicaoFechada);
  delay(80); 
  minhasPalpebras.write(posicaoAberta); 
  delay(80);
}


void olhar_procurando_brava() {
  for (int repeticao = 0; repeticao < 2; repeticao++) {

    minhasPalpebras.write(posicaoBrava);
    delay(100); 

    int alvoDireita = posicaoCentral + 60;
    for (int pos = posicaoCentral; pos <= alvoDireita; pos += 3) { 
      meusOlhos.write(pos); 
      delay(10); 
    }
    delay(80); 

    int alvoEsquerda = posicaoCentral - 60;
    for (int pos = alvoDireita; pos >= alvoEsquerda; pos -= 4) { 
      meusOlhos.write(pos);
      delay(2);
    }
    delay(80);

    for (int pos = alvoEsquerda; pos <= posicaoCentral; pos += 3) {
      meusOlhos.write(pos);
      delay(10);
    }
    delay(50);

    minhasPalpebras.write(posicaoFechada);
    delay(60); 
    minhasPalpebras.write(posicaoBrava);   
    delay(50);

    minhasPalpebras.write(posicaoFechada);
    delay(60); 
    minhasPalpebras.write(posicaoBrava);   
    delay(50);

    minhasPalpebras.write(posicaoFechada);
    delay(60); 
    minhasPalpebras.write(posicaoBrava);   
    delay(50);
  
    
    if (repeticao == 0) {
      delay(300); 
    }
  }
}
