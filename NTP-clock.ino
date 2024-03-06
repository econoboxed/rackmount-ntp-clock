/*
 Original Udp NTP Client code by
 by Michael Margolis, Tom Igoe, Arturo Guadalupi

 This code is in the public domain.

 */

#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

Adafruit_7segment matrix = Adafruit_7segment();

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

unsigned int localPort = 8888;
const char timeServer[] = "192.168.2.10";
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[NTP_PACKET_SIZE];

// long to store UNIX time
unsigned long secsSince1900;

// update to your local time zone
const int timezone = -4;

// change to true if you wish for the dots to flash
const bool flash = false;

// A UDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() {

  // Begin matrix display
  matrix.begin(0x70);

  // You can use Ethernet.init(pin) to configure the CS pin
  Ethernet.init(10);  // Most Arduino shields

  // start Ethernet and UDP
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    } else if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // no point in carrying on, so do nothing forevermore:
    while (true) {
      delay(1);
    }
  }
  Udp.begin(localPort);
}

void loop() {
  sendNTPpacket(timeServer); // send an NTP packet to a time server

  // wait to see if a reply is available
  if (flash){
    for (int i = 0; i<=2; i++){
    matrix.drawColon(i % 2 == 1);
    matrix.writeDisplay();
    delay(1000);
    }
  }
  else{
    delay(2000);
  }
  
  // if the UDP request recieved a packet, update the clock with the information from the time server
  if (Udp.parsePacket()) {
    matrix.writeDigitRaw(0, 0x04);
    matrix.drawColon(true);

    // We've received a packet, read the data from it
    Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
    // the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, extract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    secsSince1900 = highWord << 16 | lowWord;

  // if the UPD request got no responce, update the clock based on the delay since the last update
  // This will not keep perfect time, and will slowly drift out-of-sync until a new packet is able to properly update the clock
  // An LED in the upper left of the clock is illuminated to show this state to the user
  }else{
    matrix.writeDigitRaw(2, 0x04);
    secsSince1900 += 12;
  }

  // now convert NTP or estimated time into everyday time:
  // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
  const unsigned long seventyYears = 2208988800UL;

  // subtract seventy years:
  unsigned long epoch = secsSince1900 - seventyYears;
    
  //subtract seconds based on what the timezone is set to
  epoch = epoch + (timezone * 3600);

  // draw the display digit by digit
  matrix.writeDigitNum(0,(((epoch  % 86400L) / 3600) / 10));
  matrix.writeDigitNum(1,(((epoch  % 86400L) / 3600) % 10));
  matrix.writeDigitNum(3,(((epoch  % 3600) / 60) / 10));
  matrix.writeDigitNum(4,(((epoch  % 3600) / 60) % 10)); 

  matrix.writeDisplay();

  // wait ten seconds before asking for the time again
  if (flash){
    for (int i = 0; i<=10; i++){
    matrix.drawColon(i % 2 == 0);
    matrix.writeDisplay();
    delay(1000);
    }
  }
  else{
    delay(10000);

  }
  
  Ethernet.maintain();
}

// send an NTP request to the time server at the given address
void sendNTPpacket(const char * address) {
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); // NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}