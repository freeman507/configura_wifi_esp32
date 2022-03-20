#include <Arduino.h>
#include <WiFi.h>
#include "html.h"

const char *SERVER_SSID = "PONTO_ACESSO_ESP32";
const char *SERVER_PASWORD = "123456789";

String ssid;
String password;

WiFiServer wifiServer(80);
WiFiClient wifiClient;

void iniciarPontoAcesso();
bool conectarWifi();
void defineDadosWifi(String body);
void buscaBody(WiFiClient client);
void manterServer();

/**
 * Inicia o ponto de acesso para poder hospedar a web page.
 **/
void iniciarPontoAcesso()
{
  WiFi.enableAP(true);
  WiFi.softAP(SERVER_SSID, SERVER_PASWORD);
  IPAddress IP = WiFi.softAPIP();
  delay(1000);
  Serial.print("AP IP address: ");
  Serial.println(IP);
  wifiServer.begin();
  Serial.println("WEBPAGE ONLINE ESPERANDO DADOS DE ACESSO");
  ssid = "";
  password = "";
}

/**
 * Tenta estabelcer uma conecção Wifi, se a conecção for estabelecida
 * o Acess Point(AP) é desativado e o device passa a operar apenas
 * como station(STA). Retorna false quando a conecção no WiFi falha.
 **/ 
bool conectarWifi()
{
  bool conectado = false;
  if (!ssid.isEmpty())
  {
    Serial.println("CONECTANDO AO WIFI");
    WiFi.enableAP(false);
    WiFi.begin(ssid.begin(), password.begin());
    conectado = true;
    int tentativas = 60;
    while (WiFi.status() != WL_CONNECTED)
    {
      tentativas--;
      if (tentativas == 0)
      {
        iniciarPontoAcesso();
        conectado = false;
        break;
      }
      Serial.print(".");
      delay(500);
    }
    Serial.println("");
  }
  return conectado;
}

/**
 * Resgata as credenciais do Wifi dentro de um Response Body
 * de uma requisição HTTP, os dados podem ser acessados através
 * das variáveis ssid e password.
 **/
void defineDadosWifi(String body)
{
  if (!body.isEmpty())
  {
    int ssidIni = 0, ssidFin = 0, passwordIni = 0, passwordFin = 0;
    ssidIni = body.indexOf('=') + 1;
    ssidFin = body.indexOf('\n');
    passwordIni = body.indexOf('=', ssidFin + 1) + 1;
    passwordFin = body.length() - 1;
    ssid = body.substring(ssidIni, ssidFin);
    password = body.substring(passwordIni, passwordFin);
    ssid.trim();
    password.trim();
  }
}

/**
 * Verifica se a resposta do client contém credenciais do Wifi
 **/
void buscaBody(WiFiClient client)
{
  String body = "";
  int read = 0;
  while (true)
  {
    read = client.read();
    if (read > -1)
    {
      body += char(read);
    }
    else
    {
      break;
    }
  }
  defineDadosWifi(body);
}

/**
 * Mantem online a página web que contém o formulário para coletar
 * o ssid e senha da rede.
 **/ 
void manterServer()
{
  String header;
  String currentLine = "";
  WiFiClient client = wifiServer.available();
  if (client)
  {
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        header += c;
        if (c == '\n')
        {
          if (currentLine.length() == 0)
          {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            client.println(html);
            client.println();
            if (header.indexOf("POST") > -1)
            {
              buscaBody(client);
            }
            break;
          }
          else
          {
            currentLine = "";
          }
        }
        else if (c != '\r')
        {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
  }
}

/**
 * Verifica se o Wifi está conectado, retorna false quando não está.
 **/
bool verificaWifiOnline() {
  return WiFi.isConnected();
}

String buscaMACAdress() {
  return WiFi.macAddress();
}