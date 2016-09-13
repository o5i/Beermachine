
#include <SimpleModbusSlave.h> //http://forum.arduino.cc/index.php?topic=176142.new#new https://drive.google.com/folderview?id=0B0B286tJkafVSENVcU1RQVBfSzg&usp=drive_web'
#include <OneWire.h> //http://playground.arduino.cc/Learning/OneWire
#include <DallasTemperature.h>

// ONEWIRE
#define BUS_PIN 10
#define TEMPERATURE_PRECISION 11
OneWire ONEWIRE_BUS(BUS_PIN);
DallasTemperature DS18B20(&ONEWIRE_BUS);
DeviceAddress DS18B20_0, DS18B20_1, DS18B20_2;

// MODBUS
#define MOD_REGS 4
#define MOD_COM Serial1
#define MOD_BAUD 115200
unsigned int MOD_REG[MOD_REGS];

unsigned int j = 0;

void setup() {
  DS18B20.begin();
  DS18B20.getAddress(DS18B20_0, 0);
  DS18B20.getAddress(DS18B20_1, 1);
  DS18B20.getAddress(DS18B20_2, 2);
  DS18B20.setResolution(DS18B20_0, TEMPERATURE_PRECISION);
  DS18B20.setResolution(DS18B20_1, TEMPERATURE_PRECISION);
  DS18B20.setResolution(DS18B20_2, TEMPERATURE_PRECISION);

  modbus_configure(&MOD_COM, MOD_BAUD, SERIAL_8N1, 1, 2, MOD_REGS, MOD_REG);
  modbus_update_comms(MOD_BAUD, SERIAL_8N1, 1);

}

void loop() {
  modbus_update();
  DS18B20.setWaitForConversion(false);
  DS18B20.requestTemperatures();
  MOD_REG[0] = DS18B20.getTempC(DS18B20_0);
  MOD_REG[1] = DS18B20.getTempC(DS18B20_1);
  MOD_REG[2] = DS18B20.getTempC(DS18B20_2);
  MOD_REG[3] = j;
  j++;
  if (j >= 50000) {
    j = 0;
  }
}
