import QtQuick 2.0
import QtTest 1.2
import QtQuick.Controls 2.2

import testutils 1.0


Rectangle {
    id: root

    color: "red"
    width: 300
    height: 300


    TestCase {
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
            var imgPath = "test_grab_image.png";
            var img = ImageLoader.grabImage(root);
            img.save( imgPath );

            var img2 = ImageLoader.loadImage( imgPath );
            verify( img.equals(img2) );
        }

    }
}
