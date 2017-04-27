/**
 * Compass test that shows North after calibration.
 *
 * @copyright (c) Calliope gGmbH.
 *
 * Licensed under the Apache Software License 2.0 (ASL 2.0)
 * Portions (c) Copyright British Broadcasting Corporation under MIT License.
 *
 * @author Matthias L. Jugel <leo@calliope.cc>
 * @author Joerg Wedeck
 */

#include "MicroBit.h"

MicroBit uBit;

// draw a line
void line(int x0, int y0, int x1, int y1) {

    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2, e2;

    for (;;) {
        uBit.display.image.setPixelValue(x0, y0, 255);
        if (x0 == x1 && y0 == y1) break;
        e2 = err;
        if (e2 > -dx) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dy) {
            err += dx;
            y0 += sy;
        }
    }
}

int main() {
    uBit.init();
    uBit.serial.baud(115200);
    uBit.serial.send("Calliope Compass Demo\r\n");

    // do some calibration
    uBit.compass.calibrate();

    while (1) {
        int heading = uBit.compass.heading();

        uBit.display.clear();
        // calculate the point on the circle, center [2,2] radius 3
        const int x1 = (int) round(2 + 3 * cos(-heading * PI / 180));
        const int y1 = (int) round(2 + 3 * sin(-heading * PI / 180));
        printf("[%d,%d]\r\n", x1, y1);

        line(2, 2, x1, y1);
        uBit.sleep(100);
    }

}