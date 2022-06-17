import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 2.15
import QtQuick.Controls 2.15
import daniil.backend 1.0

Window {
    width: 440
    height: 600
    visible: true
    title: qsTr("Auth app")

    BackEnd {
        id: backend
    }

    ColumnLayout {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        spacing: 6

        Rectangle {
            Layout.fillWidth: true
            height: 50
            width: 120
            border.width: 1
            border.color: "#000000"

            TextInput {
                id: textInput
                anchors.centerIn: parent
                text: "Код сюда"
                font.pixelSize: 24
                activeFocusOnPress: true
                onFocusChanged: {
                    textInput.text = ""
                }
            }
        }

        Button {
            text: "Enter"
            Layout.fillWidth: true
            onClicked: {
                if (backend.checkCode(textInput.text.toUpperCase())) {
                    result.text = "Code is correct!"
                    result.color = "green"
                } else {
                    result.text = "Incorrect code!"
                    result.color = "red"
                }
            }
        }
        Text {
            id: result
            Layout.fillWidth: true
        }
    }
    ColumnLayout {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom

        Timer {
            // timer to count seconds
            interval: 1000
            running: true
            repeat: true
            onTriggered: {
                backend.Time -= 1
                time.text = "Time left: " + backend.Time + 's'
            }
        }
        Timer {
            // timer to update code
            interval: 10000
            running: true
            repeat: true
            onTriggered: {
                backend.Time += 10
                backend.regenerateCode()
            }
        }
        Text {
            id: code
            text: backend.Code
            font.pixelSize: 26
            Layout.fillWidth: true
        }
        Text {
            id: time
            Layout.fillWidth: true
        }
    }
}
