import QtQuick 2.0
import QCC.Controls 1.0

Rectangle {
    border.color: "black"
    border.width: 0

    // сетка
    Rectangle {
        x: parent.width - width
        width: 1; height: parent.height
        color: parent.border.color
    }

    Rectangle {
        y: parent.height - height
        width: parent.width; height: 1
        color: parent.border.color
    }
}
