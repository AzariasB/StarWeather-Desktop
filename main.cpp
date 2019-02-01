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
 * File:   main.cpp
 * Author: azarias
 *
 * Created on 14/12/2018
 */
#include "MainWindow.hpp"
#include <QApplication>
#include <QDebug>
#include <QTimer>
#include <QtSerialPort>
#include <QInputDialog>
#include <QMessageBox>
#include "Communicator.hpp"

QString chooseSerialPort(){
    QStringList possiblePorts;

#ifdef QT_DEBUG
    if(QFile::exists("./virtual-tty"))
        possiblePorts.append("./virtual-tty");

#else


    const auto ports =  QSerialPortInfo::availablePorts();
    for(const auto &port : ports){
        possiblePorts.append(port.portName());
    }
#endif

    if(possiblePorts.isEmpty()){
        QMessageBox::warning(nullptr, "Aucun port detecté", "Aucun port série n'a été détecté");
        return "";
    }

    bool ok = false;
    QString chosen = QInputDialog::getItem(nullptr, "Port série", "Choix du port série", possiblePorts, 0, false, &ok);
    if(!ok) {
        return "";
    }

    return chosen;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qsrand(time(NULL));

    QString chosen = chooseSerialPort();
    if(chosen.isEmpty()) return EXIT_SUCCESS;

    QSerialPort port(chosen);
    if(!port.open(QIODevice::ReadWrite)){
        qWarning() << port.errorString() << "\n" << port.error();
        return EXIT_FAILURE;
    }
    Communicator c(port);


    MainWindow win(c);
    win.show();

    return a.exec();
}
