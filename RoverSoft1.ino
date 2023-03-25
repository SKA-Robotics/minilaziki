#include <ESP8266WiFi.h>
String header;
WiFiServer server(80);
String outputPRZOD = "off";
String outputTYL = "off";
String outputPRAWO = "off";
String outputLEWO = "off";
const int motorsRight1 = 5; //d1
const int motorsRight2 = 4; // d2
const int motorsRightSpeed = 0; //d3
const int motorsLeft1 = 2; //d4
const int motorsLeft2 = 14; //d5
const int motorsLeftSpeed = 12; //d6

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("wgrano");

  pinMode(motorsRight1, OUTPUT);
  pinMode(motorsRight2, OUTPUT);
  pinMode(motorsLeft1, OUTPUT);
  pinMode(motorsLeft2, OUTPUT);
  pinMode(motorsRightSpeed, OUTPUT);
  pinMode(motorsLeftSpeed, OUTPUT);
  // Set outputs to LOW
  digitalWrite(motorsRight1, LOW);
  digitalWrite(motorsRight2, LOW);
  analogWrite(motorsRightSpeed, 0);
  digitalWrite(motorsLeft2, LOW);
  digitalWrite(motorsLeft1, LOW);
  analogWrite(motorsLeftSpeed, LOW);

  

  //CONNECT TO WIFI:
  /*WiFi.begin("Piwnica SKA", "StaryRouterZnowuNieDziala");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());*/

  //WIFI ACCESS POINT:
  Serial.print("Setting soft-AP ... ");
  boolean result = WiFi.softAP("MiniLazikSKA", "SKAR2137");
  if(result == true)
  {
    Serial.println("Ready");
  }
  else
  {
    Serial.println("Failed!");
  }
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);


  server.begin();
}

void loop() {
  //FOR WIFI ACCESS POINT
  /*Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());
  delay(3000);
  digitalWrite(D1, HIGH);*/

  //FOR WIFI CLIENT
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("GPIO 5 on");
              outputPRZOD = "on";
              outputTYL = "off";
              outputPRAWO = "off";
              outputLEWO = "off";
              digitalWrite(motorsLeft2, LOW);
              digitalWrite(motorsRight2, LOW);
              digitalWrite(motorsRight1, HIGH);
              digitalWrite(motorsLeft1, HIGH);
              analogWrite(motorsLeftSpeed, 127);
              analogWrite(motorsRightSpeed, 127);
            } else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("GPIO 4 on");
              outputPRZOD = "off";
              outputTYL = "on";
              outputPRAWO = "off";
              outputLEWO = "off";
              digitalWrite(motorsLeft1, LOW);
              digitalWrite(motorsRight1, LOW);
              digitalWrite(motorsRight2, HIGH);
              digitalWrite(motorsLeft2, HIGH);
              analogWrite(motorsLeftSpeed, 127);
              analogWrite(motorsRightSpeed, 127);
            } else if (header.indexOf("GET /3/on") >= 0) {
              Serial.println("GPIO 3 on");
              outputPRZOD = "off";
              outputTYL = "off";
              outputPRAWO = "on";
              outputLEWO = "off";
              digitalWrite(motorsLeft1, HIGH);
              digitalWrite(motorsRight1, HIGH);
              digitalWrite(motorsRight2, LOW);
              digitalWrite(motorsLeft2, LOW);
              analogWrite(motorsLeftSpeed, 127);
              analogWrite(motorsRightSpeed, 31);
            } else if (header.indexOf("GET /2/on") >= 0) {
              Serial.println("GPIO 2 on");
              outputPRZOD = "off";
              outputTYL = "off";
              outputPRAWO = "off";
              outputLEWO = "on";
              digitalWrite(motorsLeft1, HIGH);
              digitalWrite(motorsRight1, HIGH);
              digitalWrite(motorsRight2, LOW);
              digitalWrite(motorsLeft2, LOW);
              analogWrite(motorsLeftSpeed, 31);
              analogWrite(motorsRightSpeed, 127);
            } else {
              outputPRZOD = "off";
              outputTYL = "off";
              outputPRAWO = "off";
              outputLEWO = "off";
              digitalWrite(motorsLeft1, LOW);
              digitalWrite(motorsRight1, LOW);
              digitalWrite(motorsRight2, LOW);
              digitalWrite(motorsLeft2, LOW);
              analogWrite(motorsLeftSpeed, 0);
              analogWrite(motorsRightSpeed, 0);
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>CENTRUM STEROWANIA LAZIKIEM</h1>");
            
            // Display current state, and ON/OFF buttons for WPRZÓD  (5) 
            client.println("<p>Przod - State " + outputPRZOD + "</p>");
            // If the output5State is off, it displays the ON button       
            if (outputPRZOD=="off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for WSTECZ (4)
            client.println("<p>Wstecz - State " + outputTYL + "</p>");
            // If the output4State is off, it displays the ON button       
            if (outputTYL=="off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for PRAWO  (3) 
              client.println("<p>Prawo - State " + outputPRAWO + "</p>");
            // If the output5State is off, it displays the ON button       
            if (outputPRAWO=="off") {
              client.println("<p><a href=\"/3/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/3/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for LEWO (2)
            client.println("<p>Lewo - State " + outputLEWO + "</p>");
            // If the output4State is off, it displays the ON button       
            if (outputLEWO=="off") {
              client.println("<p><a href=\"/2/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/2/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            
            /*
            // Display current state, and ON/OFF buttons for PRAWO  (3) 
            client.println("<p>Przod - State " + output5State + "</p>");
            // If the output5State is off, it displays the ON button       
            if (output3State=="off") {
              client.println("<p><a href=\"/3/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/3/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for LEWO (2)
            client.println("<p>Wstecz - State " + output4State + "</p>");
            // If the output4State is off, it displays the ON button       
            if (output2State=="off") {
              client.println("<p><a href=\"/2/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/2/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            */
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
