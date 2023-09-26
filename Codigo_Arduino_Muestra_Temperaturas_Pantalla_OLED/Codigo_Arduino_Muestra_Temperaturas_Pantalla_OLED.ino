#define __DEBUG__
 
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include <Thermistor.h> //BIBLIOTECA
 
// Definir constantes
#define ANCHO_PANTALLA 128 // ancho pantalla OLED
#define ALTO_PANTALLA 64 // alto pantalla OLED
 
// Objeto de la clase Adafruit_SSD1306
Adafruit_SSD1306 display(ANCHO_PANTALLA, ALTO_PANTALLA, &Wire, -1);

char c;
int a;

//Variable de tipo Thermistor, PIN ANALÓGICO (A3) es adonde el sensor se conecta
Thermistor temp(3);

//Variables y Pines usados para los Leds
int ledVerde = 6;
int ledRojo = 7;

//Variables y Pines usados para la sirena
int Sirena = 8;                  //Pin de la sirena
int Cuarto = 1000/4;             //Tiemo de 1/4 de nota
int Octavo = 1000/8;             //Tiempo de 1/8 de nota
double pausa = 1.30;             //Pausa del 30% de la nota

int N1 = 264;                    //Frecuencia de la nota 1 es de 264 Hz
int N2 = 440;                    //Frecuencia de la nota 2 es de 440 Hz
int N3 = 669;                    //Frecuencia de la nota 3 es de 669 Hz
 
void setup() {
#ifdef __DEBUG__
  Serial.begin(9600);
  delay(6000); //Lectura cada 6 segundos
  Serial.println("Iniciando pantalla OLED");
#endif

// Iniciar pantalla OLED en la dirección 0x3C
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
#ifdef __DEBUG__
    Serial.println("No se encuentra la pantalla OLED");
#endif
    while (true);
  }

  pinMode(6, OUTPUT);          //Pin 7 salida
  pinMode(7, OUTPUT);          //Pin 8 salida
  {
    pinMode(Sirena, OUTPUT);   //Sirena será un Pin de salida
  }
  
 
}
 
void loop() {
  float temperature = temp.getTemp(); //Variable decimal (float)
  Serial.print("Temperatura: "); //Imprime texto en el Monitor
  Serial.print(temperature); //Imprime la temperatura sensada
  Serial.println("*C");

  // Limpiar buffer
  display.clearDisplay();
 
  // Tamaño del texto
  display.setTextSize(1);
  // Color del texto
  display.setTextColor(SSD1306_WHITE);
  // Posición del texto
  display.setCursor(10, 32);

  display.print(" Temp: "); //Muestra la temperatura sensada
  display.print(temperature);
  display.println(" *C");

 
  // Enviar a pantalla
  display.display();

  delay(6000); //Lectura cada 6 segundos

  // Mover texto de izquierda a derecha
  display.startscrollright(0x00, 0x0F);
  delay(15000);
  display.stopscroll();

  if(Serial.available()){
    c=Serial.read();
    while (c!='d'){
      a=analogRead(0);
      Serial.println(a);
      delay(100);
      c=Serial.read();
      if (c=='d'){
        return;
      }
    }
  }

  if (temperature >=25){
  digitalWrite(7, HIGH);               //si la temperatura es mayor a 25ºC Led rojo prende
  digitalWrite(6, LOW);                //si la temperatura es mayor a 25ºC Led verde se apaga
  }

  
  else if (temperature <=21){
    digitalWrite(7, LOW);               //si la temperatura es menor o igual a 25ºC Led rojo apaga
    digitalWrite(6, HIGH);                //si la temperatura es menor o igual a 25ºC Led verde se prende

    tone(Sirena, N1, Cuarto);
    delay(Cuarto*pausa);
     tone(Sirena, N2, Octavo);
    delay(Octavo*pausa);
     tone(Sirena, N3, Octavo);
    delay(Octavo*pausa);
  }

}