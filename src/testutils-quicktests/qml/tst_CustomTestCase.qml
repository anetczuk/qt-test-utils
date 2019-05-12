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

import testutils 1.0

//import "qrc:/qml/js/TestUtils.js" as TestUtils
//import "qrc:/qml"


Rectangle {
    id: root

    color: "red"
    width: 300
    height: 300


    Column {

        anchors.fill: parent
        anchors.margins: 20


        Rectangle {

            color: "blue"
            width: 100
            height: 100


            CustomTestCase {
                name: "CustomTestCase"
                when: windowShown

                function test_grab() {
                    var img = this.saveImageOfItem( root );
                    verify( img !== null );
                }

                function test_compareImages_same_path() {
                    var status = this.compareImages("data/screens/CustomTestCaseScreen.png", "data/screens/CustomTestCaseScreen.png");
                    compare( status, 0 );
                }

                function test_compareImages_invalid_path() {
                    var status = this.compareImages("tests/screens/CustomTestCaseScreen.png_invalid", "data/screens/CustomTestCaseScreen.png");
                    compare( status, -1 );
                }

                function test_compareImages_invalid_path_B() {
                    var status = this.compareImages("tests/screens/CustomTestCaseScreen_invalid.png", "data/screens/CustomTestCaseScreen_invalid.png");
                    compare( status, 0 );
                }

                function test_compareImages_same_image_path() {
                    var img = this.makeScreenshot( root );
                    verify( img !== null );

                    var status = this.compareImages("tests/screens/CustomTestCaseScreen.png", "data/screens/CustomTestCaseScreen.png");
                    compare( status, 0 );
                }

                function test_compareImages_same_image_obj() {
                    var imgA = this.loadImage("data/screens/CustomTestCaseScreen.png");
                    verify( imgA !== null );

                    var imgB = this.makeScreenshot( root );
                    verify( imgB !== null );

                    var status = this.compareImages(imgA, imgB);
                    compare( status, 0 );
                }

                function test_compareImages_diff_image() {
                    var status = this.compareImages("data/red.png", "data/blue.png", test_compareImages_diff_image.name);
                    compare( status, -1 );
                }

                function test_compareScreenshots_default_valid() {
                    var status = this.compareScreenshots();
                    compare( status, 0 );
                }

                function test_compareScreenshots_item_valid() {
                    var status = this.compareScreenshots( root );
                    compare( status, 0 );
                }

            }
        }
    }

}
