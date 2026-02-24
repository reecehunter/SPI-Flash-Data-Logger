#include <SPI.h>

const int CS_PIN = 0;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;

  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);

  SPI.begin();
  SPI.beginTransaction(SPISettings(
      4000000, // 4 MHz
      MSBFIRST,
      SPI_MODE0));

  uint8_t mfg, memType, capacity;

  delay(200); // Prevent bad readings from startup instability
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(0x9F); // JEDEC ID command
  mfg = SPI.transfer(0x00);
  memType = SPI.transfer(0x00);
  capacity = SPI.transfer(0x00);
  digitalWrite(CS_PIN, HIGH);

  Serial.print("Manufacturer ID: 0x");
  Serial.println(mfg, HEX);
  Serial.print("Memory Type: 0x");
  Serial.println(memType, HEX);
  Serial.print("Capacity: 0x");
  Serial.println(capacity, HEX);

  write();
}

void loop() {}

//////////////////////////////////
//
// W25Q32 datasheet:
//   https://www.mouser.com/datasheet/2/949/w25q32jv_revg_03272018_plus-1489806.pdf?srsltid=AfmBOooOTjB2pOX_GyBSbHJVjGWvGK3wrtMf1u8OwBOIPlPtCPxb2-Ru
//
//////////////////////////////////

void write()
{
  // Pull CS low to select    - Power consumption increases to active levels
  digitalWrite(CS_PIN, LOW);
  // Pull CS high to deselect - Power consumption decreases to standby levels
  digitalWrite(CS_PIN, HIGH);
}
