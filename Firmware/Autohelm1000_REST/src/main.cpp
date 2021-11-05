#include "Arduino.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SoftwareSerial.h>
#include <ArduinoOTA.h>

const char *ssid = "autohelm1000";
const char *password = "12qwaszx";

const char *main_page = R"=====(
<!DOCTYPE html>
<html>

<body>
    <style>
    .greenbutton {
        background-color:green; /* Green */
        border: none;
        color: white;
        padding: 15px 32px;
        text-align: center;
        text-decoration: none;
        display: inline-block;
        font-size: 16px;
        
        width:100%
    }

    .redbutton {
        background-color:red; /* Green */
        border: none;
        color: white;
        padding: 15px 32px;
        text-align: center;
        text-decoration: none;
        display: inline-block;
        font-size: 16px;
        width:100%
    }
   
    </style>

    <script>
        function sendData(relative_bearing) {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function () {
                if (this.status == 200) {
                    
                }
            };
            xhttp.open("GET", "setRelativeBearing?bearing=" + relative_bearing, true);
            xhttp.send();
        }
    </script>
    <div id="demo">
        <table>
            <tr>
        <td><button type="button" class="redbutton" onclick="sendData(-1)">-1</button></td>
        <td><button type="button" class="greenbutton"  onclick="sendData(1)">+1</button></td>
        </tr>
        <td><button type="button" class="redbutton"  onclick="sendData(-10)">-10</button></td>
        <td><button type="button" class="greenbutton"  onclick="sendData(10)">+10</button></td>
    </div>


</body>

</html>
)=====";

WiFiManager wifiManager;
ESP8266WebServer server(80);
SoftwareSerial ah(0, 2);

void handleRoot()
{ 
  server.send(200, "text/html", main_page); //Send web page}
}

void setRelativeBearing()
{
  if (server.arg("bearing") == "")
  {
    Serial.println("Ret5quired parameter bearing not found!");
  }
  else
  { 
    String message = server.arg("bearing"); //Gets the value of the query parameter
    int bearing = message.toInt();
    ah.println(bearing);
    server.send(200, "text/html", ""); //Send web page}
  }
}

// Manage not found URL
void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void)
{
  Serial.begin(115200);
  // set the data rate for the SoftwareSerial port
  ah.begin(9600);
  WiFi.mode(WIFI_STA);
  wifiManager.autoConnect(ssid, password);

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("Autohelm1000");

  // No authentication by default
  ArduinoOTA.setPassword((const char *)"12qwaszx");

  ArduinoOTA.onStart([]()
                     { Serial.println("Start"); });
  ArduinoOTA.onEnd([]()
                   { Serial.println("\nEnd"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });
  ArduinoOTA.onError([](ota_error_t error)
                     {
                       Serial.printf("Error[%u]: ", error);
                       if (error == OTA_AUTH_ERROR)
                         Serial.println("Auth Failed");
                       else if (error == OTA_BEGIN_ERROR)
                         Serial.println("Begin Failed");
                       else if (error == OTA_CONNECT_ERROR)
                         Serial.println("Connect Failed");
                       else if (error == OTA_RECEIVE_ERROR)
                         Serial.println("Receive Failed");
                       else if (error == OTA_END_ERROR)
                         Serial.println("End Failed");
                     });
  ArduinoOTA.begin();

  server.on(F("/"), HTTP_GET, handleRoot);
  server.on(F("/setRelativeBearing"), HTTP_GET, setRelativeBearing);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void)
{
  ArduinoOTA.handle();
  server.handleClient();
}