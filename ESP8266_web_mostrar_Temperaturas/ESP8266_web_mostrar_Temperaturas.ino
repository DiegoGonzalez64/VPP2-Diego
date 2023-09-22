//Escribe en un navegador 192.168.4.1

#define DEBUG true
 
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

float valor = 0;

SoftwareSerial esp8266(9,8); 
// El TX del módulo al terminal 9 del Arduino.
// El RX del módulo al terminal 8 del Arduino.
 
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
  
esp8266.begin(9600); // Importante la velocidad del módulo

sendData("AT+CWJAP=\"router\",\"contraseña\"\r\n", 2000, DEBUG);
delay(5000); // Espera un poco que conecte con el Router.
sendData("AT+CWMODE=3\r\n",1000,DEBUG); // Modo de cliente y servidor.
sendData("AT+CIFSR\r\n",1000,DEBUG); // En el Serial Monitor aparece la IP de cliente y servidor.
sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // Multiples conexiones.
sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // El Puerto web es el 80

 
}
 
void loop() {

valor = analogRead(3);
if(esp8266.available()) // Consulta si el módulo está enviando algún mensaje
{

if(esp8266.find("+IPD,"))
{
delay(1000);

int connectionId = esp8266.read()-48; 
// Aquí las construcción de la página con dos formularios.

String webpage = "<head><meta http-equiv=\"refresh\" content=\"4\"></head>";
// String webpage = "<head></head>";
webpage += "<h1>Temperatura_Actual</h1>";
webpage += "ºC: ";
webpage += valor;

String cipSend = "AT+CIPSEND=";
cipSend += connectionId;
cipSend += ",";
cipSend +=webpage.length();
cipSend +="\r\n";

sendData(cipSend,1000,DEBUG);
sendData(webpage,1000,DEBUG);

// Cierra la conexión
String closeCommand = "AT+CIPCLOSE="; 
closeCommand+=connectionId;
closeCommand+="\r\n";

sendData(closeCommand,3000,DEBUG);
}
}
}

// Función para Enviar datos al Servidor.
String sendData(String command, const int timeout, boolean debug)
{
String response = "";
esp8266.print(command); // Envía la información de command al servidor
long int time = millis();

while( (time+timeout) > millis())
{
while(esp8266.available())
{
// A response van los datos que regresan al servidor.
char c = esp8266.read(); // Va leyendo caracter a caracter.
response+=c; 
} 
}

if(debug)
{
String variable;
Serial.print(response);
} 
return response;

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