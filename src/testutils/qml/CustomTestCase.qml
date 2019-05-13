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
import "js/TestUtils.js" as TestUtils


TestCase {
    id: control

    readonly property string output_root_dir: "tests"
    readonly property string data_root_dir:   "data"


    function getImagePath( dir, suffix ) {
        var filePath = dir;
        if ( dir.endsWith( "/" ) === false ) {
            filePath += "/";
        }
        filePath += control.name;
        if ( name.endsWith( suffix ) === false ) {
            filePath += suffix;
        }
        filePath += ".png";
        return filePath;
    }

    function getDataImagePath( subdir, suffix ) {
        return getImagePath( data_root_dir + "/" + subdir, suffix );
    }

    function getTestImagePath( subdir, suffix ) {
        return getImagePath( output_root_dir + "/" + subdir, suffix );
    }


    // ============================================================


    function loadImage( path ) {
        return ImageLoader.loadImage( path )
    }

    function saveImageOfItem( item, suffix ) {
        if (suffix === undefined) {
            suffix = "";
        }
        var path = getTestImagePath( "", suffix );
        return ImageLoader.saveImageOfItem( root, path )
    }

    function makeScreenshot(item) {
        var path = getTestImagePath( "screens", "Screen" );
        return ImageLoader.saveImageOfItem( root, path )
    }


    // ============================================================


    function compareImages(imgA, imgB, diffSuffix) {
        var imageA = _getImage(imgA);
        var imageB = _getImage(imgB);

        var result = imageA.equals( imageB )
        if (result === true)
            return 0;

        // generate diff file
        var imgDiff = ImageLoader.makeDiff( imageA, imageB );
        if (diffSuffix === undefined) {
            diffSuffix = imgDiff.hash();
        }
        var diffPath = getTestImagePath("", "-diff-" + diffSuffix);
        imgDiff.save( diffPath );
        console.log("diff image stored to:", diffPath);

        return -1;
    }

    function compareScreenshots( item, imagePath ) {
        if (item === undefined) {
            item = TestUtils.getRoot( this );
        }

        var screenImage = makeScreenshot( item );

        if (imagePath === undefined) {
            imagePath = getDataImagePath( "screens", "Screen" );
        }

        var loadedImg = ImageLoader.loadImage( imagePath );
        if (loadedImg.empty()) {
            console.debug("could not load image for test:", imagePath);
        }

        return compareImages( screenImage, imagePath );
    }


    // ============================================================


    function _getImage( img ) {
        if (typeof img === "string") {
            return ImageLoader.loadImage( img );
        } else {
            return img;
        }
    }

}
