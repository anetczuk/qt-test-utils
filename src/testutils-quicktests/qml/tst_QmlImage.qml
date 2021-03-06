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
import QtQuick.Controls 2.2

import testutils 1.0
import "qrc:/qml"
//import "qrc:/qml/js/TestUtils.js" as TestUtils


Rectangle {
    id: root

    color: "red"
    width: 300
    height: 300


    CustomTestCase {
        name: "QmlImage"
        when: windowShown

        function test_grab_size() {
            var img = ImageLoader.grabImage(root);

            compare( img.width(), root.width );
            compare( img.height(), root.height );
        }

        function test_color() {
            var img = ImageLoader.grabImage(root);

            compare( img.red(0,0), 255 );
            compare( img.green(0,0), 0 );
            compare( img.blue(0,0), 0 );
            compare( img.alpha(0,0), 255 );
        }

        function test_grab_compare() {
            var img = ImageLoader.grabImage(root);
            var img2 = grabImage(root)
            verify( img.equals(img2) );
        }

        function test_save_load() {
            var imgPath = output_root_dir + "/test_grab_image.png";
            var img = ImageLoader.grabImage(root);
            img.save( imgPath );

            var img2 = ImageLoader.loadImage( imgPath );
            verify( img.equals(img2) );
        }

    }
}
