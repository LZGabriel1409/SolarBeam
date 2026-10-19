/*
  SolarBeam - Teste de bancada do rele (GPIO27)

  Comandos pelo Monitor Serial (115200, "Nova linha" ou "Both NL & CR"):
    1  = LIGAR          0  = DESLIGAR
    p  = alternar polaridade (ativo em LOW / ativo em HIGH)
    d  = alternar modo dreno aberto (pino solto para desligar)
    t  = teste automatico: liga 2 s, desliga 2 s, 3 vezes
    s  = mostra a configuracao atual e o nivel real do pino
*/

const int PINO_RELE = 27;

bool ativoEmLow = true;     // comeca igual ao firmware v1.2.0
bool drenoAberto = true;    // comeca igual ao firmware v1.2.0
bool ligado = false;

void escreverRele(bool liga) {
  ligado = liga;

  if (ativoEmLow && drenoAberto) {
    if (liga) {
      pinMode(PINO_RELE, OUTPUT);
      digitalWrite(PINO_RELE, LOW);
    } else {
      pinMode(PINO_RELE, INPUT);   // solto: o modulo puxa para cima
    }
  } else {
    bool nivelAtivo = ativoEmLow ? LOW : HIGH;
    bool nivelInativo = ativoEmLow ? HIGH : LOW;
    pinMode(PINO_RELE, OUTPUT);
    digitalWrite(PINO_RELE, liga ? nivelAtivo : nivelInativo);
  }
}

void mostrarStatus() {
  Serial.println("--- STATUS ---");
  Serial.println(String("Polaridade: ") + (ativoEmLow ? "ativo em LOW" : "ativo em HIGH"));
  Serial.println(String("Dreno aberto: ") + (drenoAberto ? "SIM" : "NAO") +
                 ((drenoAberto && !ativoEmLow) ? " (sem efeito: so vale com ativo em LOW)" : ""));
  Serial.println(String("Comando atual: ") + (ligado ? "LIGAR" : "DESLIGAR"));
  Serial.println(String("Nivel lido no pino: ") + (digitalRead(PINO_RELE) ? "HIGH" : "LOW"));
  Serial.println("--------------");
}

void testeAutomatico() {
  Serial.println("Teste: 3 ciclos de 2 s ligado / 2 s desligado");
  for (int i = 1; i <= 3; i++) {
    escreverRele(true);
    Serial.println("Ciclo " + String(i) + ": LIGADO");
    delay(2000);
    escreverRele(false);
    Serial.println("Ciclo " + String(i) + ": DESLIGADO");
    delay(2000);
  }
  Serial.println("Teste concluido.");
}

void setup() {
  escreverRele(false);
  Serial.begin(115200);
  delay(300);
  Serial.println();
  Serial.println("Teste de rele - GPIO" + String(PINO_RELE));
  Serial.println("Comandos: 1=ligar 0=desligar p=polaridade d=dreno t=teste s=status");
  mostrarStatus();
}

void loop() {
  if (!Serial.available()) return;
  char c = Serial.read();

  switch (c) {
    case '1':
      escreverRele(true);
      Serial.println("-> LIGAR");
      break;
    case '0':
      escreverRele(false);
      Serial.println("-> DESLIGAR");
      break;
    case 'p':
      ativoEmLow = !ativoEmLow;
      escreverRele(false);   // por seguranca volta para desligado
      Serial.println(String("Polaridade agora: ") + (ativoEmLow ? "ativo em LOW" : "ativo em HIGH"));
      break;
    case 'd':
      drenoAberto = !drenoAberto;
      escreverRele(false);
      Serial.println(String("Dreno aberto agora: ") + (drenoAberto ? "SIM" : "NAO"));
      break;
    case 't':
      testeAutomatico();
      break;
    case 's':
      mostrarStatus();
      break;
    default:
      break;   // ignora \r, \n e outros
  }
}
