import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.12

import QCC.AppManager 1.0 as QCM
import QCC.Controls 1.0 as QCC
import QCC.Fonts 1.0
import QCC.Validators 1.0
import QCC.Dialogs 1.0 as QD
//import QtQuick.Controls 1.4 as CC

//import GradientImage 1.0


ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("QCC Controls")

    //        font.pixelSize: 30
    
    //    Column {
    //        width: parent.width
    //        anchors.verticalCenter: parent.verticalCenter
    //        spacing: 20

    //        GradientImage {
    //            width: parent.width
    //            height: 20
    //        }

    ////        QCC.WaitingBar {
    ////            width: parent.width
    ////            height: 20
    ////        }
    //    }

    QCC.CWaitingBar {
        width: parent.width
        height: 20
        running: true
        //        duration: 5000
    }




    /*
    Column {
        id: column
        //        width: parent.width
        anchors { fill: parent; margins: 20; }
        //        Component.onCompleted: print(CFontLoader.awesome.name)
        spacing: 20

        QCC.TextField {
            //            floatingLabel: false
            validator: IntValidator {
                bottom: 5
                top: 100
            }
            text: "1"
            message: acceptableInput ? "" : "Message"
            placeholderText: "Header"
        }


        QCC.TextField {
            id: passwordField
            placeholderText: qsTr("Password")

            property bool checked: false
            echoMode: checked ? TextInput.Normal : TextInput.Password

            rightAreaDelegate: QCC.SymbolButton {
                width: passwordField.height * 0.8; height: width
                anchors.verticalCenter: parent.verticalCenter

                symbol: checked ? "\uf06e" : "\uf070"
                checkable: true
                color: checked ? Material.accent : Material.backgroundDimColor

                onClicked: passwordField.checked = checked
                toolTip.text: checked ? qsTr("Hide password") : qsTr("Show password")
            }
        }


        QCC.DateInput {
            id: di
            format: "dd.MM.yyyy hh:mm"
            value: "10.10.1010 10:10"
            placeholderText: "Date"
        }


        Row {
            QCC.SymbolButton {
                symbol: "\uf070"
                text: "Text"
            }

            QCC.SymbolButton {
                symbol: "\uf06e"
            }
        }


        Button {
            text: "Color dialog"
            onClicked: colorDialogLoader.active = true
        }

        //        CC.Calendar {

        //        }


        //        TimePicker {

        //        }
    }


    Loader {
        id: colorDialogLoader
        active: false
        sourceComponent: QD.ColorDialog {
            //        anchors.fill: parent
            onAccepted: print(color)
            onRejected: print("rej")
            onClosing: colorDialogLoader.active = false
        }
    }
*/


    

}
