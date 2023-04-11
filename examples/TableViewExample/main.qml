import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.12

import QCC.DataView.TableView 1.0 as QCC

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    QCC.TableView {
        anchors.fill: parent
        model: table_model
    }
}

