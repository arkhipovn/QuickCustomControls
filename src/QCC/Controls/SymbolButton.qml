import QtQuick 2.9
import QtQuick.Controls 2.9
import QtQuick.Controls.Material 2.9
import QtQuick.Controls.Material.impl 2.12
import QCC.Fonts 1.0
import QtQuick.Controls.impl 2.12
import QtQuick.Templates 2.12 as T

import QtQuick.Layouts 1.12


T.Control {
    id: control

    activeFocusOnTab: true

    property bool active: false

    property bool scaleToContentSize: false // Чтобы размер был исходя из размера контента. А не control.Material.buttonHeight
    implicitWidth: Math.max((scaleToContentSize ? 0
                                                : control.Material.buttonHeight + leftInset + rightInset),
                            implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max((scaleToContentSize ? 0
                                                 : control.Material.buttonHeight + leftInset + rightInset),
                             implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    property alias symbol: iconLabel.text
    property alias color: iconLabel.color

    property real symbolScale: 1//0.9

    property alias family: iconLabel.font.family
    property alias size: iconLabel.font.pixelSize
    property alias symbolLabel: iconLabel

    property alias text: label.text
    property alias textColor: label.color
    property alias textLabel: label
    property alias textElide: label.elide

    property alias pressed: controlMouseArea.pressed
    property alias pressAndHoldInterval: controlMouseArea.pressAndHoldInterval
    property alias acceptedButtons: controlMouseArea.acceptedButtons
    property alias pressedButtons: controlMouseArea.pressedButtons


    property color highlightedRippleColor: Material.highlightedRippleColor
    property color rippleColor: Material.rippleColor


    signal pressAndHold(var mouse)
    signal doubleClicked(var mouse)
    signal clicked(var mouse)
    signal entered()
    signal exited()
    signal released(var mouse)

    property alias cursorShape: controlMouseArea.cursorShape

    property bool checkable: false
    property bool checked: false
    property bool radioButton: false

    property alias toolTip: toolTipItem

    property bool highlighted: false

    property bool clickable: true

    topInset: 6; bottomInset: 6
    padding: Math.max(topInset + bottomInset, 4) //6)
    horizontalPadding: Math.max(padding - 4, spacing)
    spacing: Math.max(6, padding * 0.5)

    property alias layoutDirection: contentLayout.layoutDirection
    property alias squareBackground: background.square

    contentItem: RowLayout {
        id: contentLayout
        spacing: control.spacing

        Label {
            id: iconLabel

            Layout.fillWidth: true
            Layout.maximumWidth: implicitWidth
            Layout.alignment: Qt.AlignHCenter

            visible: text.length > 0

            //                label.text.length > 0 ? Qt.AlignLeft
            //                                      : Qt.AlignHCenter

            //            Layout.fillHeight: true
            verticalAlignment: Text.AlignVCenter

            font.family: CFontLoader.awesome.name

            font.pixelSize: Math.max((Material.buttonHeight * 0.5), control.font.pixelSize * 1.3) * control.symbolScale

            //            font.pixelSize:  control.font.pixelSize * control.symbolScale

            color: Material.color(Material.Indigo)

            Behavior on color {
                ColorAnimation { duration: 200; }
            }
        }

        Label {
            id: label
            visible: text.length > 0
            font: control.font
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter
            elide: Label.ElideRight
        }
    }

    background: Ripple {
        id: background

        implicitWidth: control.size * 2 / control.symbolScale
        implicitHeight: control.size * 2 / control.symbolScale

        //        implicitWidth: Math.max(control.size * 2 / control.symbolScale, control.Material.buttonHeight) //control.Material.touchTarget
        //        implicitHeight: Math.max(control.size * 2 / control.symbolScale/*, control.Material.buttonHeight*/)
        property bool square: control.text.length == 0// || control.contentItem.width <= control.contentItem.height

        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        clip: !square
        width: square ? parent.height / 5 : parent.width
        height:  square ? parent.height / 5 : parent.height
        pressed: controlMouseArea.pressed && control.clickable
        anchor: control
        active: control.active || (control.enabled && (control.visualFocus || control.hovered || control.highlighted))
        color: control.highlighted ? control.highlightedRippleColor
                                   : control.rippleColor
    }

    MouseArea {
        id: controlMouseArea
        anchors.fill: parent
        enabled: clickable || hoverEnabled
        hoverEnabled: control.hoverEnabled
        onClicked: {
            control.forceActiveFocus()

            if(control.clickable) {
                if((mouse.button === Qt.LeftButton) && control.checkable) {
                    if(!checked) checked = true
                    else if(!radioButton) checked = false
                }
                control.clicked(mouse)
            }
        }
        onPressAndHold: if(control.clickable) control.pressAndHold(mouse)
        onDoubleClicked: if(control.clickable) control.doubleClicked(mouse)
        onEntered: control.entered()
        onExited: control.exited()
        onReleased: if(control.clickable) control.released(mouse)
    }

    ToolTip {
        id: toolTipItem
        Material.foreground: control.Material.primary    //@N
        visible: text.length > 0 && control.hovered
        timeout: 3000
        delay: 300
    }
}


