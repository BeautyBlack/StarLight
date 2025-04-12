import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 2.12



Window {
    id: loginWindow

    visible: true
    width: 600
    height: 300
    color: "#ffffff"
    title: qsTr("登录")

    Text {
        id: hospitalNameText
        x: 10
        y: 10
        height: 50
        width: 250
        minimumPixelSize: 120
        text: qsTr("StarLight")
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignTop
        font.pixelSize: 26
    }

    Canvas {
        id: closeButtonCanvas
        width: 40
        height: 40
        y: 10
        x: 550
        onPaint: {
            var ctx = getContext("2d");
            draw(ctx);
        }

        function draw(ctx) {
            ctx.clearRect(closeButtonCanvas.left, closeButtonCanvas.top, closeButtonCanvas.width, closeButtonCanvas.height);

            ctx.fillStyle = "#ffffff";
            ctx.fillRect(closeButtonCanvas.left, closeButtonCanvas.top, closeButtonCanvas.width, closeButtonCanvas.height);

            ctx.fillStyle = "#000000";
            ctx.strokeStyle = "#000000"
            ctx.lineWidth = 2;

            ctx.beginPath();
            ctx.moveTo(closeButtonCanvas.left + 5, closeButtonCanvas.top + 5);
            ctx.lineTo(closeButtonCanvas.left + 5 + 30, closeButtonCanvas.top + 5 + 30);

            ctx.moveTo(closeButtonCanvas.left + 5 + 30, closeButtonCanvas.top + 5);
            ctx.lineTo(closeButtonCanvas.left + 5, closeButtonCanvas.top + 5 + 30);

        }
    }

    RowLayout {
		width: 400
        id: userLayout
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: hospitalNameText.bottom

        TextField {
            id: userTextField
            width: 300
            height: 45
            echoMode: 2
            anchors.horizontalCenter: parent.horizontalCenter
            placeholderText: qsTr("请输入用户名")
        }
    }


    RowLayout {
		width: 400
        id: passwordLayout
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: userLayout.bottom

        TextField {
            id: passwordTextField
            width: 300
            height: 45
            echoMode: 2
            anchors.horizontalCenter: parent.horizontalCenter
            placeholderText: qsTr("请输入密码")
        }
    }

    RowLayout {
		width: 400
        id: buttonLayout
        anchors.bottomMargin: 50
		anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom

        Button {
            id: loginButton
            width: 300
            height: 40
            text: qsTr("登 录")
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
