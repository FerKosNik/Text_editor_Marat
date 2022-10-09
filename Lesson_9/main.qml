import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Window 2.12
import QtQuick.Layouts 1.1
import QtQuick.LocalStorage 2.0
import to.save.task 1.0

ApplicationWindow {
    id:root
    visible: true
    width: 640
    height: 480
    title: qsTr("Органайзер")

    RowLayout {
        id: rowLayout
        anchors.top: parent.top
        width: 150

        anchors.margins: 5
        height: 30

        spacing: 5
        z:2

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "white"

            TextInput {
                id: textInput
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
    RowLayout {
        id: rowLayout2
        anchors.top: parent.top
        width: 65
        x:160

        anchors.margins: 5
        height: 30

        spacing: 5
        z:2

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "white"

            TextInput {

                id: textInput2
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
    RowLayout {
        id: rowLayout3
        anchors.top: parent.top
        width: 65
        x:230

        anchors.margins: 5
        height: 30

        spacing: 5
        z:2

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "white"

            TextInput {

                id: textInput3
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
    }

    Button {
        anchors.left:rowLayout3.right
        text: "Добавить задачу"
        id:exButton
        width: 300
        height: 30
        onClicked: {
            listModel.append({ textList: textInput.text })
            listModel2.append({ textList2: textInput2.text })
            listModel3.append({ textList3: textInput3.text })
        }
    }

    TableView {

        id: listView

        anchors.top: rowLayout.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 5
        z: 1

        TableViewColumn
        {
            role: "Name_task"
            title: "Название задачи"
        }

        Text
        {
            text: textList
        }
        model: ListModel
        {
            id: listModel
        }
    }
    TableView {

        id: listView2

        anchors.top: rowLayout.bottom
        x:100
        width:80
        anchors.bottom: parent.bottom
        anchors.margins: 5
        z: 1

        TableViewColumn
        {
            role: "Name_task"
            title: "Дедлайн (в формате dd.mm.yyyy)"
        }
        Text
        {
            text: textList2
        }
        model: ListModel
        {
            id: listModel2
        }
    }
    TableView {

        id: listView3

        anchors.top: rowLayout.bottom
        x:230
        width:70
        anchors.bottom: parent.bottom
        anchors.margins: 5
        z: 1

        TableViewColumn
        {
            role: "Name_task"
            title: "Текущий прогресс (от 0 до 10)"
        }
        Text
        {
            text: textList3
        }
        model: ListModel
        {
            id: listModel3
        }
    }

    property string sc: 'ElementUser{width:600; height:150;'
    property string nc: 'NewData{width:600; height:150;}'
    property int num: 0
    var taskName = textInput.text
    var deadLine = textInput2.text
    var curProgress = textInput3.text
    property NewData newData: value
    function addThisDate(taskName, deadLine, curProgress)
    {
        loader.writeNewInformation(taskName, deadLine, curProgress)
        var itog = sc + 'Task:"' + taskName + '";Deadline:"' + deadLine + '";Current Progress' + curProgress
        var el = Qt.createQmlObject(itog, list, "obj" + num++)
    }
    SaveTask{
        id : loader
        onInitEnd:
        {
            if (succeed)
            {
                newData = Qt.createQmlObject(nc, list, "objdata")
                loader.getNextTask()
            } else Qt.quit()
        }
        onLoadTask: {
            var itog = sc + 'Task:"' + taskName + '";Deadline:"' + deadLine + '";Current Progress' + curProgress
            var el = Qt.createQmlObject(itog, list, "obj" + num++)
                loader.getNextTask()
        }
    }
        function add(taskName, deadLine, curProgress) {
            root.addThisDate(taskName, deadLine, curProgress)
            console.log(taskName)
            console.log(deadLine)
            console.log(curProgress)
        }
}
