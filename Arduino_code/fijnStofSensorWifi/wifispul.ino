/*
Arduino Due - ESP 8266 WiFi Module
Serial (Tx/Rx) communicate to PC via USB
Serial3 (Tx3/Rx3) connect to ESP8266
Tx3 - ESP8266 Rx
Rx3 - ESP8266 Tx
ESP8266 CH_PD Connect to ESP8266 VCC
*/
#define ASCII_0 48
#define ESP8266 Serial2
String SSID = "TestAP";
String PASSWORD = "12345678";
const int ESP8266_resetPin = 11;

int LED = 13;

boolean FAIL_8266 = false;

String strHTMLa = "<!doctype html>\
<html>\
<head>\
<title>HAN Sensor Workshop</title>\
</head>\
<body>\
<H1>Sensorwaarde:";

String strHTMLb = "</H1>\
</body>\
</html>";

void initWifi() {
  pinMode(LED, OUTPUT);

  digitalWrite(LED, LOW);
  delay(300);
  digitalWrite(LED, HIGH);
  delay(200);
  digitalWrite(LED, LOW);
  delay(300);
  digitalWrite(LED, HIGH);
  delay(200);
  digitalWrite(LED, LOW);
  delay(300);
  digitalWrite(LED, HIGH);
  delay(200);
  digitalWrite(LED, LOW);
  pinMode(ESP8266_resetPin, OUTPUT);
  digitalWrite(ESP8266_resetPin, HIGH);

  Serial.begin(9600);
  ESP8266.begin(115200);

  //Wait Serial Monitor to start
  while (!Serial);
  Serial.println("--- Start ---");


  ESP8266.println("AT+RST");
  delay(2000);
  ESP8266.println("AT+CWMODE=2"); // configure as access point
  delay(1000);
  ESP8266.println("AT+CIFSR"); // get ip address
  delay(1000);
  ESP8266.println("AT+CIPMUX=1"); // configure for multiple connections
  delay(1000);
  ESP8266.println("AT+CIPSERVER=1,80"); // turn o
  delay(1000);


  digitalWrite(LED, HIGH);
}

void checkWifi(float meetwaarde) {
  if (ESP8266.available())
  {
    Serial.println("Something received");
    delay(1000);
    if (ESP8266.find("+IPD,"))
    {
      String action;

      Serial.println("+IPD, found");
      int connectionId = ESP8266.read() - ASCII_0;
      Serial.println("connectionId: " + String(connectionId));

      char* str = new char[30];

      //float flt = 2.4567F;
      //sprintf(str, "%.4g", meetwaarde );

      String strHTML = strHTMLa + dtostrf(meetwaarde, 4, 4, str) + strHTMLb;

      Serial.println(strHTML);
      sendHTTPResponse(connectionId, strHTML);

      //Close TCP/UDP
      String cmdCIPCLOSE = "AT+CIPCLOSE=";
      cmdCIPCLOSE += connectionId;
      sendESP8266Cmdln(cmdCIPCLOSE, 1000);
    }
  }
}

void sendHTTPResponse(int id, String content)
{
  String response;
  response = "HTTP/1.1 200 OK\r\n";
  response += "Content-Type: text/html; charset=UTF-8\r\n";
  response += "Content-Length: ";
  response += content.length();
  response += "\r\n";
  response += "Connection: close\r\n\r\n";
  response += content;

  String cmd = "AT+CIPSEND=";
  cmd += id;
  cmd += ",";
  cmd += response.length();

  Serial.println("--- AT+CIPSEND ---");
  sendESP8266Cmdln(cmd, 1000);

  Serial.println("--- data ---");
  sendESP8266Data(response, 1000);
}

boolean waitOKfromESP8266(int timeout)
{
  do {
    Serial.println("wait OK...");
    delay(1000);
    if (ESP8266.find("OK"))
    {
      return true;
    }

  } while ((timeout--) > 0);
  return false;
}

boolean cwJoinAP()
{
  String cmd = "AT+CWJAP=\"" + SSID + "\",\"" + PASSWORD + "\"";
  ESP8266.println(cmd);
  return waitOKfromESP8266(10);
}

//Send command to ESP8266, assume OK, no error check
//wait some time and display respond
void sendESP8266Cmdln(String cmd, int waitTime)
{
  ESP8266.println(cmd);
  delay(waitTime);
  clearESP8266SerialBuffer();
}

//Basically same as sendESP8266Cmdln()
//But call ESP8266.print() instead of call ESP8266.println()
void sendESP8266Data(String data, int waitTime)
{
  ESP8266.print(data);
  delay(waitTime);
  clearESP8266SerialBuffer();
}

//Clear and display Serial Buffer for ESP8266
void clearESP8266SerialBuffer()
{
  Serial.println("= clearESP8266SerialBuffer() =");
  while (ESP8266.available() > 0) {
    char a = ESP8266.read();
    Serial.write(a);
  }
  Serial.println("==============================");
}
