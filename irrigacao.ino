//Inclusão das bibliotecas
#include <Wire.h>
#include <LiquidCrystal.h>

// declarações do LCD
const int RS=7, EN=6, D4=5, D5=4, D6=3, D7=2;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

// Declarações do Sensor de Umidade
const int sensor_umi = A0;           // pino analogico conectado ao sensor
int leitura_sensor = 0;              // variavel que armazena as leituras do sensor
const int numero_amostras = 100;     // quantidade de amostras para coletar
long somatoria = 0;                  // variavel que armazena soma das leituras
const int rele = 8;                  // pino conectado ao relé
const int valor_maximo = 634;        // valor com solo seco
const int valor_minimo = 304;        // valor com solo umido
bool pausa = true;                   // variavel auxiliae para o delay de 10 segundos
// variaveis que armazenam as concentrações desejadas
const int concentracao_minima = 30;  
const int concentracao_maxima = 50; 

void setup(){

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print(" Monitoramento de");
  lcd.setCursor(0, 1);
  lcd.print("Umidade do Solo");
  delay(5000);
  lcd.clear();
  
  Serial.begin(9600);
  Serial.println("Sensor de Umidade de Solo Capacitivo - Primeiros passos");
  Serial.println("Coletando media de leituras do sensor...");
  Serial.println("");

  pinMode(sensor_umi, INPUT);
  pinMode(rele, OUTPUT);
  digitalWrite(rele, LOW);
  
  // realiza a leitura do sensor de acordo com a varial "numero_amostras"
  for(int i=1; i<=numero_amostras; i++){
    leitura_sensor = analogRead(sensor_umi);
    somatoria = somatoria + leitura_sensor;
    float tensao = leitura_sensor*(5.0/1023); // converte a leitura para tensao para referencia
    Serial.print("Amostra ");
    Serial.print(i);
    Serial.print("  |  ");
    Serial.print("Leitura: ");
    Serial.print(leitura_sensor);
    Serial.print("  |  ");
    Serial.print("Tensão: ");
    Serial.println(tensao);
    delay(1000);
    
  }

  int media = somatoria / numero_amostras;
  Serial.println("");
  Serial.print("Media obtida: ");
  Serial.println(media);
}

void loop(){
  leitura_sensor = analogRead(sensor_umi);
  // leitura do sensor, mapeia entre 0 e 100% e mostra o valor no lcd
  leitura_sensor = map(leitura_sensor, valor_minimo, valor_maximo, 100, 0);
  lcd.print("Umidade = ");
  lcd.print(leitura_sensor);
  lcd.print(" %");

  // verificação da leitura para conferir a concentração minima desejada 
  if(leitura_sensor < concentracao_minima){ 
    // Se sim, 
    digitalWrite(rele, HIGH);    // aciona o rele
    pausa = false;
    // Se sim, informa pelo LCD que precisa irrigar
    lcd.setCursor(0,1);
    lcd.print("Precisa Irrigar!");
    // talvez colocar um delay aq
    //lcd.noBacklight();
    //delay(500);
    //lcd.backlight();
    //delay(500);
  }
  else if (leitura_sensor > concentracao_maxima){
    // Se não
    digitalWrite(rele, LOW);     // rele se mantem desligado
    pausa = true;
    delay(5000);
  }
  if(pausa){
    delay(10000);  // aguarda 10 seg para próxima leitura
  }
  lcd.clear();
}