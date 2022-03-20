#include <Arduino.h>
#include "conexao-wifi.h"

bool wifiOnline;

void setup();
void loop();
void autenticaoWifi();

void setup()
{
  Serial.begin(115200);
  iniciarPontoAcesso();
  wifiOnline = false;
}

void loop()
{
  if (!wifiOnline)
  {
    autenticaoWifi();
  }
}

void autenticaoWifi()
{
  manterServer();
  wifiOnline = conectarWifi();
  if (wifiOnline)
  {
    Serial.println("WIFI ON");
  }
}
