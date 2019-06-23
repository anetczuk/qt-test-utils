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


function listProperties(item) {
    console.log("Properties of object:", item);
    var props = Object.keys(item);
    for( var i in props) {
        var p = props[i];
        var val = item[p];
        console.log("  [" + i + "]", p + ": " + val, "type:", typeof val);
    }
}

function listChildren(item) {
    console.log("Children of object:", item);
    var children = item.children;
    var props = Object.keys(children);
    for( var i in props) {
        var p = props[i];
        var val = children[p];
        console.log("  [" + i + "]", p + ": " + val);
    }
}

function getRoot( item ) {
    var obj = item;
    while( obj ) {
        var par = obj.parent;
        if ( !par )
            return obj;
        obj = par;
    }
    return obj;
}
