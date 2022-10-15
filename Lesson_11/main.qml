import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Органайзер")

    RowLayout {
        id: rowLayout
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5

        spacing: 10

        Text {text: qsTr("Задание")}
        TextField {id: taskField}
        Text {text: qsTr("Дедлайн")}
        TextField { id: deadlineField}
        Text {text: qsTr("Прогресс")}
        TextField {id: progressField}

        Button {
            text: qsTr("Добавить")

            onClicked: {
                database.inserIntoTable(taskField.text , deadlineField.text, progressField.text)
                myModel.updateModel()
            }
        }
    }

    TableView {
        id: tableView
        anchors.top: rowLayout.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 5

        TableViewColumn {
            role: "task"
            title: "Задание"
        }
        TableViewColumn {
            role: "deadline"
            title: "Дедлайн"
        }
        TableViewColumn {
            role: "progress"
            title: "Прогресс"
        }

        model: myModel

        rowDelegate: Rectangle {
            anchors.fill: parent
            color: styleData.selected ? 'skyblue' : (styleData.alternate ? 'whitesmoke' : 'white');
            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.RightButton | Qt.LeftButton
                onClicked: {
                    tableView.selection.clear()
                    tableView.selection.select(styleData.row)
                    tableView.currentRow = styleData.row
                    tableView.focus = true

                    switch(mouse.button) {
                    case Qt.RightButton:
                        contextMenu.popup()
                        break
                    default:
                        break
                    }
                }
            }
        }
    }

    Menu {
        id: contextMenu

        MenuItem {
            text: qsTr("Удалить")
            onTriggered: {

                dialogDelete.open()
            }
        }
    }

    MessageDialog {
        id: dialogDelete
        title: qsTr("Удаление записи")
        text: qsTr("Подтвердите удаление записи из журнала")
        icon: StandardIcon.Warning
        standardButtons: StandardButton.Ok | StandardButton.Cancel

        onAccepted: {
            database.removeRecord(myModel.getId(tableView.currentRow))
            myModel.updateModel();
        }
    }

    Button {
                text: qsTr("Экспорт в Excel")
                onClicked: {
                    myModel.exportCSV()
                }
            }
}
