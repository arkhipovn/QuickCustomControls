import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Material.impl 2.12

Pane {
    id: control
    padding: 0

    property int elevation: 3
    readonly property color currentColor: !canvas.available ? "black" : canvas.getPixelData(colorItemMouseArea.canvasX, colorItemMouseArea.canvasY)

    background: Rectangle {
        layer.enabled: /*control.enabled && */control.elevation > 0
        layer.effect: ElevationEffect {
            elevation: control.elevation
        }
    }

    Canvas {
        id: canvas
        anchors.fill: parent

        function getPixelData(x, y) {
            var ctx = getContext("2d")
            const pixel = ctx.getImageData(x, y, 1, 1)
            const data = pixel.data
            const r = data[0]/255
            const g = data[1]/255
            const b = data[2]/255
            //            const a = data[3]/255
            return Qt.rgba(r, g, b, 1)
        }

        onPaint: {
            var context = getContext('2d');
            context.rect(0, 0, canvas.width, canvas.height);
            var grd = context.createLinearGradient(0, 0, canvas.width, 0);
            grd.addColorStop(0, "#7FFF00" )
            grd.addColorStop(1/12, "#FFFF00" )
            grd.addColorStop(2/12, "#FF7F00" )
            grd.addColorStop(3/12, "#FF0000" )
            grd.addColorStop(4/12, "#FF007F" )
            grd.addColorStop(5/12, "#FF00FF" )
            grd.addColorStop(6/12, "#7F00FF" )
            grd.addColorStop(7/12, "#0000FF" )
            grd.addColorStop(8/12, "#007FFF" )
            grd.addColorStop(9/12, "#00FFFF" )
            grd.addColorStop(10/12, "#00FF7F" )
            grd.addColorStop(11/12, "#00FF00" )
            grd.addColorStop(1, "#7FFF00" )
            context.fillStyle = grd;
            context.fill();
            var grd2 = context.createLinearGradient(0, 0, 0, canvas.height);
            grd2.addColorStop(0, "#000000" )
            grd2.addColorStop(0.5, "transparent" )
            grd2.addColorStop(1, "#FFFFFF" )
            context.fillStyle = grd2;
            context.fill();
        }
    }   // Canvas

    MouseArea {
        id: colorItemMouseArea
        anchors.fill: parent;
        //        mouseX: 0; mouseY: 0
        property int canvasX: mouseX < canvas.x ? canvas.x:
                                                  mouseX > canvas.x + canvas.width ? canvas.x + canvas.width - 1
                                                                                   : mouseX;
        property int canvasY: mouseY < canvas.y ? canvas.y:
                                                  mouseY > canvas.y + canvas.height ? canvas.y + canvas.height - 1
                                                                                    : mouseY;
    }

    Rectangle {
        x: colorItemMouseArea.canvasX - width / 2; y: colorItemMouseArea.canvasY - height / 2;
        width: 10; height: 10; radius: width / 2
        border.width: 1
        color: "white"
    }
}



