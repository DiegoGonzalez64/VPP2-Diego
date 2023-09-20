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
  
 
}
 
void loop() {
  float temperature = temp.getTemp(); //Variable entera (int)
  Serial.print("Temperatura: "); //Imprime texto en el Monitor
  Serial.print(temperature); //Imprime la temperatura sensada
  Serial.println("*C");

  // Limpiar buffer
  display.clearDisplay();
 
  // Tamaño del texto
  display.setTextSize(2);
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
}