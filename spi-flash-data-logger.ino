#include <SPI.h>

const int CS_PIN = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    // Wait for Serial on boards that require it (e.g., SAMD-based)
  }

  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH); // Deselect flash (CS is active-low)

  SPI.begin();
  SPI.beginTransaction(SPISettings(
      4000000,    // 4 MHz
      MSBFIRST,   // Data order: MSB first
      SPI_MODE0   // CPOL=0, CPHA=0
  ));

  uint8_t mfg, memType, capacity;

  delay(200);                     // Allow power/clock rails to settle after startup
  digitalWrite(CS_PIN, LOW);      // Select flash (CS low)
  SPI.transfer(0x9F);             // JEDEC ID command
  mfg = SPI.transfer(0x00);       // Read manufacturer ID
  memType = SPI.transfer(0x00);   // Read memory type
  capacity = SPI.transfer(0x00);  // Read capacity
  digitalWrite(CS_PIN, HIGH);     // Deselect flash (CS high)

  Serial.print("Manufacturer ID: 0x");
  Serial.println(mfg, HEX);
  Serial.print("Memory Type: 0x");
  Serial.println(memType, HEX);
  Serial.print("Capacity: 0x");
  Serial.println(capacity, HEX);

  SPI.endTransaction();

  // Tests
  writeEnable();
  sectorErase(0x000000);
  uint8_t status = readStatus();
  Serial.print("Status: 0x");
  Serial.println(status, HEX);
}

void loop() {}

/*
  W25Q32 datasheet:
  https://www.mouser.com/datasheet/2/949/w25q32jv_revg_03272018_plus-1489806.pdf?srsltid=AfmBOooOTjB2pOX_GyBSbHJVjGWvGK3wrtMf1u8OwBOIPlPtCPxb2-Ru
*/

/**
 * @brief Sets the Write Enable Latch (WEL) so the next program/erase can proceed.
 *
 * Notes:
 * - WEL is cleared automatically after many operations (program/erase/status writes),
 *   and is cleared on power-up.
 * - You generally must call this before any erase/program command.
 */
void writeEnable() {
  digitalWrite(CS_PIN, LOW);    // Select flash (CS low)
  SPI.transfer(0x06);           // Write Enable (06h)
  digitalWrite(CS_PIN, HIGH);   // Deselect flash (CS high)
}

/**
 * @brief Erases a 4KB sector containing the provided address.
 *
 * @param address 24-bit address within the sector to erase.
 *
 * Requirements:
 * - Write Enable must be set before issuing the erase command.
 */
void sectorErase(uint32_t address) {
  writeEnable();

  digitalWrite(CS_PIN, LOW);    // Select flash (CS low)
  SPI.transfer(0x20);           // Sector Erase (4KB) (20h)

  // Send 24-bit address MSB first (3 bytes)
  SPI.transfer((address >> 16) & 0xFF);
  SPI.transfer((address >> 8) & 0xFF);
  SPI.transfer(address & 0xFF);

  digitalWrite(CS_PIN, HIGH);   // Deselect flash (CS high)

  waitUntilReady();
}

/**
 * @brief Reads Status Register-1 (SR1).
 *
 * @return The 8-bit SR1 value.
 *
 * Notes:
 * - BUSY bit (bit0): 1 = busy, 0 = ready
 * - SR1 read command: 0x05
 */
uint8_t readStatus() {
  digitalWrite(CS_PIN, LOW);    // Select flash (CS low)
  SPI.transfer(0x05);           // Read Status Register-1 (05h)
  uint8_t status = SPI.transfer(0x00); // Clock out SR1
  digitalWrite(CS_PIN, HIGH);   // Deselect flash (CS high)
  return status;
}

/**
 * @brief Blocks until the flash is no longer busy.
 *
 * This polls SR1.BUSY (bit0). When the device finishes an internal program/erase
 * cycle, BUSY clears to 0 and the device will accept new commands.
 */
void waitUntilReady() {
  while (readStatus() & 0x01) {
    delay(1);
  }
}