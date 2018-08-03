import QtQuick 2.0
import QtQuick.Window 2.1
import QtQuick.Controls 1.2

Rectangle {
    width: 1024
    height: 600

    Text {
        id: title
        x: 473
        y: 2
        textFormat:Text.AutoText
        wrapMode: Text.NoWrap
        text: qsTr("Basic Scene Control")
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 11

    }
    Rectangle {
        id: consoleContainer
        objectName: "consoleContainer"
        x: 605
        y: 40
        color: "transparent"
        border.width: 1
        width:405
        height: 170

        Text {
            id: consoleLabel
            x: 0
            y: -20
            width: 250
            text: qsTr("Program Messages")
            wrapMode: Text.NoWrap
            textFormat: Text.AutoText
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }

        ScrollView {
            clip: true
            anchors.fill: parent
            TextEdit {
                id: consoleOutput
                objectName: "consoleOutput"
                x: 5
                y: 5
                width: 395
                height: 3200
                text: qsTr("GUI started")
                clip: false
                wrapMode: TextInput.Wrap
                horizontalAlignment: Text.AlignLeft
                font.pixelSize: 12
                readOnly: true
                }
            }
    }

    Rectangle {
        id: vsContainer
        x: 10
        y: 240
        color: "transparent"
        border.width: 1
        width:430
        height: 355

        Text {
            id: vsLabel
            x: 0
            y: -20
            width: 250
            text: qsTr("Vetex Shader Source")
            wrapMode: Text.NoWrap
            textFormat: Text.AutoText
            horizontalAlignment: Text.AlignLeft
            font.pixelSize: 12
        }

        ScrollView {
            clip: true
            anchors.fill: parent

            TextEdit {
                id: vertexTextInput
                objectName: "vertexTextInput"
                x: 5
                y: 5
                width: 425
                height: 1200
                text: "attribute vec4 inPos;\nattribute vec4 inColor;\n\nvarying vec4 oColor;\n\nvoid main()\n{\n oColor = inColor;\n gl_Position = inPos;\n}"
                smooth: false
                transformOrigin: Item.Center
                textFormat: Text.PlainText
                cursorVisible: true
                clip: false
                wrapMode: TextInput.Wrap
                horizontalAlignment: Text.AlignLeft
                font.pixelSize: 12
            }
        }
    }

    Rectangle {
        id: fsContainer
        x: 545
        y: 240
        color: "transparent"
        border.width: 1
        width: 465
        height: 355

        Text {
            id: fslabel
            x: 0
            y: -20
            width: 250
            text: qsTr("Fragment Shader Source")
            wrapMode: Text.NoWrap
            textFormat: Text.AutoText
            horizontalAlignment: Text.AlignLeft
            font.pixelSize: 12
        }

        ScrollView {
            clip: true
            anchors.fill: parent

            TextEdit {
                id: fragmentTextInput
                objectName: "fragmentTextInput"
                x: 5
                y: 5
                width: 452
                height: 1200
                text: qsTr("varying highp vec4 oColor;

void main()
{
 gl_FragColor = oColor;
}")
                clip: false
                wrapMode: TextInput.Wrap
                horizontalAlignment: Text.AlignLeft
                font.pixelSize: 12
            }
        }
    }

    Rectangle {
        id: clearConsoleMessages
        x: 885
        y: 5
        width: 130
        height: 30
        color: "#e4e4e4"
        border.width: 1
        Text {
            id: clearConsoleLabel
            x: 25
            y: 10
            text: qsTr("Clear Messages")
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }

        MouseArea {
            id: clearConsoleMouse
            width: 130
            height: 30

            onClicked: basicController.clearConsoleText()
        }
    }

    Rectangle {
        id: updateShaders
        x: 445
        y: 240
        width: 95
        height: 95
        color: "#e4e4e4"
        border.width: 1
        Text {
            id: updateShadersLabel
            x: 0
            y: 41
            width: 95
            height: 20
            text: qsTr("Update Shaders")
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }

        MouseArea {
            id: updateShadersMouse
            x: 0
            y: 0
            width: 95
            height: 95
            onClicked: basicController.updateShaderData()
        }
    }

    Rectangle {
        id: updateVertexData
        x: 10
        y: 10
        width: 160
        height: 32
        color: "#e4e4e4"
        border.width: 1

        Text {
            id: updateVertexDataLabel
            x: 13
            y: 10
            text: qsTr("Update Vertex Data")
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12

        }

        MouseArea {
            id: updateVertexDataMouse
            width: 160
            height: 32
            onClicked: basicController.updateVertexData()
        }
    }

    Rectangle {
        id: positionAttribute

        x: 10
        y: 45
        width: 270
        height: 165
        color: "#ffffff"

        border.width: 1

        Text {
            id: positionAttributeLabel
            x: 50
            y: 8
            text: qsTr("Position. Bind Name : inPos")
            font.pixelSize: 12
        }

        ScrollView {
            clip: true
            anchors.fill: parent

            TextEdit {
                id: positionInText
                objectName: "positionInText"
                x: 5
                y: 25
                width: 265
                height: 600
                text: qsTr("#Positions -> this line is required\n#format: X Y Z in [-1.0, 1.0] on each line\n0.0 0.5 0.0\n-0.5 -0.5 0.0\n0.5 -0.5 0.0\n")
                font.family: "Times New Roman"
                cursorVisible: true
            }
        }
    }

    Rectangle {
        id: positionAttribute1
        x: 295
        y: 45
        width: 270
        height: 165
        color: "#ffffff"
        Text {
            id: positionAttributeLabel1
            x: 50
            y: 8
            text: qsTr("Color. Bind Name : inColor")
            font.pixelSize: 12
        }

        ScrollView {
            clip: true
            anchors.fill: parent

            TextEdit {
                id: colorInText
                objectName: "colorInText"
                x: 8
                y: 25
                width: 265
                height: 600
                text: qsTr("#Colors -> this line is required\n#format: A R G B in [1.0, 1.0] on each line \n1.0 0.0 0.0 1.0\n0.0 1.0 0.0 1.0\n0.0 0.0 1.0 1.0\n")
                font.pixelSize: 12
            }
        }
        border.width: 1
    }

    Text {
        id: vertexDataLabel
        x: 260
        y: 20
        text: qsTr("Vertex Data")
        font.pixelSize: 12
    }

    Rectangle {
        id: drawMode
        x: 445
        y: 363
        width: 95
        height: 95
        color: "#e4e4e4"
        border.width: 1

        Text {
            id: renderModeText
            objectName: "renderModeText"
            x: 0
            y: 41
            width: 95
            height: 20
            text: qsTr("Draw:Triangles")
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }
        MouseArea {
            id: updateDrawModeMouse
            x: 0
            y: 0
            width: 95
            height: 95
            onClicked: basicController.updateDrawMode()
        }
    }

    Rectangle {
        id: cullMode
        x: 445
        y: 463
        width: 95
        height: 95
        color: "#e4e4e4"
        border.width: 1

        Text {
            id: cullModeText
            objectName: "cullModeText"
            x: 0
            y: 41
            width: 95
            height: 20
            text: qsTr("Cull:BackFace")
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }
        MouseArea {
            id: updateCullModeMouse
            x: 0
            y: 0
            width: 95
            height: 95
            onClicked: basicController.updateCullMode()
        }
    }


}
