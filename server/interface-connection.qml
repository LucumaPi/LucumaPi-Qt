import QtQuick 2.0

Item {
    Rectangle {
        id: rectangle
        x: 0
        y: 0
        width: 667
        height: 480
        color: "#fce94f"
    }

    Image {
        id: image1
        x: 404
        y: 238
        width: 100
        height: 100
        fillMode: Image.PreserveAspectFit
        source: "img/plc-s7-1200.jpg"
    }

    Image {
        id: image2
        x: 270
        y: 41
        width: 100
        height: 100
        fillMode: Image.PreserveAspectFit
        source: "img/iot-2000.jpeg"
    }

    Image {
        id: image
        x: 74
        y: 71
        width: 100
        height: 100
        fillMode: Image.PreserveAspectFit
        source: "img/server-or-cloud.jpeg"
    }

    Image {
        id: image3
        x: 189
        y: 238
        width: 100
        height: 100
        fillMode: Image.PreserveAspectFit
        source: "img/laptop-client01.jpg"
    }

    Image {
        id: image4
        x: 463
        y: 71
        width: 100
        height: 100
        fillMode: Image.PreserveAspectFit
        source: "img/smartphone-client.png"
    }


}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
