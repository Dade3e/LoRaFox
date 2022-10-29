/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/ttgo-lora32-sx1276-arduino-ide/
*********/

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 23
#define DIO0 26

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 866E6

//OLED pins
#define OLED_SDA 21  //4
#define OLED_SCL 22 //15
#define OLED_RST -1 //16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

const uint8_t vbatPin = 35;
float VBAT;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

String LoRaData;

void setup() { 
  //initialize Serial Monitor
  Serial.begin(115200);
  pinMode(vbatPin, INPUT);
  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);
  
  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,10);
  display.print("LORA RECEIVER ");
  display.display();

  Serial.println("LoRa Receiver Test");
  
  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
  
  Serial.println("LoRa Initializing OK!");
  display.setCursor(0,20);
  display.println("LoRa Initializing OK!");
  display.setCursor(0,56);
  display.print("BATT:");
  display.setCursor(32,56);
  display.print(VBAT);
  display.display();
  
}
const float pi = 3.14159267 ;
int raggio = 30;
int clock_center_y = 32;
int clock_center_x = 96;
int wait = millis();
void loop() {
  //display.drawLine()
  // see if a packet was received
  
  int packetSize = LoRa.parsePacket();

  if (packetSize) {

 

    // if a packet size is defined, a packet was received

    Serial.println("");

    Serial.println("Received packet!");

 

    // read the packet
    String message = "";

    //while (LoRa.available()) {

      message += (char)LoRa.read();

    //}
    // Battery Voltage
    VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
    
    int rssi = LoRa.packetRssi();
    // print the Packet and RSSI
    Serial.println(message);
    Serial.println(rssi);
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,00);
    display.print("RSSI:");
    display.setTextSize(3);
    display.setCursor(0,20);
    display.print(rssi);
    display.setTextSize(1);
    display.setCursor(86,30);
    display.print("last");
    display.setCursor(86,40);
    display.print("recv:");
    display.setCursor(86,50);
    display.print((millis()-wait)/1000);
    wait = millis();
    display.setCursor(0,56);
    display.print("BATT:");
    display.setCursor(32,56);
    display.print(VBAT);
    
    display.display();
  }
}

void draw_minute(int line){
   int y = ((raggio-1)*cos(pi-(2*pi)/360*line))+clock_center_y;
   int x = ((raggio-1)*sin(pi-(2*pi)/360*line))+clock_center_x;
   display.drawLine(clock_center_x,clock_center_y,x,y,SSD1306_WHITE);
}
