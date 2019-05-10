
function listProperties(item) {
    console.log("Properties of object:", item);
    var props = Object.keys(item);
    for( var i in props) {
        var p = props[i];
        var val = item[p];
        console.log("  [" + i + "]", p + ": " + val, "type:", typeof val);
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
