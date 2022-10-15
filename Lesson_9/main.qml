import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Window 2.12
import QtQuick.Layouts 1.1
import QtQuick.LocalStorage 2.0
import to.save.task 1.0
import to.sort.task 1.0

ApplicationWindow {
    id:root
    visible: true
    height: 768
    width: 1024
    title: qsTr("Task manager")



    toolBar: ToolBar {
        RowLayout {
            id: rowLayout
            anchors.top: parent.top
            width: 150
            x: 5

            anchors.margins: 5
            height: 30

            spacing: 5
            z:2

            TextField {
                id: taskBox

                placeholderText: "Task..."
                validator: RegExpValidator
                anchors.fill: parent
                selectByMouse: true
                anchors.margins: 4
                focus: true
                horizontalAlignment: Text.AlignHLeft
                verticalAlignment: Text.AlignVCenter
                KeyNavigation.tab: deadlineBox
            }
        }
        RowLayout {
            id: rowLayout2
            anchors.top: parent.top
            width: 100
            x:160

            anchors.margins: 5
            height: 30

            spacing: 5
            z:2

            TextField {
                validator: RegExpValidator {
                        regExp: /[0-9]{2}-[0-9]{2}-[0-9]{4}/
                    }
                id: deadlineBox

                placeholderText: "dd-mm-yyyy"
                anchors.fill: parent
                anchors.margins: 4
                focus: true
                horizontalAlignment: Text.AlignHLeft
                verticalAlignment: Text.AlignVCenter
                KeyNavigation.tab: progressBox
            }
        }
        RowLayout {
            id: rowLayout3
            anchors.top: parent.top
            width: 100
            x:270

            anchors.margins: 5
            height: 30

            spacing: 5
            z:2

            TextField {
                validator: RegExpValidator {
                        regExp: /[0-9]{2}/
                    }
                id: progressBox

                placeholderText: "0 to 10..."
                anchors.fill: parent
                anchors.margins: 4
                focus: true
                horizontalAlignment: Text.AlignHLeft
                verticalAlignment: Text.AlignVCenter
                KeyNavigation.tab: addtaskButton
            }
        }

        RowLayout {
            id: rowLayout4
            anchors.top: parent.top
            width: 100
            x:380

            anchors.margins: 5
            height: 30

            spacing: 5
            z:2
            Button {
                //anchors.left:rowLayout3.right
                text: "Add task"
                id:addtaskButton
                Layout.fillWidth: true
                Layout.fillHeight: true
                KeyNavigation.tab: searchBox
                onClicked: {
                    sourceModel.append({"task": taskBox.text, "deadline": deadlineBox.text, "progress": progressBox.text})
                }
            }
        }

        RowLayout {
            id: rowLayout5
            anchors.top: parent.top
            width: 300
            x:485

            anchors.margins: 5
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            height: 30

            spacing: 5
            z:2

            TextField {
                id: searchBox

                placeholderText: "Search..."
                inputMethodHints: Qt.ImhNoPredictiveText

                Layout.fillWidth: true
                Layout.fillHeight: true
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                KeyNavigation.tab: taskBox
            }
        }
    }

    TableView {
        id: tableView
        frameVisible: false
        sortIndicatorVisible: true

        anchors.fill: parent

        Layout.minimumWidth: 400
        Layout.minimumHeight: 240
        Layout.preferredWidth: 600
        Layout.preferredHeight: 400


        TableViewColumn {
            id: taskColumn
            title: "Task"
            role: "task"
            movable: false
            resizable: false
            width: tableView.viewport.width - deadlineColumn.width - progressColumn.width
        }

        TableViewColumn {
            id: deadlineColumn
            title: "Deadline"
            role: "deadline"
            movable: false
            resizable: false
            width: tableView.viewport.width / 5
        }

        TableViewColumn {
            id: progressColumn
            title: "Progress"
            role: "progress"
            movable: false
            resizable: false
            width: tableView.viewport.width / 5
        }


        model: SortFilterProxyModel {
            id: proxyModel
            source: sourceModel.count > 0 ? sourceModel : null

            sortOrder: tableView.sortIndicatorOrder
            sortCaseSensitivity: Qt.CaseInsensitive
            sortRole: sourceModel.count > 0 ? tableView.getColumn(tableView.sortIndicatorColumn).role : ""

            filterString: "*" + searchBox.text + "*"
            filterSyntax: SortFilterProxyModel.Wildcard
            filterCaseSensitivity: Qt.CaseInsensitive
        }

        ListModel {
            id: sourceModel
        }


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
}
