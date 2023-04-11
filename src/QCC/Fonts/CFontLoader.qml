pragma Singleton
import QtQuick 2.12

QtObject {
    readonly property QtObject awesome: FontLoader { source: "qrc:/fonts/fontawesome-webfont.ttf"; }
}
