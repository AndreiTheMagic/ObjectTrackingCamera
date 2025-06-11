#include <Arduino.h>
#include <ESP32SPISlave.h>

ESP32SPISlave slave;

static constexpr uint32_t BUFFER_SIZE {32};
uint8_t spi_slave_tx_buf[BUFFER_SIZE];
uint8_t spi_slave_rx_buf[BUFFER_SIZE];

#define LED 2
void setup() {
    Serial.begin(115200);
    delay(2000);
    // begin() after setting
    // HSPI = CS: 15, CLK: 14, MOSI: 13, MISO: 12 -> default
    // VSPI = CS:  5, CLK: 18, MOSI: 23, MISO: 19
    slave.setDataMode(SPI_MODE0);
    //slave.begin();
    slave.begin(VSPI);   // you can use VSPI like this

    // clear buffers
    memset(spi_slave_tx_buf, 0, BUFFER_SIZE);
    memset(spi_slave_rx_buf, 0, BUFFER_SIZE);

    slave.queue(NULL, spi_slave_rx_buf, BUFFER_SIZE);
}

void loop() {
    // wait for the completion of the queued transactions
    const std::vector<size_t> received_bytes = slave.wait();

    if (received_bytes.empty()) {
        Serial.println("No data received");
    }
    else{
      Serial.printf("Received %d bytes:\n", received_bytes[0]);
      for (size_t i = 0; i < received_bytes[0]; ++i) {
          Serial.printf("0x%02X ", spi_slave_rx_buf[i]);
      }
    }
  
  memset(spi_slave_tx_buf, 0, BUFFER_SIZE);
  memset(spi_slave_rx_buf, 0, BUFFER_SIZE);

  slave.queue(NULL, spi_slave_rx_buf, BUFFER_SIZE);
  delay(10); // wait for a second before the next loop iteration
}