/// MIT License
///
/// Copyright (c) 2019 Arkadiusz Netczuk <dev.arnet@gmail.com>
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

import QtQuick 2.0
import QtTest 1.2

//import testutils 1.0
import "qrc:/qml"
//import "qrc:/qml/js/TestUtils.js" as TestUtils


Rectangle {
    id: root

    color: "red"
    width: 300
    height: 300


    Column {

        anchors.fill: parent
        anchors.margins: 20


        Rectangle {
            id: rectangle001

            color: contextRectangleColor
            width: 100
            height: 100

        }
    }

    CustomTestCase {
        name: "SetupExample"
        when: windowShown


        function test_width() {
            compare( rectangle001.width, 100 );
        }

        function test_height() {
            compare( rectangle001.height, 100 );
        }
        function test_contextColor() {
            compare( contextRectangleColor, "blue" );
            compare( rectangle001.color, "#0000ff" );
        }


        function test_screen() {
            var status = compareScreenshots();
            compare( status, 0 );
        }
    }
}
