import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import GradientImage 1.0

Item {
    Rectangle {
        id: colorArea
        width: parent.width * 0.3
        height: parent.height * 0.3
        color: canvas.currentColor
        border.width: 1
    }

    Pane {
        anchors { left: colorArea.right; right: parent.right; top: parent.top; bottom: parent.bottom; margins: 10; }
        Material.elevation: 3
        padding: 0

        Canvas
        {
            id: canvas
            anchors.fill: parent

            property color currentColor: !available ? "black" : getPixelData(colorItemMouseArea.canvasX, colorItemMouseArea.canvasY)

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

            onPaint:
            {
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
        }



        MouseArea {
            id: colorItemMouseArea
            anchors.fill: parent;

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


    /*
    QLinearGradient horGradient(offset, 0, boundingrect.width() - offset, 0);
    QGradientStops stops;
    stops.append(QGradientStop(0.0, "#ff0000"));
    stops.append(QGradientStop(0.2, "#ffff00"));
    stops.append(QGradientStop(0.4, "#00ff00"));
    stops.append(QGradientStop(0.6, "#00ffff"));
    stops.append(QGradientStop(0.7, "#0000ff"));
    stops.append(QGradientStop(1.0, "#ff00ff"));
    horGradient.setStops(stops);

    QLinearGradient verGradient(0, offset, 0, boundingrect.height() - offset);
    QGradientStops stops2;
    stops2.append(QGradientStop(0.0, Qt::black));
    stops2.append(QGradientStop(0.4, Qt::transparent));
    stops2.append(QGradientStop(1.0, Qt::white));
    verGradient.setStops(stops2);
*/
    /*
    GradientImage {
        id: gradientImage
        anchors { left: colorArea.right; right: parent.right; top: parent.top; bottom: parent.bottom; margins: 10; }
        //        width: 500; height: 300


        MouseArea {
            id: colorItemMouseArea
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.CrossCursor

            //            onMouseXChanged: image.getPixelData(mouseX, mouseY)
            //            onMouseYChanged: image.getPixelData(mouseX, mouseY)
        }
    }*/




    /*Item {
        id: colorItem
        anchors.fill: parent
        Rectangle {
            id: colorRectangle
            anchors.fill: parent
            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop { position: 0.0; color: "#ff0000" }
                GradientStop { position: 0.2; color: "#ffff00" }
                GradientStop { position: 0.4; color: "#00ff00" }
                GradientStop { position: 0.6; color: "#00ffff" }
                GradientStop { position: 0.8; color: "#0000ff" }
                GradientStop { position: 1.0; color: "#ff00ff" }
            }
        }
        Rectangle {
            id: overLayRectangle
            anchors.fill: parent
            gradient: Gradient {
                orientation: Gradient.Vertical
                GradientStop { position: 0.0; color: "#000000" }
                GradientStop { position: 0.5; color: "transparent" }
                GradientStop { position: 1.0; color: "#ffffff" }
            }
        }
    }
    */

    /*
    Image {
        anchors.fill: parent
        fillMode: Image.Tile

        Gradient {

            orientation: Gradient.Horizontal
            GradientStop { position: 0.0; color: "#ff0000" }
            GradientStop { position: 0.2; color: "#ffff00" }
            GradientStop { position: 0.4; color: "#00ff00" }
            GradientStop { position: 0.6; color: "#00ffff" }
            GradientStop { position: 0.8; color: "#0000ff" }
            GradientStop { position: 1.0; color: "#ff00ff" }
        }
    }
    */

    /*
    onWidthChanged: storeImage()
    onHeightChanged: storeImage()

    Component.onCompleted: storeImage()

    property var currentGradientImage: ""
    property color currentColor: "black"

    function storeImage() {
        // This operation could be costly.
        console.log("Saving colorItem -> currentGradientImage.")
        colorItem.grabToImage( function(result) { currentGradientImage = result.url } )
    }

    Canvas {
        id: image
        anchors.fill: colorItem
        onPaint: {
            // All what we do in paint is just draw stored image if we have any.
            var ctx = getContext("2d")
            if (currentGradientImage) {
                ctx.drawImage(currentGradientImage, 0, 0)
            }
        }

        function getPixelData(x, y) {
            // Extract (x,y)-pixel color from our canvas and store as currentColor.
            var ctx = getContext("2d")
            const pixel = ctx.getImageData(x, y, 1, 1)
            const data = pixel.data
            const r = data[0]/255
            const g = data[1]/255
            const b = data[2]/255
            const a = data[3]/255
            currentColor = Qt.rgba(r, g, b, a)
        }
    }

    Item {
        id: colorItem
        anchors.fill: parent
        Rectangle {
            id: colorRectangle
            anchors.fill: parent
            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop { position: 0.0; color: "#ff0000" }
                GradientStop { position: 0.2; color: "#ffff00" }
                GradientStop { position: 0.4; color: "#00ff00" }
                GradientStop { position: 0.6; color: "#00ffff" }
                GradientStop { position: 0.8; color: "#0000ff" }
                GradientStop { position: 1.0; color: "#ff00ff" }
            }
        }
        Rectangle {
            id: overLayRectangle
            anchors.fill: parent
            gradient: Gradient {
                orientation: Gradient.Vertical
                GradientStop { position: 0.0; color: "transparent" }
                GradientStop { position: 1.0; color: "#ffffff" }
            }
        }
    }

    MouseArea {
        id: colorItemMouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.CrossCursor
        onMouseXChanged: image.getPixelData(mouseX, mouseY)
        onMouseYChanged: image.getPixelData(mouseX, mouseY)
    }

    CheckBox {
        // Checkbox for debug. Make it visible and check to see what is drawn on canvas image.
        visible: false; checked: false
        onCheckedChanged: colorItem.visible = !checked
    }



    Rectangle {
        id: colorRect
        // Draw colorRect near current cursor position
        x: colorItemMouseArea.mouseX + 2
        y: colorItemMouseArea.mouseY + 2
        width: 30
        height: width
        radius: width / 10
        color: currentColor
        border {
            width: 2
            color: "black"
        }
    }
*/


    /*
    Rectangle {
        width: 100
        height: 100
        rotation: -90
        //        transformOrigin: Item.TopLeft
        gradient: Gradient {
            id: gr
            GradientStop {position: 0.000; color: Qt.rgba(1, 0, 0, 1)}
            GradientStop {position: 0.167; color: Qt.rgba(1, 1, 0, 1)}
            GradientStop {position: 0.333; color: Qt.rgba(0, 1, 0, 1)}
            GradientStop {position: 0.500; color: Qt.rgba(0, 1, 1, 1)}
            GradientStop {position: 0.667; color: Qt.rgba(0, 0, 1, 1)}
            GradientStop {position: 0.833; color: Qt.rgba(1, 0, 1, 1)}
            GradientStop {position: 1.000; color: Qt.rgba(1, 0, 0, 1)}
        }
    }
*/


}

/*
ApplicationWindow {
    visible: true
    width: 500; height: 400
    flags: Qt.Dialog


    Rectangle {
        width: 100
        height: 100
//        rotation: -90
//        transformOrigin: Item.TopLeft
        gradient: Gradient {
            GradientStop {position: 0.000; color: Qt.rgba(1, 0, 0, 1)}
            GradientStop {position: 0.167; color: Qt.rgba(1, 1, 0, 1)}
            GradientStop {position: 0.333; color: Qt.rgba(0, 1, 0, 1)}
            GradientStop {position: 0.500; color: Qt.rgba(0, 1, 1, 1)}
            GradientStop {position: 0.667; color: Qt.rgba(0, 0, 1, 1)}
            GradientStop {position: 0.833; color: Qt.rgba(1, 0, 1, 1)}
            GradientStop {position: 1.000; color: Qt.rgba(1, 0, 0, 1)}
        }
    }


    Rectangle {
        anchors.fill: parent

        // note we smoothscale the shader from a smaller version to improve performance
//        ShaderEffect {
//            id: map
//            width: 64
//            height: 64
//            //                            opacity: alphaSlider.value
//            //                            scale: paletteMap.width / width;
//            layer.enabled: true
//            layer.smooth: true
//            anchors.centerIn: parent
//            //                            property real hue: hueSlider.value

//            fragmentShader:  "
//                            varying mediump vec2 qt_TexCoord0;
//                            uniform highp float qt_Opacity;
//                            uniform highp float hue;

//                            highp float hueToIntensity(highp float v1, highp float v2, highp float h) {
//                                h = fract(h);
//                                if (h < 1.0 / 6.0)
//                                    return v1 + (v2 - v1) * 6.0 * h;
//                                else if (h < 1.0 / 2.0)
//                                    return v2;
//                                else if (h < 2.0 / 3.0)
//                                    return v1 + (v2 - v1) * 6.0 * (2.0 / 3.0 - h);

//                                return v1;
//                            }

//                            highp vec3 HSLtoRGB(highp vec3 color) {
//                                highp float h = color.x;
//                                highp float l = color.z;
//                                highp float s = color.y;

//                                if (s < 1.0 / 256.0)
//                                    return vec3(l, l, l);

//                                highp float v1;
//                                highp float v2;
//                                if (l < 0.5)
//                                    v2 = l * (1.0 + s);
//                                else
//                                    v2 = (l + s) - (s * l);

//                                v1 = 2.0 * l - v2;

//                                highp float d = 1.0 / 3.0;
//                                highp float r = hueToIntensity(v1, v2, h + d);
//                                highp float g = hueToIntensity(v1, v2, h);
//                                highp float b = hueToIntensity(v1, v2, h - d);
//                                return vec3(r, g, b);
//                            }

//                            void main() {
//                                lowp vec4 c = vec4(1.0);
//                                c.rgb = HSLtoRGB(vec3(hue, 1.0 - qt_TexCoord0.t, qt_TexCoord0.s));
//                                gl_FragColor = c * qt_Opacity;
//                            }
//                            "
//        }


    }




}
*/

/*
                    sourceComponent: ColorDialog {
                        visible: true
                        title: qsTr("Please choose a color")
                        onAccepted: {
                            var s = chartViewArea.getSeries(index)
                            if(s === null) return
                            s.color = color
                            colorDialogLoader.active = false
                        }
                        onRejected: {
                            colorDialogLoader.active = false
                        }
                    }
*/
