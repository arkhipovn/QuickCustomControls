import QtQuick 2.9
import QtQuick.Controls 2.9
import QtQuick.Controls.Material 2.9
import QtQuick.Controls.Material.impl 2.12

import QCC.Controls 1.0 as QCC
import QCC.Controls.Private 1.0

Item {
    id: control
    width: 120
    // минимум textField.height
    // а дальше идет выбор между размером textField.height + suggestionListView.height + textField.margin и maximumHeight
    //    height: Math.max(textField.height, Math.min(textField.height + suggestionListView.height + textField.margin, (maximumHeight > 0 ? maximumHeight : Infinity)))


    // нужно ли делать реальную высоту.. Или привязать ее к высоте textField.. Что-то подумтаь..
    property bool fixedHeight: false


    // textField.margin - потому что снизу еще отступ будет
    height: fixedHeight ? textField.height
                        : textField.height+ suggestionListView.height + textField.margin //+ elevationEffect.elevation * 2

    property int maximumHeight: -1

    signal selected(var index)

    signal accepted()
    signal editingFinished()

    property alias text: textField.text
    property alias placeholderText: textField.placeholderText
    property alias model: suggestionsList.model

    // Максимальное кол-во предложений
    property alias maximumNumberOfSuggestions: suggestionsList.maximumNumberOfSuggestions


    property alias readOnly: textField.readOnly

    property alias color: textField.color
    property alias accentColor: textField.accentColor
    property alias hintColor: textField.hintColor


    // Нужно ли перемещать подсказку наверх
    property alias floatingLabel: textField.floatingLabel

    property alias validator: textField.validator

    property alias selectByMouse: textField.selectByMouse
    property alias acceptableInput: textField.acceptableInput
    property alias echoMode: textField.echoMode
    property alias length: textField.length
    property alias maximumLength: textField.maximumLength

    property alias elevation: textField.elevation

    QCC.TextField {
        id: textField
        z: 2

        width: control.width

        onAccepted: control.accepted()
        onEditingFinished: control.editingFinished()

        property bool isSelected: false

        Keys.onUpPressed: {
            if(!suggestionListView.visible) return
            if(suggestionListView.currentIndex > 0)
                suggestionListView.currentIndex = suggestionListView.currentIndex - 1
            else suggestionListView.currentIndex = suggestionListView.count - 1
        }

        Keys.onDownPressed: {
            if(!suggestionListView.visible) return
            if(suggestionListView.currentIndex > suggestionListView.count - 2)
                suggestionListView.currentIndex = 0
            else suggestionListView.currentIndex = suggestionListView.currentIndex + 1
        }

        Keys.onReturnPressed: {
            if(suggestionListView.currentIndex != -1)
                suggestionListView.currentItem.select()
        }

        onTextEdited: {
            if(textField.isSelected)
                textField.isSelected = false
        }
    }

    Rectangle {
        id: borderRec
        visible: suggestionListView.visible
        width: control.width - 2; height: suggestionListView.height + (textField.height - y) + textField.margin
        anchors.horizontalCenter: parent.horizontalCenter
        y: textField.height * 0.2
        radius: 3
        //        border.width: 1
        //        border.color: control.hintColor


        layer.enabled: true//control.enabled && control.elevation > 0
        layer.effect: ElevationEffect {
            id: elevationEffect
            elevation: control.elevation == 0 ? 2
                                              : control.elevation
        }
    }

    ListView {
        id: suggestionListView
        visible: textField.activeFocus && count > 0 && !textField.isSelected

        anchors.top: textField.bottom
        width: parent.width - 2


        height: visible ? (control.maximumHeight <= 0 ? contentHeight
                                                      : Math.min(contentHeight, control.maximumHeight - textField.height - textField.margin) )
                        : 0

        clip: true

        flickableDirection: ListView.VerticalFlick
        boundsBehavior: ListView.StopAtBounds

        currentIndex: -1

        model: SuggestionsList {
            id: suggestionsList
            text: textField.text
        }

        delegate: suggestionComponent
        highlight: highlightComponent
    }

    // подстветка при наведии мыши
    Component {
        id: highlightComponent

        Rectangle {
            opacity: 0.05
            color: "black"

            Behavior on opacity {
                NumberAnimation { duration: 100; }
            }
        }
    }

    property Component dataComponent: Label {
        width: parent.width
        color: textField.color
        text: currentText
        verticalAlignment: Label.AlignVCenter
        wrapMode: Label.WordWrap
    }

    Component {
        id: suggestionComponent

        Item {
            width: suggestionListView.width; height: textField.implicitBackgroundHeight

            function select() {
                control.selected(suggestionIndex)
                textField.text = suggestionModelData
                textField.isSelected = true
            }

            Loader {
                id: loader

                anchors { fill: parent; leftMargin: textField.margin; rightMargin: textField.margin; }
                sourceComponent: dataComponent

                property int index: suggestionIndex
                property string currentText: suggestion
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: suggestionListView.currentIndex = index
                onExited: suggestionListView.currentIndex = -1
                onClicked: select()
            }
        }
    }
}
