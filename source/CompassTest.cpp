/**
 * Compass test that shows North after calibration.
 *
 * @copyright (c) Calliope gGmbH.
 *
 * Licensed under the Apache Software License 2.0 (ASL 2.0)
 *
 * @author Matthias L. Jugel <leo@calliope.cc>
 */

#include "MicroBit.h"
#include "BMX055Magnetometer.h"
#include "BMX055Accelerometer.h"

MicroBit uBit;

MicroBitImage N = MicroBitImage(
        "0,0,0,0,0\n"
        "0,1,1,1,0\n"
        "0,1,0,1,0\n"
        "0,1,0,1,0\n"
        "0,0,0,0,0\n");
MicroBitImage E = MicroBitImage(
        "0,0,0,0,0\n"
        "0,1,1,1,0\n"
        "0,1,1,0,0\n"
        "0,1,1,1,0\n"
        "0,0,0,0,0\n");
MicroBitImage S = MicroBitImage(
        "0,0,0,0,0\n"
        "0,0,1,1,0\n"
        "0,0,1,0,0\n"
        "0,1,1,0,0\n"
        "0,0,0,0,0\n");
MicroBitImage W = MicroBitImage(
        "0,0,0,0,0\n"
        "0,1,0,1,0\n"
        "0,1,1,1,0\n"
        "0,1,1,1,0\n"
        "0,0,0,0,0\n");

void on_compass(MicroBitEvent e) {
    (void) e;
    int heading = uBit.compass.heading();

    uBit.display.clear();
    if (heading < 11 || heading > 348) {
        uBit.display.print(N);
        uBit.display.image.setPixelValue(2, 0, 128);
        uBit.serial.send("N   ");
    } else if (heading < 34) {
        uBit.display.print(N);
        uBit.display.image.setPixelValue(1, 0, 128);
        uBit.serial.send("NNE ");
    } else if (heading < 56) {
        uBit.display.image.setPixelValue(0, 0, 128);
        uBit.serial.send("NE  ");
    } else if (heading < 78) {
        uBit.display.print(E);
        uBit.display.image.setPixelValue(0, 1, 128);
        uBit.serial.send("ENE ");
    } else if (heading < 102) {
        uBit.display.print(E);
        uBit.display.image.setPixelValue(0, 2, 128);
        uBit.serial.send("E   ");
    } else if (heading < 123) {
        uBit.display.print(E);
        uBit.display.image.setPixelValue(0, 3, 128);
        uBit.serial.send("ESE ");
    } else if (heading < 146) {
        uBit.display.image.setPixelValue(0, 4, 128);
        uBit.serial.send("SE  ");
    } else if(heading < 168) {
        uBit.display.print(S);
        uBit.display.image.setPixelValue(1, 4, 128);
        uBit.serial.send("SSE ");
    } else if (heading <192) {
        uBit.display.print(S);
        uBit.display.image.setPixelValue(2, 4, 128);
        uBit.serial.send("S   ");
    } else if(heading < 214) {
        uBit.display.print(S);
        uBit.display.image.setPixelValue(3, 4, 128);
        uBit.serial.send("SSW ");
    } else if (heading < 236) {
        uBit.display.image.setPixelValue(4, 4, 128);
        uBit.serial.send("SW  ");
    }else if(heading < 259) {
        uBit.display.print(W);
        uBit.display.image.setPixelValue(4, 3, 128);
        uBit.serial.send("WSW ");
    } else if (heading < 281) {
        uBit.display.print(W);
        uBit.display.image.setPixelValue(4, 2, 128);
        uBit.serial.send("W   ");
    }else if(heading < 304) {
        uBit.display.print(W);
        uBit.display.image.setPixelValue(4, 1, 128);
        uBit.serial.send("WNW ");
    } else if(heading < 326) {
        uBit.display.image.setPixelValue(4, 0, 128);
        uBit.serial.send("NW  ");
    } else {
        uBit.display.print(N);
        uBit.display.image.setPixelValue(3, 0, 128);
        uBit.serial.send("NNW  ");
    }
    uBit.serial.send(heading);
    uBit.serial.send("º\r\n");
}

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')


void rawTest() {
    MicroBitI2C i2c(I2C_SDA0, I2C_SCL0);

    i2c.writeRegister(BMX055_A_DEFAULT_ADDR, BMX055_A_BGW_SOFTRESET, 0xB6);
    wait_ms(100);

    // Softreset magnetometer, ends up in sleep mode, so wake up afterwards
    i2c.writeRegister(BMX055_M_DEFAULT_ADDR, BMX055_M_PWR_CNTL1, 0x82);
    wait_ms(100);
    i2c.writeRegister(BMX055_M_DEFAULT_ADDR, BMX055_M_PWR_CNTL1, 0x01);
    wait_ms(100);

    i2c.writeRegister(BMX055_M_DEFAULT_ADDR, BMX055_M_PWR_CNTL2, 0 << 3);
    i2c.writeRegister(BMX055_M_DEFAULT_ADDR, BMX055_M_INT_EN_2, 0b10000100);
    printf("detected: %d\r\n", BMX055Magnetometer::isDetected(i2c));

    wait_ms(2000);
    MicroBitPin int1(MICROBIT_ID_IO_INT1, p23, PIN_CAPABILITY_STANDARD);

    printf("\033[2J");
    while(1) {
        printf("\033[H");
        for(int i = 0x52; i >= 0x40; i--) {
          int bits = i2c.readRegister(BMX055_M_DEFAULT_ADDR, i);
          printf("%02x %02x [" BYTE_TO_BINARY_PATTERN "]\r\n", i, bits, BYTE_TO_BINARY(bits));
        }
    }
}

int main() {
    uBit.init();
    uBit.serial.send("Calliope Compass Demo v2.0\r\n");
    uBit.messageBus.listen(MICROBIT_ID_COMPASS, MICROBIT_COMPASS_EVT_DATA_UPDATE, &on_compass);
    uBit.compass.heading();
    while(1) uBit.sleep(1000);
}