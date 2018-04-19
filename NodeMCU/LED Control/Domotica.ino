#include <ESP8266WiFi.h>

// Credenciales de acceso a la Wifi
const char* _SSID = "Tech_D0010461";
const char* _PASS = "UUUWXCCH";

// Elementos del circuito
const byte _LED = 13; // GPIO13 (D7)

// Creamos un servidor web (:80)
WiFiServer _SERVER(80);

void setup()
{
  Serial.begin(9600); // Iniciamos la consola a 9600 baudios
  // Iniciamos el LED apagado
  pinMode(_LED, OUTPUT);
  digitalWrite(_LED, LOW);
  // Nos conectamos a la WiFi
  Serial.flush();
  Serial.print("Conectandonos a la WiFi: ");
  Serial.println(_SSID);
  WiFi.begin(_SSID,_PASS);
  while(WiFi.status() != WL_CONNECTED)
    delay(500);
  Serial.println("¡WiFi conectado!");
  // Iniciamos el servidor
  _SERVER.begin();
  Serial.print("¡Server iniciado! URL: http://");
  Serial.println(WiFi.localIP());
}

void loop()
{
  // Comprobamos si hay algún cliente conectado
  WiFiClient client = _SERVER.available();
  if(!client)
    return;
  Serial.println("Nuevo cliente conectado");
  // Esperamos a que se envíe algún dato
  while(!client.available())
    delay(1);
  // Leemos la primera linea de los datos enviados
  String request = client.readStringUntil('\n');
  Serial.println(request);
  client.flush();
  // Parseamos los datos de la peticion
  if(request.indexOf("/LED=ON") != -1)
    digitalWrite(_LED, HIGH);
  else if (request.indexOf("/LED=OFF") != -1)
    digitalWrite(_LED, LOW);
  // Devolvemos la respuesta
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<a href=\"/LED=ON\"\"><button>ENCENDER MAGICAMENTE EL LED</button></a></br>");
  client.println("<a href=\"/LED=OFF\"\"><button> APAGAR MAGICAMENTE EL LED</button></a>");
  // Mostramos que se ha desconectado el cliente
  Serial.println("Cliente desconectado");
  // Ponemos un retardo para no recargar la ejecucion
  delay(1);
}

