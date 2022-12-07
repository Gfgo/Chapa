//#include <SoftwareSerial.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN      5                              //Pin control led
#define NUMpixel 8                             //Pixeles de la tira


Adafruit_NeoPixel pixel = Adafruit_NeoPixel(NUMpixel, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500                                  

#include "BluetoothSerial.h"                      //Envio por BLE de Solicitud de apertura 
#include "ESP32Servo.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
Servo servo;                                 //Objeto servo
Servo servo2;

//Variables
const byte paso=15;
int pos=0;                                   // variable de posición

byte buser=18;                               //control buzzer
byte pir=19;
byte indicator = 5; 

String dato = "";  
bool findato = false;                                                           // si el string esta completo
int a=0;                                                                        //Seleccion de opcion

void setup() {
  Serial.begin(115200);
  SerialBT.begin(115200);                   // inicio bluetooth
  SerialBT.begin("ControlAcceso");          // Nombre dispositivo
  SerialBT.println("Conexion disponible");
  servo.setPeriodHertz(50);
  servo.attach(32);                             //Asigna pin 15 al objeto servo
  servo2.setPeriodHertz(50);
  servo2.attach(33);                             //Asigna pin 4 al objeto servo2
  servo.write(0);
  servo2.write(180);
  pixel.begin();                                //Inicia leds
  pixel.setBrightness(20);
  pixel.show();
  pinMode(buser, OUTPUT);
  pinMode(pir,INPUT);
  pinMode(indicator,OUTPUT);
  dato.reserve(200); 
  //pinMode(21,INPUT);
}
void loop() {
  Serial.println("Seleccione opcion (1) Abrir o (2) Cerrar");      //Pregunta por opcion
  SerialBT.println("Seleccione opcion (1) Abrir o (2) Cerrar");
  if(SerialBT.available()){
  dato = SerialBT.readString();
  a=dato.toInt();
  }
      byte state = digitalRead(pir);
      digitalWrite(indicator,state);            //Led on-board esp 32
      if(state == 1)Serial.println("Se detecta personal!!!!!!!!!!! ");
      else if(state == 0)Serial.println("No se detecta ");
      delay(500);


    if ((a==1)&&((servo.read())==0)) //ESTA CERRADA VOY A ABRIR
    {
//-------------------------------------------------------------------------AQUI ------>>>>>Abro puerta<<<-------
        Serial.println("Abriendo Puerta" );
        for(uint16_t i=0; i<pixel.numPixels(); i++) {
            pixel.setPixelColor(i, 255, 255, 0);//amarillo
            pixel.show();
          }
        for (pos=0; pos<=180; pos++) { // va de 0 a 180 grados
                                              // en pasos de 1 grado
          servo.write(pos);                   // servo va a posición pos
          digitalWrite(buser, HIGH);                //Alarma
          delay(paso);                          // espera 15ms para ir a la posición
        }
        for (pos=180; pos>=0; pos--) { // va de 180 a 0 grados
          servo2.write(pos);                   // servo va a posición pos
          delay(paso);                          // espera 15ms para ir a la posición
        }
        for(uint16_t i=0; i<pixel.numPixels(); i++) {
            pixel.setPixelColor(i, 0, 255,  0);//verde
            pixel.show();
            delay(10);
        }
       digitalWrite(buser, LOW);        
//-----------------------------------------------------------------------------          
       Serial.println("Puerta Abierta");SerialBT.println("Puerta Abierta");
          
          a=0;
          dato="";          //limpiar el dato
          findato = false;
    } 
    else if((a==2)&&((servo.read())==180)) //ESTA CERRADA VOY A ABRIR
    {                           
//------------------------------------------------------------------AQUI ------>>>>>Cierro puerta<<<-------
        Serial.println("Cerrando Puerta");
        for(uint16_t i=0; i<pixel.numPixels(); i++) {
            pixel.setPixelColor(i, 255, 255,  0);//amarillo
            pixel.show();
        }
        for (pos=180; pos>=0; pos--) { // va de 180 a 0 grados
                                              // en pasos de 1 grado
          servo.write(pos);                   // servo va a posición pos
          digitalWrite(buser, HIGH);                //Alarma
          delay(paso);                          // espera 15ms para ir a la posición
        }
        for (pos=0; pos<=180; pos++) { // va de 0 a 180 grados
          servo2.write(pos);                   // servo va a posición pos
          delay(paso);                          // espera 15ms para ir a la posición
        }
        for(uint16_t i=0; i<pixel.numPixels(); i++) {
            pixel.setPixelColor(i, 255, 0,  0);//rojo
            pixel.show();
        delay(10);
        } 
        digitalWrite(buser, LOW);      
//-----------------------------------------------------------------------------
       Serial.println("Puerta Cerrada");SerialBT.println("Puerta Cerrada");
          
          a=0;
          dato="";          //limpiar el dato
          findato = false;
    }
}
