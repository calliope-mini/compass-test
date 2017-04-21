/**
 * Calliope Demo Code.
 *
 * The code contains is the wrapper for the main functionality found after
 * unpacking the Calliope mini. It contains the basic test functionality for
 * in-production testing, as well as some small demo programs that can be
 * selected using A and B buttons.
 *
 * - Oracle (press a button and get a smiley or sad face)
 * - Rock, Paper, Scissors, Well
 * - Love Meter (Pin 1 and 2)
 * - Snake (adapted from the original microbit-samples)
 * - ((game of life))
 * - ((proximty hearts))
 *
 * The last two require the CalliopeDemoMaster to be compiled and put on
 * an extra board. Then those games can be activated via Radio.
 *
 * @copyright (c) Calliope gGmbH.
 *
 * Licensed under the Apache Software License 2.0 (ASL 2.0)
 * Portions (c) Copyright British Broadcasting Corporation under MIT License.
 *
 * @author Matthias L. Jugel <leo@calliope.cc>
 * @author Stephan Noller <stephan@calliope.cc>
 * @author Franka Futterlieb <franka@urbn.de>
 * @author Niranjan Rao
 */

#include "MicroBit.h"
#include "CalliopeDemo.h"

#ifndef COMPILE_FIRMWARE_MASTER

MicroBit uBit;

int main() {
    // Initialise the micro:bit runtime.
    uBit.init();
    uBit.serial.baud(115200);
    uBit.serial.send("Calliope Compass Demo\r\n");
    // disabled the test board procedure as it may confuse users who burn the initial firmware
    // call the test board procedure, will return if done already

   uBit.accelerometer.configure();
   uBit.serial.baud(115200);
   int id = uBit.compass.whoAmI();
            uBit.serial.send("found id=");
            uBit.serial.send(id);
            uBit.serial.send("\r\n");
   uBit.compass.calibrate();

    while(1)
    {
            int x = uBit.compass.heading();
            uBit.serial.send("heading=");
            uBit.serial.send(x);
            uBit.serial.send("\r\n");
//          serial.send("y=");
//          serial.send(ry);
//          serial.send("\r\n");

        uBit.sleep(100);
    }

}

#endif
