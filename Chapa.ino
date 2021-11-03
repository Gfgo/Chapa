#include "BluetoothSerial.h"                      //Envio por BLE de Solicitud de apertura 
#include <Servo.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
Servo servo;                                 //Objeto servo

//Variables
byte opc=0;                                  //Seleccion de opcion
String datoSerEve = "";  
bool findatoSerEve = false;                  //Si el string esta completo (SerEve)

int seg=0;
double velo=0;
volatile int  cont = 0; 
int pos=0;                                   // variable de posición

void setup() {
/*SerialBT.begin(115200);                   // inicio bluetooth
  SerialBT.begin("ControlAcceso");          // Nombre dispositivo
  SerialBT.println("Conexion disponible");*/
  Serial.begin(115200);                     // inicio serial------------------------
  Serial.println("Conexion disponible");
  datoSerEve.reserve(200);                      //Guardo 200 bytes para datos de llegada
  servo.attach(15);                             //Asigna pin 15 al objeto servo
}
void loop() {
//SerialBT.println("Seleccione opcion (1) Abrir o (2) Cerrar");      //Pregunta por opcion
  Serial.println("Seleccione opcion (1) Abrir o (2) Cerrar");
  serialEvent();
  opc=datoSerEve.toInt();
  //Serial.println(datoSerEve);
  switch (opc) {                                //seleccion de caso segun usuario
      case 1:
        SerialBT.println("Cerrando Puerta" );
//-------------------------------------------------------------------------AQUI ------>>>>>Cierro puerta<<<-------

  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
//-----------------------------------------------------------------------------          
          
          opc=0;                //limpiar el dato
          datoSerEve="";          
          findatoSerEve= false;
          SerialBT.println("Fin de medición.");
          break;
      case 2:                             //AQUI ------>>>>>Abro puerta<<<-------
        SerialBT.print("Seleccion vacia");
        delay(2000);
        opc=0;                  //limpiar el dato
        datoSerEve="";          
        findatoSerEve= false;
        break;
      default:
        SerialBT.print("Seleccion no valida");
        delay(2000);
        opc=0;                  //limpiar el dato
        datoSerEve="";          
        findatoSerEve= false;
        break;
  }
}

//--------------------------------------------------->>Esperar datos de ususario---------------------------
void serialEvent() {                        // Funcion para esperar datos entrados por usuario
  while (SerialBT.available()) {              //Espera por el buffer de datos
    char inChar = (char)SerialBT.read();    //Almacena dato entrante (serial normal)
    datoSerEve=inChar;                         //Almacena el dato local en variable global
    if (inChar == '\n') {                   //Si el dato que viene es nueva linea lo pone en variable para el loop
      findatoSerEve= true;
    }
  }
}
//--------------------------------------------------->>Esperar datos de ususario---------------------------
