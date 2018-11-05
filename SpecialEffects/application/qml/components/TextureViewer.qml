import QtQuick 2.1
import QtQuick.Controls 1.1

import "qrc:/qml/components"

import MyCppComponents 1.0

TextureRenderer {
  id: root

  ComboBox {
    id: disp_mode

    anchors.top: parent.top
    anchors.left: parent.left
    anchors.margins: 5

    width: 96

    opacity: 0.7

    model: ["byId","byName"]
  }

  ComboBox {
    anchors.top: parent.top
    anchors.right: parent.right
    anchors.margins: 5

    width: 128

    opacity: 0.7

    model: disp_mode.currentText === "byId" ? root.textureIds : root.textureNames;

    onCurrentTextChanged: disp_mode.currentText === "byId" ? root.setTextureId(currentText) : root.setTextureName(currentText);
  }

  ComboBox {
    anchors.bottom: parent.bottom
    anchors.right: parent.right
    anchors.margins: 5

    width: 72

    opacity: 0.7

    model: [1,10,100,1000];

    onCurrentTextChanged: root.setColorMagnification(currentText)
  }
}
