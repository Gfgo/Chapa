#include "BluetoothSerial.h"                      //Envio por BLE de Solicitud de apertura 
#define vel 2                                     //ESP-WROOM-32 Pin del sensor detector de las interrupciones D2 con Res=1k


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

//Variables
byte opc=0;                                  //Seleccion de opcion
String datoSerEve = "";  
bool findatoSerEve = false;                  //Si el string esta completo (SerEve)

int seg=0;
double velo=0;
volatile int  cont = 0; 




void setup() {
/*SerialBT.begin(115200);                   // inicio bluetooth
  SerialBT.begin("ControlAcceso");          // Nombre dispositivo
  SerialBT.println("Conexion disponible");*/
  Serial.begin(115200);                     // inicio serial
  Serial.println("Conexion disponible");
  datoSerEve.reserve(200);                      //Guardo 200 bytes para datos de llegada
  pinMode(vel, INPUT);                          //RPM
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
        for (int i=0;i<=20;i++){                //AQUI ------>>>>>Cierro puerta<<<-------
//--------------------------------------------------------------------------


//-----------------------------------------------------------------------------          
            }
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
