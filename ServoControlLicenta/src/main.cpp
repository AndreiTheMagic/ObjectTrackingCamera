#include <Arduino.h>
#include <map>
#include <ESP32SPISlave.h>

ESP32SPISlave slave;

int freq = 500;

static constexpr uint32_t BUFFER_SIZE {32};
uint8_t spi_slave_tx_buf[BUFFER_SIZE];
uint8_t spi_slave_rx_buf[BUFFER_SIZE];


String command;
int transcommand=0;

int lrpin = 13;
int udpin = 14;
int fcpin = 15;

const int pwmFreq = 50;         // 50Hz for servo
const int pwmResolution = 16;   // 16-bit resolution

const int lrChannel = 0;
const int udChannel = 1;
const int fcChannel = 2;

const int dutyMin = 1500;
const int dutyMax = 8000;

uint32_t angleToDuty(int angle) {
    angle = constrain(angle, 0, 180);
    return map(angle, 0, 180, dutyMin, dutyMax);
}

void setServoAngle(int channel, int angle) {
    ledcWrite(channel, angleToDuty(angle));
}


int posV = 90;
int posH = 90;

int step = 5;

int pos = 0;

using Func=void (*)();

void setup() {

    ledcAttachPin(lrpin, lrChannel); // atașează pinul și definește impulsul minim/maxim (μs)
    ledcSetup(lrChannel, pwmFreq, pwmResolution);
    ledcAttachPin(udpin, udChannel);
    ledcSetup(udChannel, pwmFreq, pwmResolution);
    ledcAttachPin(fcpin, fcChannel);
    ledcSetup(fcChannel, pwmFreq, pwmResolution);

    setServoAngle(lrChannel, posH);
    setServoAngle(udChannel, posV);

    Serial.begin(115200);
    delay(2000);

    slave.setDataMode(SPI_MODE0);
    //slave.begin();
    slave.begin(VSPI);   // specificare bus VSPI

    // curățare buffer-ele
    memset(spi_slave_tx_buf, 0, BUFFER_SIZE);
    memset(spi_slave_rx_buf, 0, BUFFER_SIZE);

    slave.queue(NULL, spi_slave_rx_buf, BUFFER_SIZE);
}


void moveRight() {
    if (posH >= step) {
        posH -= step;
        setServoAngle(lrChannel, posH);
        Serial.println("Right: ");
    }
}

void moveLeft() {
    if (posH <= 180 - step) {
        posH += step;
        setServoAngle(lrChannel, posH);
    }
}

void moveDown() {
    if (posV >= step) {
        posV -= step;
        setServoAngle(udChannel, posV);
    }
}

void moveUp() {
    if (posV <= 180 - step) {
        posV += step;
        setServoAngle(udChannel, posV);
    }
}

void moveHome() {
    setServoAngle(lrChannel, 90);
    setServoAngle(udChannel, 90);
}

void focusIN(){
    if (pos <= 180 - step) {
        pos += step;
        setServoAngle(fcChannel, pos);
    }
}

void focusOUT(){
    if (pos >= step) {
        pos -= step;
        setServoAngle(fcChannel, pos);
    }
}

void focusHome(){
    setServoAngle(fcChannel, 90);
}

std::map<u_int8_t, Func> commands = {
  {0x01, moveRight},
  {0x02, moveLeft},
  {0x03, moveDown},
  {0x04, moveUp},
  {0x11, moveHome},
  {0x05, focusIN},
  {0x06, focusOUT},
  {0x12, focusHome}
};

void loop() {
  
  // așteaptă finalizarea tranzacțiilor puse în coadă
    const std::vector<size_t> received_bytes = slave.wait();

    if (received_bytes.empty()) {
        Serial.println("Nu s-au primit date");
    }
    else{
      Serial.printf("S-au primit %d octeti:\n", received_bytes[0]);
      for (size_t i = 0; i < received_bytes[0]; ++i) {
          Serial.printf("0x%02X ", spi_slave_rx_buf[i]);
          auto it = commands.find(spi_slave_rx_buf[i]);
          if (it != commands.end()) {
              it->second(); // apelează funcția asociată
          }
        }
    }
  
  memset(spi_slave_tx_buf, 0, BUFFER_SIZE);
  memset(spi_slave_rx_buf, 0, BUFFER_SIZE);

  slave.queue(NULL, spi_slave_rx_buf, BUFFER_SIZE);
  delay(10); // așteaptă o secundă înainte de următoarea iterație a buclei

}
