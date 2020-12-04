/*
  WiFi AP mode

  Broadcast a small table with weather information at ip address: 192.168.4.1

*/


#include <WiFiNINA.h>
#include <Adafruit_MLX90614.h>
#include <nRF24L01.h>
#include <RF24.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

bool SERIAL_GO = true;               // set to false when device is stand-alone
char ssid[]    = "Astro_Weather";     // your network SSID (name)
char pass[]    = "Bingo9999";         // your network password (use for WPA, or use as key for WEP)
int keyIndex   = 0;                   // your network key Index number (needed only for WEP)

//  wifi setup
int status = WL_IDLE_STATUS;
WiFiServer server(80);

// radio setup
RF24 radio(7, 6); // CE, CSN
const byte address[6] = "00001";

void RadioTransmit() {
  if ( SERIAL_GO ) Serial.println("Transmit Radio Status");
  const char text[] = "Hello World";
  radio.write(&text, sizeof(text));
}

void WifiClientService() {
  // compare the previous status to the current status
  if (status != WiFi.status()) {
    // it has changed update the variable
    status = WiFi.status();

    if ( SERIAL_GO ) {
      if (status == WL_AP_CONNECTED) {
        // a device has connected to the AP
        Serial.println("Device connected to AP");
      } else {
        // a device has disconnected from the AP, and we are back in listening mode
        Serial.println("Device disconnected from AP");
      }
    }
  }

  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    if ( SERIAL_GO ) Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        if ( SERIAL_GO ) Serial.write(c);   // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            
            //meta-refresh page every 5 seconds
            //client.print("<HEAD>");
            //client.print("<meta http-equiv=\"refresh\" content=\"5\">");
            //client.print("<TITLE />Zoomkat's meta-refresh test</title>");
            //client.print("</head>");

            client.print("<table><tr><th>Ambient_F</th><th>Object_F</th></tr>");
            client.print("<tr><td>"+String(mlx.readAmbientTempF(),2)+"</td><td>"+String(mlx.readObjectTempF(),2)+"</td></tr></table>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // close the connection:
    client.stop();
    if ( SERIAL_GO ) Serial.println("client disconnected");
  }
}

void setup() {
  if ( SERIAL_GO ) {
    //Initialize serial and wait for port to open:
    Serial.begin(9600);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }
    Serial.println("Access Point Web Server");
  }
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    if ( SERIAL_GO ) Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  if ( SERIAL_GO ) {
    String fv = WiFi.firmwareVersion();
    if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
      Serial.println("Please upgrade the firmware");
      Serial.println(fv);
      Serial.println(WIFI_FIRMWARE_LATEST_VERSION);
    }
  
    // print the network name (SSID);
    Serial.print("Creating access point named: ");
    Serial.println(ssid);
  }
  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    if ( SERIAL_GO ) Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }

  // wait 10 seconds for connection:
  delay(10000);

  // start the web server on port 80
  server.begin();
  mlx.begin();

  // you're connected now, so print out the status
  if ( SERIAL_GO ) printWiFiStatus();

  //  Start the radio
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}


void loop() {
  WifiClientService();
  RadioTransmit();
  delay(5000);
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);

}
