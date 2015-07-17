/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  This example code is in the public domain.
 */
#include <LiquidCrystal.h>
LiquidCrystal lcd(30, 31, 32, 33, 34, 35);

#include <SimpleTimer.h>
SimpleTimer timer;

#include <WiFi.h>
#include <WString.h>

//char ssid[] = "LUCIE-PHILOU";     //  your network SSID (name)
//char pass[] = "";    // your network password
char ssid[] = "AnotherWifi";
char pass[] = "";
int status = WL_IDLE_STATUS;     // the Wifi radio's status
IPAddress ip;
WiFiServer server(80);
String readString = String(100);


#include <SPI.h>
#include <SD.h>
const int chipSelect = 4;

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;
int FRONT_RED = 46;
int FRONT_BLACK = 47;
int BACK_RED = 44;
int BACK_BLACK = 45;

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  pinMode(FRONT_RED, OUTPUT);
  pinMode(FRONT_BLACK, OUTPUT);
  pinMode(BACK_RED, OUTPUT);
  pinMode(BACK_BLACK, OUTPUT);
  
  // init lcd
  lcd.begin(16, 2);
  printLcd("Kitpages Wifi", "Connecting");

   // initialize serial:
  Serial.begin(9600);
  // attempt to connect using WPA2 encryption:
  Serial.println("Attempting to connect to WPA network...");
  status = WiFi.begin(ssid, pass);

  // if you're not connected, stop here:
  if ( status != WL_CONNECTED) {
    Serial.println("Couldn't get a wifi connection");
    printLcd("Wifi failed");
    while(true);
  }
  // if you are connected, print out info about the connection:
  else {
    Serial.println("Connected to network");
    Serial.print("LocalIp=");
    ip = WiFi.localIP();
    Serial.println(ip);
    printWifiStatus();
        
    printLcd("Kitpages WiFi OK");
    lcd.print(ip);
  }
  
  server.begin();

  // init SD card
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  
  // bootstrap app
  bootstrap();
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  
  Serial.print("Firmware version: ");
  Serial.println(WiFi.firmwareVersion());
}

void goForward() {
  digitalWrite(FRONT_RED, HIGH);
  digitalWrite(FRONT_BLACK, LOW);
  digitalWrite(BACK_RED, HIGH);
  digitalWrite(BACK_BLACK, LOW);
}
void goBackward() {
  digitalWrite(FRONT_RED, LOW);
  digitalWrite(FRONT_BLACK, HIGH);
  digitalWrite(BACK_RED, LOW);
  digitalWrite(BACK_BLACK, HIGH);
}

void turnLeft() {
  digitalWrite(FRONT_RED, LOW);
  digitalWrite(FRONT_BLACK, HIGH);
  digitalWrite(BACK_RED, HIGH);
  digitalWrite(BACK_BLACK, LOW);  
}
void turnRight() {
  digitalWrite(FRONT_RED, LOW);
  digitalWrite(FRONT_BLACK, HIGH);
  digitalWrite(BACK_RED, HIGH);
  digitalWrite(BACK_BLACK, LOW);  
}
void goStop() {
  digitalWrite(FRONT_RED, LOW);
  digitalWrite(FRONT_BLACK, LOW);
  digitalWrite(BACK_RED, LOW);
  digitalWrite(BACK_BLACK, LOW);  
}

void printLcd(char* line1, char* line2)
{
  lcd.clear();
  lcd.print(line1);
  lcd.setCursor(0,1);
  lcd.print(line2);
}
void printLcd(char* line1, String line2)
{
  lcd.clear();
  lcd.print(line1);
  lcd.setCursor(0,1);
  lcd.print(line2);
}
void printLcd(char *line)
{
  printLcd(line, "");
}


#define BUFSIZ 100
uint8_t bufindex;  //Buffer index, used to keep track of current buffer byte
const uint8_t maxbyte=255;  //The maximum allowable buffer length
uint8_t buf[maxbyte];  //Creates the buffer with a length equal to the max length
WiFiClient client;

void webServer() {
  char requestLine[BUFSIZ];
  String clientline;
  int index = 0;
  int image = 0;
  client = server.available();
  if (client) {
    boolean current_line_is_blank = true;
    index = 0;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c != '\n' && c != '\r') {
          requestLine[index] = c;
          index++;
          if (index >= BUFSIZ)
            index = BUFSIZ -1;
          continue;
        }
        requestLine[index] = 0;
        if (strstr(requestLine, "GET /") != 0) {
          clientline = String(requestLine);
        }
        index=0;
      }
    }
    sendResponse(clientline);
    delay(1);
    client.stop();
  }
}
    

void sendResponse(String clientline) {
  String filename;
  File dataFile;
  String mimeType;

  Serial.print("send response : ");
  Serial.println(clientline);
  
  if (clientline.startsWith("GET / ")) {
    filename = String("index.htm");
  }
  if (clientline.startsWith("GET /")) {
    if (!filename) filename = clientline.substring(5, clientline.length() - 9);
    
    dataFile = SD.open(filename.c_str());
    if (! dataFile) {
      client.println("HTTP/1.1 404 Not Found");
      client.println("Content-Type: text/html");
      client.println();
      client.println("<h2>File Not Found no file</h2>");
      client.println("<pre>");
      client.println(filename);
      client.println("</pre>");
      return;
    }
    Serial.print(filename);
    Serial.println("-Opened!");
    client.println("HTTP/1.1 200 OK");
    if (filename.endsWith(".htm"))
      mimeType = String("text/html");
    else if (filename.endsWith(".css"))
      mimeType = String("text/css");
    else if (filename.endsWith(".png"))
      mimeType = String("image/png");
    else if (filename.endsWith(".jpg"))
      mimeType = String("image/jpeg");
    else if (filename.endsWith(".gif"))
      mimeType = String("image/gif");
    else if (filename.endsWith(".js"))
      mimeType = String("application/x-javascript");
    else if (filename.endsWith(".xml"))
      mimeType = String("application/xml");
    else
      mimeType = String("text");
    mimeType = String("Content-Type: "+mimeType);
    
    client.println(mimeType);
    client.println("Connection: close");  // the connection will be closed after completion of the response

    Serial.println(mimeType);
    client.println();
    int16_t c;
    bufindex=0;  //reset buffer index
    
    while (dataFile.available()) {
      c = dataFile.read();
      buf[bufindex++]=((char)c);  //fill buffer
      if(bufindex==maxbyte)  //empty buffer when maximum length is reached.
      {
        client.write(buf, maxbyte);
        bufindex=0;
      }
    }
    if(bufindex>0)  //most likely, the size of the file will not be an even multiple of the buffer length, so any remaining data is read out.
    {
      client.write(buf, bufindex);
    }
    bufindex=0; //reset buffer index (reset twice for redundancy)
    dataFile.close();  //close the file
  } 
  else {
    Serial.print("404 response : ");
    Serial.println(clientline);
    client.println("HTTP/1.1 404 Not Found");
    client.println("Content-Type: text/html");
    client.println();
    client.println("clientline : <pre>");
    client.println(clientline);
    client.println("</pre>");
    client.println("<h2>File Not Found, bad request parsing. comment on arrive là ?</h2>");
  }
}

// the loop routine runs over and over again forever:
void loop() {
  //timer.run();
  delay(100);
  webServer();
  /*
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");       
          }
          client.println("</html>");
           break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
  */
}

void bootstrap() {
 
   // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt");

  // if the file is available, write to it:
  if (dataFile) {
    while (dataFile.available()) {
      Serial.write(dataFile.read());
    }
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }

/* 
  goForward();
  delay(1000);
  turnRight();
  delay(1500);
  goStop();
*/  
}
