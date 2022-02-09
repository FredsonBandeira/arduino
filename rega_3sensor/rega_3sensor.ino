

const int analogInPin = A0;  // nome do pino ligado na porta analogica zero
int valorSensor = 0;        // Essa variavel inicia com valor zero
int valorSaida = 0;        // Essa variavel inicia com valor zero
int led_v = 7;                // nome do pino ligado na porta digital 7
int buzzer = 10;            // nome do pino ligado na porta digital 6
int led_A = 6;
int led_V=5;               // nome do pino ligado na porta 5
int led_chuva= 3;          // led para detetar se está chovendo ou não


int bomba=8;  
#define pino_sinal_analogico A1
int valor_analogico;

#include "DHT.h"
#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11


DHT dht(DHTPIN, DHTTYPE);



const int pinoSensor = 12; //PINO DIGITAL UTILIZADO PELO SENSOR d chuva

#include <Wire.h>
#include <LiquidCrystal_I2C.h>//biblioteca I2C
LiquidCrystal_I2C lcd(0x27,16,2);//Seta endereço do display 0x27, para 16 caracteres e 2 linhas o display

 
 // bomba de agua

 
void setup() 
{ 
  lcd.init(); //inicializa o Lcd
  lcd.backlight();
  
  Serial.begin(9600); // Inicializa o Monitor Serial porta 9600
  pinMode(pino_sinal_analogico, INPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(led_v,OUTPUT);
  pinMode(led_A,OUTPUT);
  pinMode(led_V,OUTPUT);
  pinMode(bomba,OUTPUT);
  

  pinMode(pinoSensor, INPUT); //DEFINE O PINO COMO ENTRADA sensor chuva
  pinMode(led_chuva, OUTPUT); //DEFINE O PINO COMO SAÍDA    
  digitalWrite(led_chuva, LOW); //LED INICIA DESLIGADO

  

  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  dht.begin();
}

void loop() {
  delay(1000);
    if(digitalRead(pinoSensor) == LOW){ //SE A LEITURA DO PINO FOR IGUAL A LOW, FAZ
      digitalWrite(led_chuva, HIGH); //ACENDE O LED
       // Serial.println("chuva : Sim");
       lcd.clear();
       lcd.print("CH:Sim");
          delay(500);
        
  }else{ //SENÃO, FAZ
    digitalWrite(led_chuva, LOW);
    lcd.clear();
     lcd.print("CH:Nao");
      //Serial.println("chuva: Não");
  //APAGA O LED
  }


  
  // Ler o valor da porta e armazenar na variavel chamada (sensor value)
  valorSensor = analogRead(analogInPin);
   valor_analogico = analogRead(pino_sinal_analogico);
  //Mostra o valor da porta analogica no serial monitor
 
  //Serial.print("Solo: ");
  //Serial.println(valor_analogico);
  if(valor_analogico<450){
      lcd.setCursor(8,0);
      lcd.print("Solo:Mo");
         }

    else if ( valor_analogico>450 &&  valor_analogico<1012){
      lcd.setCursor(8,0);
      lcd.print("Solo:Sc");
        }
    else {// Check if any reads failed and exit early (to try again).
      lcd.setCursor(8,0);
      lcd.print("Solo:Off");
        }
         
  
  // Função map , ele pega o valor que vai de 0 até 1023 e converter proporcionalmente em porcentagem
  //valorSaida = map(valorSensor, 0, 500, 0, 100);

    // Wait a few seconds between measurements.


  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();


  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
   lcd.setCursor(0,1);
   lcd.print("H:");
   lcd.print(h,1);
   lcd.setCursor(7,1);
   lcd.print("T:");
   lcd.print(t,1);

  

  if (valorSensor < 200 || valor_analogico > 1018 || (valor_analogico> 100 && valor_analogico <= 300) || ( h > 81 && t > 29) || digitalRead(pinoSensor) == LOW ) // Porcentagem de agua 0 (sem agua) , 100 (muita agua) .
  {
    digitalWrite(led_v,HIGH); // tanque esta vazio 
    digitalWrite(led_A,LOW); // tanque esta vazio 
    digitalWrite(led_V,LOW); // tanque esta vazio 
    digitalWrite(bomba,LOW); // bomba fechada 
    //digitalWrite(pinoSensor, LOW);
   // delay(5000);
  }

   if (valorSensor > 200 && valorSensor <= 380 && (valor_analogico >= 970 && valor_analogico <1018) && ((h >= 50 && h <= 80) && ( t >= 24 && t <= 28)) || digitalRead(pinoSensor) == HIGH) // TANQUE ESTA MEDIO.  
   {
    digitalWrite(led_v,LOW); // tanque esta MEDIU
    digitalWrite(led_A,HIGH); // tanque esta MEDIU 
    digitalWrite(led_V,LOW); // tanque esta MEDIU 
    digitalWrite(bomba,HIGH); // bomba ABERTA
    
    delay(10000);
  }
  if (valorSensor > 200 && (valor_analogico > 300 && valor_analogico <=969) && ((h >= 50 && h <= 80) && ( t >= 24 && t <= 28))) // TANQUE ESTA CHEIO.
  {
    digitalWrite(led_v,LOW); // tanque esta CHEIO
    digitalWrite(led_A,LOW); // tanque esta CHEIO 
    digitalWrite(led_V,HIGH); // tanque esta CHEIO  
    digitalWrite(bomba,HIGH); // bomba ABERTA
    delay(10000);
  }

  // Valores no monitor serial
  Serial.print("sensor = ");
  Serial.print(valorSensor);

    float perct=((valorSensor/380.0)*100);  
  
  Serial.print("\t percentagem = ");
  Serial.println(perct);



  // espera 1 segundo para o proximo loop
  //delay(1000);

  //Serial.print(F("Humidity: "));
  //Serial.print(h);
  //Serial.print(F("%  Temperature: "));
  //Serial.print(t);
  //Serial.println(F("°C "));

  
  


  
  






}
