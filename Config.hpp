/*
 * The MIT License
 *
 * Copyright 2017-2018 azarias.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/*
 * File:   Config.hpp
 * Author: azarias
 *
 * Created on 1/2/2019
 */
#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <qglobal.h>

enum WeatherCommand : quint8 {
    STOP_START_MODE = 0x0,
    START_MODE_1 = 0x1,
    START_MODE_2 = 0x2,
    START_MODE_3 = 0x3,
    GET_DATA = 0x4,
    CONFIGURE_FE_1 = 0x5,
    CONFIGURE_FE_2 = 0x6,
    CONFIGURE_FE_3 = 0x7,
    CONFIGURE_MODE_2 = 0x8,
    SEND_MODE1_DATA = 0x9,
    SEND_MODE2_DATA = 0xA
};


constexpr quint16 VALUE_MASK     = 0b0000001111111111;
constexpr quint16 SENSORID_MASK  = 0b0000110000000000;

typedef quint32 timestamp_t;
constexpr quint8 TIMESTAMP_SIZE = sizeof(timestamp_t) / 8;

struct Configuration{
    quint8 freq1;
    quint8 freq2;
    quint8 freq3;
    quint8 mode2Time;
};

#endif // CONFIG_HPP
