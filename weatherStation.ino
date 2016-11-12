#include <Arduino.h>



//Weather Station
//Script de obtención de temperatura, humedad y luminosidad de distintos sensores.
//DHT22 y LDR

/*El circuito dispone de 3 leds que se encenderán dependiendo de si se llegan a los
límites indicados para cada uno de los valores.
Por otro lado también dispondrá de un LCD donde se irán mostrando los valores.*/

//Importamos las librerias necesarias
#include <DHT.h>
#include <LiquidCrystal.h>
//Definimos pin y tipo de DHT
#define DHTPIN 9
#define DHTTYPE DHT22
//Inicializamos el sensor con los valores definidos.
DHT dht (DHTPIN, DHTTYPE);
//Creamos las variables necesarias para el control de la fotocelda LDR.
int pinFotocelda = A0;
float luminosidad = 0;
//Inicializamos el LCD.
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//Creamos las variables para los leds
int ledTemperatura = 6;
int ledHumedad = 7;
int ledEncendido = 13;
int pos1 = 0;
int pos2 = 1;
int pos3 = 2;
//Setup de Arduino
void setup(){
 //Indicamos los pines de los leds
 pinMode(ledTemperatura, OUTPUT); //Pin Temperatura
 pinMode(ledHumedad, OUTPUT); //Pin Humedad
 pinMode(ledEncendido, OUTPUT); //Pin Encendido
 //Inicializamos sensor DHT.
 dht.begin();
 //Inicializamos Serial.
 Serial.begin(9600);
 Serial.println("Weather Station v1.0");

 //Inicializamos LCD.
 lcd.begin(16, 2);
 lcd.setCursor(0, 1);
 lcd.print("Weather Station v1.0");
 //Damos 3 segundos para mostrar el primer mensaje por LCD y al sensor para capturar valores.
 delay(3000);
}
//Fin Setup
//Bucle principal Arduino
void loop(){
  //Encendemos el led de ON/OFF
  digitalWrite(ledEncendido, HIGH);
  //Si no capturamos datos, mostramos error.
  if (isnan(lecturaTemperatura()) || isnan(lecturaHumedad())){
    digitalWrite(ledTemperatura, HIGH);
    digitalWrite(ledHumedad, HIGH);
    Serial.println("Error: Captura DHT.");
    lcd.setCursor(0, 0);
    lcd.print("Error: Captura DHT.");
  }
  //Si capturamos datos
  else{
    //Si la temperatura supera el límite establecido, encenderá el led correspondiente.
    if (lecturaTemperatura() > 26 || lecturaTemperatura() < 15){
      digitalWrite(ledTemperatura, HIGH);
    } else {
      digitalWrite(ledTemperatura, LOW);
    }
    //Si la temperatura supera el límite establecido, encenderá el led correspondiente.
    if ((lecturaHumedad() > 60) || (lecturaHumedad() < 40)){
      digitalWrite(ledHumedad, HIGH);
    } else {
      digitalWrite(ledHumedad, LOW);
    }
    //Obtenemos datos de la fotocelda.
    luminosidad = analogRead(pinFotocelda);
    //Escribimos en el Serial la información
    Serial.print("Temp: ");
    Serial.print(lecturaTemperatura());
    Serial.println(" ºC.");
    Serial.print("Hum: ");
    Serial.print(lecturaHumedad());
    Serial.println("%.");
    Serial.print("Luz: ");
    Serial.print(luminosidad);
    Serial.println(".");
    Serial.println(" ");
    Serial.println(" ");
    Serial.println(" ");

    actualizaLcd();
    //########FALTA IMPLEMENTAR EL MOSTRAR ILUMINACIÓN EN EL LCD########
    delay(4000);
  }
}
//Fin bucle principal
void actualizaLcd() {
    lcd.clear();
 //Escribimos en el lcd la información
    printLcd(pos1, "Temp: ", lecturaTemperatura(), "C.");
    printLcd(pos2, "Hum:  ", lecturaHumedad(), "%.");
    printLcd(pos3, "Luz:  ", luminosidad/1023*100, "%.");
    pos1 = varPlus(pos1);
    pos2 = varPlus(pos2);
    pos3 = varPlus(pos3);
}
void printLcd(int posicion, String orden, float valor, String unidad) {
    if(posicion!=2) {
      lcd.setCursor(0, posicion);
      lcd.print(orden);
      lcd.print(valor);
      lcd.print(unidad);
    }
}
int varPlus(int contador){
    int salida = contador-1;
    if (salida <0) salida=2;
    return salida;
}
//Función para capturar temperatura.
float lecturaTemperatura(){
  float temp = dht.readTemperature();
  return temp;
}
//Función para caputat humedad.
float lecturaHumedad(){
  float hum = dht.readHumidity();
  return hum;
}
