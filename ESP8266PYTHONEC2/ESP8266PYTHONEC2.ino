/* Autor:Aramis Felipe Pizza
Email:aramisfelipe@live.com

Codigo para esp8266-12/12F ou esp8266-01
leitura de banco de dados postgreSQL por servidor intermediario python.
automação via rede global utilizando EC2 AMAZON hospedando servidor python e database.

utilização do wifi manager para gerenciar melhor a rede do ambiente, modo de reset de rede ao setar valor zero no banco de dados.
*/
#include <Arduino.h>
#include <ESP8266WiFi.h>         
#include <DNSServer.h> 
#include <ESP8266WebServer.h> 
#include <WiFiManager.h>   
#include <ESP8266HTTPClient.h>

//flag para indicar se foi salva uma nova configuração de rede
bool shouldSaveConfig = false;


const char* serverAddress = "host python";
const int serverPort = 5000;
bool liga=false;
char message[8];


void setup() {
  pinMode(1,OUTPUT);
  pinMode(0,OUTPUT); 
  digitalWrite(1,HIGH);
  digitalWrite(0,LOW);
     
  //Serial.begin(115200);
  WiFiManager SelecaoRede;
  SelecaoRede.setAPCallback(configModeCallback); 
  SelecaoRede.setSaveConfigCallback(saveConfigCallback); 
  SelecaoRede.autoConnect("LANÇADOR","12345678"); //cria uma rede sem senha
 enviar(); 
}

void loop() {
  WiFiManager SelecaoRede;
  HTTPClient http;
  WiFiClient client;

    http.begin(client,"http://" + String(serverAddress) + ":" + String(serverPort) + "/obter-dados");

    int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    String response = http.getString();
    response.toCharArray(message, 8);
    if((message[4]=='1')&&(message[2]=='1'))
    {
     int tempo= (message[6]-48)*1000; 
     digitalWrite(1,LOW);
     digitalWrite(0,HIGH);
     delay(tempo);
     enviar();
     digitalWrite(1,HIGH);
     digitalWrite(0,LOW);
     
    }
    if((message[4]=='0')&&(message[2]=='1'))
    {
     digitalWrite(0,LOW);
      digitalWrite(1,HIGH);
    }
    if(message[2]=='0')
    {
    if(!SelecaoRede.startConfigPortal("ConLANÇADOR", "12345678")){
        delay(2000);
        ESP.restart();
        delay(1000);
      }
      enviar();
      }

    } 
 
    http.end();
  
    

}


//callback que indica que o ESP entrou no modo AP
void configModeCallback (WiFiManager *myWiFiManager) {  
//  
  Serial.println("Entrou no modo de configuração");
  Serial.println(WiFi.softAPIP()); //imprime o IP do AP
  Serial.println(myWiFiManager->getConfigPortalSSID()); //imprime o SSID criado da rede

}

//callback que indica que salvamos uma nova rede para se conectar (modo estação)
void saveConfigCallback () {
  Serial.println("Configuração salva");
  Serial.println(WiFi.softAPIP()); //imprime o IP do AP
}



void enviar()
{
HTTPClient http;
  WiFiClient client;
  
  http.begin(client,"http://" + String(serverAddress) + ":" + String(serverPort) + "/enviar-dados");
  http.addHeader("Content-Type", "application/json");

  String jsonData = "{\"valor1\": 0 , \"valor2\": 0 ,\"valor3\": 1}";
 //
  int httpResponseCode = http.POST(jsonData);

  if (httpResponseCode > 0) {
    String response = http.getString();
    response.toCharArray(message, 8);
    Serial.println("Resposta do servidor: " + response);
  } else {
    Serial.println("Erro na solicitação HTTP");
  }

  http.end();


}