#include <ESP32SharpIR.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN       5                              //Pin control led
#define NUMpixel 12                              //Pixeles de la tira

Adafruit_NeoPixel pixel = Adafruit_NeoPixel(NUMpixel, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500                                  

#include "BluetoothSerial.h"                      //Envio por BLE de Solicitud de apertura 
#include <Servo.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
Servo servo;                                 //Objeto servo
Servo servo2;

//Variables
byte opc=0;                                  //Seleccion de opcion
String datoSerEve= "";  
bool findatoSerEve= false;                  //Si el string esta completo (SerEve)

int pos=0;                                   // variable de posición

byte buser=18;                               //control buzzer
byte sharp=19; 


void setup() {
  Serial.begin(115200);
  SerialBT.begin(115200);                   // inicio bluetooth
  SerialBT.begin("ControlAcceso");          // Nombre dispositivo
  SerialBT.println("Conexion disponible");
  datoSerEve.reserve(200);                      //Guardo 200 bytes para datos de llegada
  servo.attach(15);                             //Asigna pin 15 al objeto servo
  servo2.attach(4);                             //Asigna pin 2 al objeto servo2
  pixel.begin();                                //Inicia leds
  pixel.setBrightness(20);
  pixel.show();
  pinMode(buser, OUTPUT);
  pinMode(19, INPUT);
}
void loop() {
  SerialBT.println("Seleccione opcion (1) Abrir o (2) Cerrar");      //Pregunta por opcion
  serialEvent();
  opc=datoSerEve.toInt();
  Serial.println(opc);

    uint16_t value = analogRead (sharp);
    uint16_t range = get_gp2d12 (value);
    Serial.println (value);
    Serial.print (range);
    Serial.println (" mm");
    Serial.println ();
    delay (500);

  switch (opc) {                                //seleccion de caso segun usuario
      case 1:
//-------------------------------------------------------------------------AQUI ------>>>>>Abro puerta<<<-------
        SerialBT.println("Abriendo Puerta" );
        for(uint16_t i=0; i<pixel.numPixels(); i++) {
            pixel.setPixelColor(i, 255, 255,  0);//amarillo
            pixel.show();
          }
        for (pos=0; pos<=180; pos++) { // va de 0 a 180 grados
                                              // en pasos de 1 grado
          servo.write(pos);                   // servo va a posición pos
          digitalWrite(buser, HIGH);                //Alarma
          delay(15);                          // espera 15ms para ir a la posición
        }
        for (pos=180; pos>=0; pos--) { // va de 180 a 0 grados
          servo2.write(pos);                   // servo va a posición pos
          delay(15);                          // espera 15ms para ir a la posición
        }
//        for(uint16_t i=0; i<pixel.numPixels(); i++) {
//            pixel.setPixelColor(i, 0, 255,  0);//verde
//            pixel.show();
//            delay(10);
//        }
       digitalWrite(buser, LOW);        
//-----------------------------------------------------------------------------          
          opc=0;                //limpiar el dato
          datoSerEve="";          
          findatoSerEve= false;
          SerialBT.println("Puerta Abierta");
          break;
      case 2:                             
//------------------------------------------------------------------AQUI ------>>>>>Cierro puerta<<<-------
        SerialBT.println("Cerrando Puerta");
        for(uint16_t i=0; i<pixel.numPixels(); i++) {
            pixel.setPixelColor(i, 255, 255,  0);//amarillo
            pixel.show();
        }
        for (pos=180; pos>=0; pos--) { // va de 180 a 0 grados
                                              // en pasos de 1 grado
          servo.write(pos);                   // servo va a posición pos
          digitalWrite(buser, HIGH);                //Alarma
          delay(15);                          // espera 15ms para ir a la posición
        }
        for (pos=0; pos<=180; pos++) { // va de 0 a 180 grados
          servo2.write(pos);                   // servo va a posición pos
          delay(15);                          // espera 15ms para ir a la posición
        }
//        for(uint16_t i=0; i<pixel.numPixels(); i++) {
//            pixel.setPixelColor(i, 255, 0,  0);//rojo
//            pixel.show();
//        delay(10);
//        } 
        digitalWrite(buser, LOW);      
//-----------------------------------------------------------------------------
        opc=0;                  //limpiar el dato
        datoSerEve="";          
        findatoSerEve= false;
       SerialBT.println("Puerta Cerrada");
        break;
      default:
        Serial.println("Seleccion no valida");
        delay(2000);
        opc=0;                  //limpiar el dato
        datoSerEve="";          
        findatoSerEve= false;
        break;
  }
}

//--------------------------------------------------->>Esperar datos de ususario---------------------------
void serialEvent() {                        // Funcion para esperar datos entrados por usuario
while (SerialBT.available()) {            //Espera por el buffer de datos
  char inChar = (char)SerialBT.read();    //Almacena dato entrante (serial normal)
    datoSerEve=inChar;                         //Almacena el dato local en variable global
    if (inChar == '\n') {                   //Si el dato que viene es nueva linea lo pone en variable para el loop
      findatoSerEve= true;
    }
  }
}
//--------------------------------------------------->>Esperar datos de ususario---------------------------

uint16_t get_gp2d12 (uint16_t value) {
    if (value < 10) value = 10;
    return ((67870.0 / (value - 3.0)) - 40.0);
}
