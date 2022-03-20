#include <Arduino.h>
#include "config.h"

String html = "<!DOCTYPE html> \
<head> \
    <title>" + TITULO_PROJETO + "</title> \
    <style> \
        .container { \
            width: 100%; \
            display: flex; \
            flex-direction: column; \
            align-items: center; \
            justify-content: center; \
        } \
    </style> \
</head> \
<body> \
    <form action=\"conectar\" method=\"post\" enctype=\"text/plain\"> \
        <div class=\"container\"> \
            <h3>HOSPITAL VIRTUTAL</h3> <label for=\"ssid\">WIFI SSID </label> <input type=\"text\" id=\"ssid\" name=\"ssid\"> \
            <br><label for=\"password\">WIFI PASSWORD</label> <input type=\"password\" id=\"password\" name=\"password\"> \
            <br><input type=\"submit\" value=\"CONECTAR\"> \
        </div> \
    </form> \
</body>";