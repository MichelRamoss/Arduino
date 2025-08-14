//Proyecto Final
//Michel A Ramos Soto 
//Diplomado 2021 //192.168.1.78 //Revisado 2025

#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>
const int mqtt_port = 1883;
const char *mqtt_user = "FnJ939HUTm6Hr2F";
const char *mqtt_pass = "2EizPNpWjedWT8P";

// Credenciales MQTT
const char *mqtt_server = "ioticos.org";
const char *Raiz_Sus = "cOdVYg5FaZ2hHtj";
const char *Raiz_Hab1_Sus= "cOdVYg5FaZ2hHtj/Hab1";
const char *Raiz_Hab1_Led_Sus= "cOdVYg5FaZ2hHtj/Hab1/Led";
const char *Raiz_Hab1_Tem_Sus= "cOdVYg5FaZ2hHtj/Hab1/Tem";
const char *Raiz_Hab1_Vent_Sus= "cOdVYg5FaZ2hHtj/Hab1/Vent";
const char *Raiz_Hab2_Mov_Sus= "cOdVYg5FaZ2hHtj/Hab2/Mov";
const char *Raiz_Pas_Luz_Sus= "cOdVYg5FaZ2hHtj/Pas/Luz";
const char *Raiz_Pas_Bom_Sus= "cOdVYg5FaZ2hHtj/Pas/Bom";

// Credenciales de WiFi
const char* ssid     = "INFINITUM3992";
const char* password = "zDP5aBLn7J";

// Variables Globales
WiFiClient espClient;
PubSubClient client(espClient);
char msg[25];
char msg2[25];
char msg3[25];
char msg4[25];
long count=0;
int contconexion = 0;

// Funciones
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();
void setup_wifi();

//Sensor de temperatura
const int P_Sensor=4;
DHT DHT1(P_Sensor, DHT11); // Crea dht1, que es un sensor DHT11 conectado

//Sensor de movimiento
const int M_Sensor = 21;                           // Asigna el pulsador al pin 4
const int MLed = 23;
bool Mov = LOW;                              // Variable para guardar el estado del pin 2

//Sensor Luz
const int P_Ldr = 39;
int AdcLuz=0;

void setup() {
  Serial.begin(115200);
  DHT1.begin();         // Inicializa el sensor DHT11
  pinMode(M_Sensor, INPUT);             
  pinMode(MLed, OUTPUT);  
  pinMode(18, OUTPUT);  //Led Habitacion 1
  digitalWrite(18, LOW);
  pinMode(15, OUTPUT);   //Ventilador  Habitacion 1
  digitalWrite(15, LOW);
  pinMode(22, OUTPUT);   //Bombilla Pasillo
  digitalWrite(22, LOW);
  // Configuración conexión WIFI
  setup_wifi();
  // Configuración conexión MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  LeerDHT1();  // Ejecuta la función LeerDTH1 
  // Se pregunta si el cliente está conectado
  if (!client.connected()) {
    // Si no está conectado se realiza la reconexión
    reconnect();
  }

  if (client.connected()){

  //Temperatura
  float t1=DHT1.readTemperature();    // Lee la temperatura en la variable t1
  float h1=DHT1.readHumidity();       // Lee la humedad en la variable h1
  while (isnan(t1) || isnan(h1)){     // Verifica que la lectura no sea fallida
    Serial.println("Lectura fallida");// Mensaje de lectura fallida
    delay(1000);                      // Retardo de 1000 msegundos
    t1=DHT1.readTemperature();        // Lee nuevamente la temperatura
    h1=DHT1.readHumidity();           // Lee nuevamente la humedad
  }
  Serial.println("Temperatura:");     // Imprime Mensaje
  Serial.println(t1);                 // Imprimera valor de temperatura
  Serial.println("Humedad:");         // Imprime mensaje
  Serial.println(h1); 

    String Tem = String(count);
    Tem = String(t1);
    Tem.toCharArray(msg,25);
    client.publish(Raiz_Hab1_Tem_Sus,msg);
    
    count++;
    
  }
  
  // Movimiento
  Mov = digitalRead(M_Sensor);               
  if (Mov==HIGH){
  Serial.println("Hay movimiento"); 
  digitalWrite(MLed, HIGH);
  }
  else{
    Serial.println("No hay movimiento"); 
    digitalWrite(MLed, LOW);   
  } 
  String Movimiento = String(count);
  Movimiento = String(Mov);
  Movimiento.toCharArray(msg2,25);
  client.publish(Raiz_Hab2_Mov_Sus,msg2); 

  //Medicion de Luz
  AdcLuz = analogRead(P_Ldr);     // Mide el Adc y lo guarda AdcLuz
  Serial.println("Medición Luz:");// Imprime mensaje
  Serial.println(AdcLuz);         // Imprime valor
  //Bombilla del pasillo 
  if (AdcLuz<300){
  Serial.println("Prende Bombilla"); 
  digitalWrite(22, HIGH);
  int BP = 0;
  String BombillaPrendida = String(count);
  BombillaPrendida = String(BP);
  BombillaPrendida.toCharArray(msg4,25);
  client.publish(Raiz_Pas_Bom_Sus,msg4); 
  }
  else{
    Serial.println("Apaga Bombilla"); 
    digitalWrite(22, LOW);  
  int BA = 1;
  String BombillaApagada = String(count);
  BombillaApagada = String(BA);
  BombillaApagada.toCharArray(msg4,25);
  client.publish(Raiz_Pas_Bom_Sus,msg4);
  } 
  //Publish
  String luz = String(count);
  luz = String(AdcLuz);
  luz.toCharArray(msg3,25);
  client.publish(Raiz_Pas_Luz_Sus,msg3); 

  client.loop();
  delay(1000);
}

void LeerDHT1(){
  float t1=DHT1.readTemperature();    // Lee la temperatura en la variable t1
  float h1=DHT1.readHumidity();       // Lee la humedad en la variable h1
  while (isnan(t1) || isnan(h1)){     // Verifica que la lectura no sea fallida
    Serial.println("Lectura fallida");// Mensaje de lectura fallida
    delay(1000);                      // Retardo de 1000 msegundos
    t1=DHT1.readTemperature();        // Lee nuevamente la temperatura
    h1=DHT1.readHumidity();           // Lee nuevamente la humedad
  }
  Serial.println("Temperatura:");     // Imprime Mensaje
  Serial.println(t1);                 // Imprimera valor de temperatura
  Serial.println("Humedad:");         // Imprime mensaje
  Serial.println(h1);                 // Imprime valor de humedad
}

//Conexión WiFi
void setup_wifi(){
  delay(10);
  delay(10);
  // Conexión WIFI
  Serial.println();
  Serial.print("Conectando a ssid: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED and contconexion <50) { 
    ++contconexion;
    delay(500);
    Serial.print(".");
  }
  if (contconexion <50) {
      Serial.println("");
      Serial.println("WiFi conectado");
      Serial.println(WiFi.localIP());
  }
  else { 
      Serial.println("");
      Serial.println("Error de conexion");
  }
}

// Conexión MQTT
void reconnect() {
  // Si no hay conexión se entra dentro del ciclo
  while (!client.connected()) {
    Serial.println("Intentando conexión Mqtt...");
    // Se crea un cliente ID
    String clientId = "DIoT_Cliente";
    clientId += String(random(0xffff), HEX);
    // Se intenta realizar l aconexión conectar
    if (client.connect(clientId.c_str(),mqtt_user,mqtt_pass)) {
      // Si hay conexión se imprime el mensaje
      Serial.println("Conectado con el Broker!");
      // Nos suscribimos al tópico Raíz/Piso3
      if(client.subscribe(Raiz_Sus)){
        Serial.print("Suscripcion --");
        Serial.println(Raiz_Sus);
        Serial.println(" -- ok");
      }else{
        Serial.println("fallo Suscripciión a --");
        Serial.println(Raiz_Sus);
      }
      // Nos suscribimos al tópico Raíz/Habitacion1/Vent
      if(client.subscribe(Raiz_Hab1_Vent_Sus)){
        Serial.print("Suscripcion --");
        Serial.println(Raiz_Hab1_Vent_Sus);
        Serial.println(" -- ok");
      }else{
        Serial.println("fallo Suscripciión a --");
        Serial.println(Raiz_Hab1_Vent_Sus);
      }
      // Nos suscribimos al tópico Raíz/Habitacion1/Led
      if(client.subscribe(Raiz_Hab1_Led_Sus)){
        Serial.print("Suscripcion --");
        Serial.println(Raiz_Hab1_Led_Sus);
        Serial.println(" -- ok");
      }else{
        Serial.println("fallo Suscripciión a --");
        Serial.println(Raiz_Hab1_Led_Sus);
      }
    } else {
      // Si no hay conexión se imprime el mensaje, el estado de conexión
      // y se intenta nuevamente dentro de 5 segundo
      Serial.print("falló :( con error -> ");
      Serial.print(client.state());
      Serial.println(" Intentamos de nuevo en 5 segundos");
      delay(5000);
    }
  }
}

// Función para cuando se recibe un mensaje por parte del broker de un tópico al
// cual se está subscrito
void callback(char* topic, byte* payload, unsigned int length){
  String incoming = "";
  Serial.print("Mensaje recibido desde -> ");
  Serial.print(topic);
  Serial.println("");
  for (int i = 0; i < length; i++) {
    incoming += (char)payload[i];
  }
  incoming.trim();
  Serial.println("Mensaje -> " + incoming);

   if (strcmp(topic,Raiz_Hab1_Vent_Sus)==0)
   {
    int VentHab1 = incoming.toInt();
    if(VentHab1==1)
    {
      digitalWrite(15, HIGH);
    }
    else
    {
      digitalWrite(15, LOW);
    }
   }

  /*Serial.print("Mensaje recibido desde -> ");
  Serial.print(topic);
  Serial.println("");
  for (int i = 0; i < length; i++) {
    incoming += (char)payload[i];
  }
   incoming.trim();
   Serial.println("Mensaje -> " + incoming);*/
   if (strcmp(topic,Raiz_Hab1_Led_Sus)==0)
  {
    int LedHab1 = incoming.toInt();
    if(LedHab1==1)
    {
      digitalWrite(18, HIGH);
    }
    else
    {
      digitalWrite(18, LOW);
    }
   }

  /*if (strcmp(topic,Raiz_Hab2_Mov_Sus)==0){
  Movimiento = digitalRead(P_Sensor);                
  if (Movimiento==HIGH){
    Serial.println("Hay movimiento"); 
  digitalWrite(Led, HIGH);
  }
  else{
    Serial.println("No hay movimiento"); 
    digitalWrite(Led, LOW);  
  } 
  } */
} 
