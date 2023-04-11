import QtQuick 2.9
import QtQuick.Controls 2.9
import QtQuick.Controls.Material 2.9
import QtQuick.Controls.Material.impl 2.12


//@N переделать.. Сделать отступы через padding или insent
//Control {
Pane {
    id: control
    implicitWidth: 120 + leftArea.width + rightArea.width
    implicitHeight: messageLabel.height + Math.max(Material.buttonHeight + control.margin * 2, font.pixelSize * 3.4)
    padding: 0

    activeFocusOnTab: true
    hoverEnabled: true//!readOnly

    signal accepted()
    signal editingFinished()
    signal textEdited()

    readonly property bool acceptableInput: textInput.acceptableInput && !incorrectInput

    property alias text: textInput.text
    property alias placeholderText: placeholderLabel.text

    property alias readOnly: textInput.readOnly

    property alias selectByMouse: textInput.selectByMouse

    property bool incorrectInput: false     // Если нужно подстветить неправильный ввод не используя валидатор

    property alias echoMode: textInput.echoMode
    property alias length: textInput.length
    property alias maximumLength: textInput.maximumLength


    property color color: Material.foreground
    property color accentColor: Material.accentColor
    property color errorColor: Material.color(Material.Red)
    property color hintColor: Material.hintTextColor
    property color backgroundColor: "white"
    property int elevation: 0

    // Нужно ли перемещать подсказку наверх
    property bool floatingLabel: true

    // текст в самом низу
    property alias message: messageLabel.text
    property color messageColor: hintColor

    property alias validator: textInput.validator
    property alias inputMask: textInput.inputMask
    property alias inputMethodHints: textInput.inputMethodHints

    property alias verticalAlignment: textInput.verticalAlignment
    property alias horizontalAlignment: textInput.horizontalAlignment
    property alias cursorPosition: textInput.cursorPosition

    readonly property alias inputWidth: textInput.implicitWidth
    readonly property int backgroundWidth: textInput.implicitWidth + leftArea.implicitWidth + rightArea.implicitWidth + control.margin * 2

    property alias leftAreaDelegate: leftArea.sourceComponent
    property alias rightAreaDelegate: rightArea.sourceComponent

    // отступ у элементов
    property int margin: 6  //@N 6

    // какой масштаб будет у messageLabel
    property real messageScale: 0.8

    background: Item {
        id: backgroundItem
        implicitWidth: width; implicitHeight: borderRect.height + messageLabel.height + rightArea.width
        //        clip: true

        Item {
            id: fieldArea
            anchors { top: parent.top; bottom: messageLabel.top; }
            width: parent.width

            // Если ничего не ввели не будем отображать ошибку..
            readonly property bool acceptableInput: textInput.text.length == 0 ? true
                                                                               : control.acceptableInput

            Rectangle {
                id: borderRect

                anchors { top: parent.top; bottom: parent.bottom; margins: Math.max(control.font.pixelSize * 0.4, 6); } //@N 6
                width: parent.width - 2;
                anchors.horizontalCenter: parent.horizontalCenter
                radius: 3
                color: control.backgroundColor

                layer.enabled: control.enabled && control.elevation > 0
                layer.effect: ElevationEffect {
                    elevation: control.elevation
                }

                border {
                    width: control.elevation > 0 ? 0
                                                 : control.activeFocus ? 2 : 1
                    //@N color..

                    color: enabled ? (fieldArea.acceptableInput ? (control.activeFocus ? control.accentColor :
                                                                                         (control.hovered ? "#4F4F4F": control.hintColor))
                                                                : control.errorColor)
                                   : control.hintColor
                }

                Behavior on border.color {
                    id: colorBehavior
                    enabled: false
                    ColorAnimation { duration: 200 }
                }
                Component.onCompleted: colorBehavior.enabled = true
            }

            Label {
                id: placeholderLabel

                anchors { left: leftArea.active ? leftArea.right : undefined;
                    verticalCenter: parent.verticalCenter; }

                width: Math.min(implicitWidth,
                                parent.width -
                                (leftArea.active ? leftArea.width : 0) -
                                (rightArea.active ? rightArea.width: 0) - 2)

                elide: Qt.ElideRight
                //                rightPadding: rightArea.active ? 0 : control.margin

                leftInset: control.margin - 2
                visible: text.length > 0
                font: control.font
                color: control.hintColor


                leftPadding: leftArea.active ? 0 : control.margin
                rightPadding: 2

                background: Item {
                    Rectangle { width: parent.width; height: parent.height * 0.6; y: parent.height * 0.4; color: control.backgroundColor }
                }

                states: [
                    State {
                        name: "header"
                        when: placeholderLabel.visible && control.floatingLabel &&
                              (control.readOnly ? textInput.text.length != 0
                                                : textInput.text.length != 0 || textInput.activeFocus)



                        AnchorChanges {
                            target: placeholderLabel
                            anchors.verticalCenter: undefined
                            anchors.top: parent.top
                            anchors.left: leftArea.active ? parent.left : undefined;
                        }
                        PropertyChanges {
                            target: placeholderLabel
                            font.pixelSize: control.font.pixelSize * control.messageScale
                            color: enabled ? (fieldArea.acceptableInput ? control.accentColor : control.errorColor)
                                           : control.hintColor
                            leftPadding: control.margin
                        }
                    },
                    State {
                        name: "hidden"
                        when: !control.floatingLabel && textInput.text.length > 0
                        PropertyChanges {
                            target: placeholderLabel
                            visible: false
                        }
                    }
                ]

                transitions: [
                    Transition {
                        id: floatingTransition
                        enabled: false
                        AnchorAnimation { duration: 150 }
                        NumberAnimation {
                            duration: 150
                            property: "font.pixelSize"
                        }
                        ColorAnimation { duration: 150 }
                    }
                ]

                Component.onCompleted: floatingTransition.enabled = true
            }

            Loader {
                id: leftArea
                active: sourceComponent !== null
                anchors { left: parent.left; verticalCenter: parent.verticalCenter; }
            }

            TextInput {
                id: textInput
                anchors { left: leftArea.right; right: rightArea.left;
                    leftMargin: leftArea.active ? 0 : control.margin;
                    rightMargin: rightArea.active ? 0 : control.margin; }
                height: parent.height
                leftPadding: 1
                verticalAlignment: Label.AlignVCenter

                clip: true
                selectByMouse: true
                focus: true
                font: control.font
                color: enabled ? control.color : control.hintColor

                onAccepted: control.accepted()
                onTextEdited: control.textEdited()
                onEditingFinished: control.editingFinished()
            }

            Loader {
                id: rightArea
                active: sourceComponent !== null
                anchors { right: parent.right; verticalCenter: parent.verticalCenter; }
            }
        }

        Label {
            id: messageLabel
            visible: text.length > 0
            width: parent.width
            wrapMode: Label.WordWrap
            anchors.bottom: parent.bottom

            leftPadding: control.margin
            rightPadding: control.margin
            color: fieldArea.acceptableInput || !enabled ? control.messageColor : control.errorColor
            height: visible ? contentHeight : 0
            //                font: control.font    //@N
            font.pixelSize: control.font.pixelSize * control.messageScale
        }
    }
}


