import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3


Window {
	visible: true

    width: 200
    height: 300


	MouseArea {
		anchors.fill: parent
		onClicked: {
            Qt.quit();
		}
	}

    Column {
        anchors.fill: parent
        spacing: 12
		
		Text {
            text: qsTr("Hello World")
		}
		
		Image {
            width: 100
            height: 100
            source: "qrc:/res/images/mandrill.jpeg"
		}

        Image {
            width: 100
            height: 100
            source: "qrc:/res/images/tulips.png"
        }
	}
}
