import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Rectangle {
  id: root


  opacity: 0.7
  border.color: "black"
  border.width: 5
  radius: 10


  ScrollView {
    anchors.fill: parent
    anchors.margins: 10

    ListView {

      id: view
      model: visu_group_model

      anchors.fill: parent
      anchors.margins: 10
      clip: true
    }
  }

  VisualDataModel {
    id: visu_group_model
    model: hidmanager_model
    delegate:
        ColumnLayout {
          id: group_column
          spacing: 0

          Text { text: name; font.bold: true; font.underline: true}

          Repeater {
            Layout.fillWidth: true
            model: VisualDataModel {
              id: test_model
              model: hidmanager_model
              rootIndex: visu_group_model.modelIndex(index)
              delegate:   Item {
                property bool show_description: false
                implicitHeight: col.height
                implicitWidth: view.width

                opacity: 0.8
                anchors.margins: 10

                Rectangle {
                  id: hvr_backg
                  anchors.fill: parent
                  visible: false
                  color: "gray"
                  opacity: 0.5

                  radius: 3
                }

                ColumnLayout {
                  id: col
                  spacing: 0
                  width: parent.width

                  RowLayout {
                    Item { width: 15 }
                    Item {
                        height: hid_name_text.height;
                        Layout.fillWidth: true
                        Text{ id: hid_name_text; text: name ? name : "Name N/A" }
                    }
                    Item {
                        height: hid_binding_text.height;
                        implicitWidth: hid_binding_text.contentWidth;
                        Text{
                          id: hid_binding_text;
                          text: binding ? binding : "Binding N/A"
                          font.bold: true
                        }
                    }
                    Item { width: 15 }
                  }
                  RowLayout {
                    visible: show_description
                    Item { width: 20}
                    Item {
                      id: hid_description
                      implicitHeight: hid_description_text.height;
                      Layout.fillWidth: true;

                      Text{
                        id: hid_description_text;
                        text: description ? description : "Description N/A";
                        width: hid_description.width // width must be explicit for Text wraping
                        wrapMode: Text.WordWrap
                        font.italic: true; color: "gray"
                      }
                    }
                    Item { width: 5 }
                  }
                  Item { height: 5 }
                }

                MouseArea {
                  anchors.fill: parent

                  hoverEnabled: true
                  onClicked: show_description = !show_description
                  onEntered: hvr_backg.visible = true
                  onExited: hvr_backg.visible = false
                }
              }
            }
          }
          Item { height: 15}
        }
  }

}
