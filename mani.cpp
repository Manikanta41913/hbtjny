#include <DHT.h>
#include <Wire.h>
#include <WiFi.h>
#define echoPin 19
#define trigPin 21
#define LDRinput 18
#define DHTinput 15
#define Fanpin 2
#define Lightpin 4

  bool h=0;
  float temp;
  long duration, distance;
  bool b0,b1,b2,b3,b4;
  const char *ssid = "MY_SSID";
  const char *password = "MY_PASSWORD";
  int x = 10; //10 seconds time to connect to a Wifi network

DHT dht(5, DHT11);

NetworkServer server(80);

void fanlightoff(){ //Fan and light off if function called
  digitalWrite(Fanpin, HIGH);
  digitalWrite(Lightpin, HIGH);
}
void sensor_IO_init(){
  dht.begin();
  delay(2000);
  pinMode(Fanpin, OUTPUT); //FAN
  pinMode(Lightpin, OUTPUT); //LIGHT 
  fanlightoff();
  pinMode(echoPin, INPUT);  //Ultrason i/p
  pinMode(trigPin, OUTPUT); //Ultrason o/p
  pinMode(LDRinput, INPUT); //Ldr i/p
  pinMode(DHTinput, INPUT); //DHT i/p
}
void wifi_server_init(){
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED && x!=0) {
    delay(1000);
    Serial.print(".");
    x--;}
  if(WiFi.status() == WL_CONNECTED){
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  delay(5000);}
  else{
  Serial.println("");
  Serial.println("WiFi Not connected.");}
}
void readTemp(){
  temp = dht.readTemperature();
  Serial.print("\nTemperature: ");
  Serial.print(temp);
  Serial.print(" C");
  if(temp>32)
  {b0=1;}
  else{b0=0;}
}
void readDist(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration / 58.2;
  Serial.print("\nDistance: ");
  Serial.print(distance);
  Serial.print(" cm");
  if(distance<6)
  {b2=1;}
  else{b2=0;}
}
void readDarkness(){
  if(digitalRead(18)==HIGH)
  {b1=1; Serial.print("\nDarkness Detected");}
  else{b1=0; Serial.print("\nIt is not Dark");}
}
void lightCtrlAuto(){
  b3 = b1 & b2;
  delay(100);
  digitalWrite(Lightpin, !b3);
}
void fanCtrlAuto(){
  b4 = b2 & b0;
  delay(100);
  digitalWrite(Fanpin, !b4);
}
void lightStatus(){
  if(digitalRead(Lightpin)==0){Serial.print("\nLight is On");}
  else{Serial.print("\nLight is Off");}
}
void fanStatus(){
  if(digitalRead(Fanpin)==0){Serial.print("\nFan is On");}
  else{Serial.print("\nFan is Off");}
}
void setup() {
  Serial.begin(115200);
  sensor_IO_init();
  wifi_server_init();
}
void loop() {

  NetworkClient client = server.accept();  // listen for incoming clients
 
    if (client) {                     // if you get a client,
      //Serial.print("\nNew Client.");  // print a message out the serial port
      String currentLine = "";        // make a String to hold incoming data from the client
      while (client.connected()) {    // loop while the client's connected
        if (client.available()) {     // if there's bytes to read from the client,
          char c = client.read();     // read a byte, then
          //Serial.write(c);            // print it out the serial monitor
          if (c == '\n') {            // if the byte is a newline character
            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0) {
              // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
              // and a content-type so the client knows what's coming, then a blank line:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println();
              if(h==1){
              // the content of the HTTP response follows the header:
              client.println("<HTML><title>ESP32</title>");
              client.println("<body><h1><center>ESP32 PROJECT JAYAKRISHNAN MENON</center></h1>");
              client.println("<h2><center>You are in Manual Mode</center></h2>");
              client.println("<p><center>Light Control</center></p>");
              client.println("<a href=\"/lightOn\"\"><center><button>ON</button></a> <a href=\"/lightoff\"\"><button>OFF</button></center></a>");
              client.println("<p><center>Fan Control</center></p>");
              client.println("<a href=\"/fanOn\"\"><center><button>ON</button></a> <a href=\"/fanoff\"\"><button>OFF</button></center></a>");
              client.println("<p><center>Manual Mode</center></p>");
              client.println("<center><button disabled>ON</button> <a href=\"/Manualoff\"\"><button>OFF</button></center></a>");
              client.println("</body></HTML>");}
              if(h==0){
              // the content of the HTTP response follows the header:
              client.println("<HTML><title>ESP32</title>");
              client.println("<body><h1><center>ESP32 PROJECT JAYAKRISHNAN MENON</center></h1>");
              client.println("<h2><center>You are in Auto Mode</center></h2>");
              client.println("<p><center>Light Control</center></p>");
              client.println("<center><button disabled>ON</button> <button disabled>OFF</button></center>");
              client.println("<p><center>Fan Control</center></p>");
              client.println("<center><button disabled>ON</button> <button disabled>OFF</button></center>");
              client.println("<p><center>Manual Mode</center></p>");
              client.println("<a href=\"/ManualOn\"\"><center><button>ON</button></a> <button disabled>OFF</button></center>");
              client.println("</body></HTML>");}
              // The HTTP response ends with another blank line:
              client.println();
              break; // break out of the while loop:
            } else{currentLine = "";}// if you got a newline, then clear currentLine:
          } else if (c != '\r'){currentLine += c;}// if you got anything else but a carriage return character, add it to the end of the currentLine
          // Check to see the client:
          if (currentLine.endsWith("GET /fanOn")) {
            digitalWrite(Fanpin, LOW);}
          if (currentLine.endsWith("GET /fanoff")) {
            digitalWrite(Fanpin, HIGH);}
          if (currentLine.endsWith("GET /lightOn")) {
            digitalWrite(Lightpin, LOW);}
          if (currentLine.endsWith("GET /lightoff")) {
            digitalWrite(Lightpin, HIGH);}
          if (currentLine.endsWith("GET /Manualoff")) {
            h=0;
            fanlightoff();}
          if (currentLine.endsWith("GET /ManualOn")) {
            h=1;  
            fanlightoff();}
        }
      }
      // close the connection:
      client.stop();
      //Serial.print("\nClient Disconnected.");
    }
    if(h==0){
      delay(1000);
      Serial.println();
      Serial.print("\nAuto Mode");
      readTemp();
      readDarkness();
      readDist();
      lightCtrlAuto();
      lightStatus();
      fanCtrlAuto();
      fanStatus();}
    else{
      delay(1000);
      Serial.println();
      Serial.print("\nWaiting for Manual Control");
      lightStatus();
      fanStatus();}
}
