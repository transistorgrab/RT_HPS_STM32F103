/** this file contains all code regarding I2C eeprom access */
#include "hal.h"
//#include "ch.h"
#include "board.h"

#ifndef _NVMEM_C_
#define _NVMEM_C_

#define NV_TYPE         M24C04
#define NV_DEVADDR      0b1010110   // E1, E2 are pulled HIGH in hardware, only 7 bits for address, R/W bit is added by driver
#define NV_MEMSIZE      512         // 4 Kibit / 512 Byte
#define NV_PAGESIZE     16          // Bytes
#define NV_PAGES        32
#define NV_WRITETIME_MS 5           // guaranteed maximum in datasheet
#define NV_TIMEOUT_MS   50

static const I2CConfig i2ccfg = {
  OPMODE_I2C,
  400000,
  FAST_DUTY_CYCLE_2
};

static const I2CConfig i2ccfg_ls = {
  OPMODE_I2C,
  100000,
  STD_DUTY_CYCLE
};

// store the last active settings
typedef struct settings_ {
  uint16_t setVolA;
  uint16_t setVolB;
  uint16_t setCurA;
  uint16_t setCurB;
} settings_struct;
#define SETTINGS_SIZE 4

settings_struct settings;

/** array of 8 bit values that holds the calibration values and current settings to send to nv memory
 *  each value is stored as 16 bit split into 8 bit parts
 *  4 settings (2x current, 2x voltage) = 4 * 2 = 8 bytes
 *  2 calibration values per settings per channel = 2 * 4 * 2 = 16 bytes
 *  in total: 24 bytes
 *  per page we can store 16 bytes
 *  therefore, settings go in one page, calibration in another page
 */
static uint8_t nvSettingValues[8];
#define SETTINGS_ADDRESS     (0x00)
static uint8_t nvCalibrationValues[16];
#define CALIBRATION_ADDRESS  (SETTINGS_ADDRESS + NV_PAGESIZE)

/** store the values for the calibration of ADC values
 *  calibration is done with a formula y = a*x + b
 *  we need a and b for each input (voltage A, B, current A, B)
 */

typedef struct calibrationValues_ {
  uint16_t calVolAa;
  uint16_t calVolAb;
  uint16_t calVolBa;
  uint16_t calVolBb;
  uint16_t calCurAa;
  uint16_t calCurAb;
  uint16_t calCurBa;
  uint16_t calCurBb;
} calibrationValuesStruct;
#define CALIBRATION_SIZE 8  // number of calibration values

calibrationValuesStruct calibration;
calibrationValuesStruct default_calibration;

/** default values assume that for both channels:
 * - voltage maximum is  30 V scaled to 2.5 V (4095)
 * - current maximum is 2.5 A scaled to 2.5 V (4095)
 * calibration values are then:
 * - (a) Vol = 4095 : 30000 mV = 7.326 mV / digit = 7326 uV/d
 * - (b) offset = 0
 * - (a) Cur = 4095 : 2500 mA =  0.6405 mA / digit = 641 uA/d
 * because of that calibration values are to be interpreted as uV and uA respectively
 */

// read calibration values from nvram and write to calibration array
void nvReadCalibration (void) {
  uint8_t counter;
  uint8_t mem_address[1] = {CALIBRATION_ADDRESS};
  uint16_t tempvalues[CALIBRATION_SIZE] = {0}; // holds the temporay read values
  if (sizeof(nvCalibrationValues) > (CALIBRATION_SIZE * 2)){
    return; // if the array is bigger than the values to read return
  }
  // get values from I2C EEPROM, send buffer contains memory address to start reading from
  i2cMasterTransmitTimeout(&I2CD1,
                           NV_DEVADDR,
                           mem_address, sizeof(mem_address),
                           nvCalibrationValues, sizeof(nvCalibrationValues),
                           TIME_MS2I(NV_TIMEOUT_MS)
                          );
  // assemble collected values and write values to temporay array
  for (counter = 0; counter < sizeof(nvCalibrationValues); counter++){
    // division by 2 increases counter for tempvalues every second increment of "counter"
    // values are assembled low byte first "&1" checks for odd counter and shifts accordingly ("<<3" == 2^3 == 8)
    tempvalues[(counter/2)] |=  nvCalibrationValues[counter] << ((counter & 1) << 3 );
  }
  calibration.calVolAa = tempvalues[0];
  calibration.calVolAb = tempvalues[1];
  calibration.calVolBa = tempvalues[2];
  calibration.calVolBb = tempvalues[3];
  calibration.calCurAa = tempvalues[4];
  calibration.calCurAb = tempvalues[5];
  calibration.calCurBa = tempvalues[6];
  calibration.calCurBb = tempvalues[7];

  // first run calibration


  default_calibration.calVolAa = 7326;
  default_calibration.calVolAb = 0;
  default_calibration.calVolBa = 7326;
  default_calibration.calVolBb = 0;
  default_calibration.calCurAa = 640;
  default_calibration.calCurAb = 0;
  default_calibration.calCurBa = 640;
  default_calibration.calCurBb = 0;

  if ((calibration.calVolAa == 0xFFFF) && (calibration.calVolAb == 0xFFFF)){
    calibration.calVolAa = default_calibration.calVolAa;
    calibration.calVolAb = default_calibration.calVolAb;
    calibration.calVolBa = default_calibration.calVolBa;
    calibration.calVolBb = default_calibration.calVolBb;
    calibration.calCurAa = default_calibration.calCurAa;
    calibration.calCurAb = default_calibration.calCurAb;
    calibration.calCurBa = default_calibration.calCurBa;
    calibration.calCurBb = default_calibration.calCurBb;
  }
}

// write values from calibration array to nvmemory
void nvWriteCalibration (uint8_t *calValues, uint8_t arraysize) {
  uint8_t counter;
  if (arraysize < (CALIBRATION_SIZE * 2)){
    return; // if the array is smaller than the values to write return
  }
  for (counter = 0; counter < arraysize; counter++){

  }
}

void nvReadSettings (void) {
  uint8_t counter;
  uint16_t tempvalues[SETTINGS_SIZE] = {0}; // holds the temporay read values
  uint8_t mem_address[1] = {SETTINGS_ADDRESS};

  if (sizeof(nvSettingValues) > (SETTINGS_SIZE * 2)){
    return; // if the array is bigger than the values to read return
  }
  // get values from I2C EEPROM
  i2cAcquireBus(&I2CD1);
  i2cMasterTransmitTimeout(&I2CD1,
                           NV_DEVADDR,
                           mem_address, sizeof(mem_address),
                           nvSettingValues, sizeof(nvSettingValues),
                           TIME_MS2I(NV_TIMEOUT_MS)
                          );
  i2cReleaseBus(&I2CD1);

  // assemble collected values and write values to temporay array
  for (counter = 0; counter < sizeof(nvSettingValues); counter++){
    // division by 2 increases counter for tempvalues every second increment of "counter"
    // values are assembled low byte first "&1" checks for odd counter and shifts accordingly ("<<3" == 2^3 == 8)
    tempvalues[(counter/2)] |=  nvSettingValues[counter] << ((counter & 1) << 3 );
  }
  settings.setVolA = tempvalues[0];
  settings.setVolB = tempvalues[1];
  settings.setCurA = tempvalues[2];
  settings.setCurB = tempvalues[3];
}

void nvWriteSettings (void) {
  uint8_t counter;
  uint16_t tempvalues[SETTINGS_SIZE] = {0};         // holds the temporay write values,
  uint8_t transmit_data[(SETTINGS_SIZE * 2)+1]; // +1 for the offset address
  transmit_data[0] = SETTINGS_ADDRESS;    // first byte of data is offset address

  if (sizeof(nvSettingValues) < (SETTINGS_SIZE * 2)){
    return; // if the array is smaller than the values to read return
  }
  tempvalues[0] = settings.setVolA;
  tempvalues[1] = settings.setVolB;
  tempvalues[2] = settings.setCurA;
  tempvalues[3] = settings.setCurB;

  // disassemle setting values to byte array
  for (counter = 0; counter < (sizeof(transmit_data)-1); counter++){
    // we need to start at postion counter +1 because [0] contains offset address
    // counter divded by 2 to address 16 bit values two times
    // shift contents of tempvalues 8 times ("<<3" == 2^3) to the right depending on odd counter values
    transmit_data[counter+1] = (uint8_t) (tempvalues[counter/2] >> ((counter & 1) << 3));
  }

  // send values to I2C EEPROM
  i2cMasterTransmitTimeout(&I2CD1,
                           NV_DEVADDR,
                           transmit_data,
                           sizeof(transmit_data),
                           NULL, 0, // no receive data buffer
                           TIME_MS2I(NV_TIMEOUT_MS));
}

#endif /** NVMEM C */
