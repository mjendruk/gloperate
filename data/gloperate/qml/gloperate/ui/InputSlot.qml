
import QtQuick 2.0
import gloperate.base 1.0
import gloperate.ui 1.0


/**
*  InputSlot
*
*  Representation of an input slot
*/
BaseItem
{
    id: item

    implicitWidth:  row.implicitWidth  + 2 * row.anchors.margins
    implicitHeight: row.implicitHeight + 2 * row.anchors.margins

    property alias name:        label.text
    property int   radius:      Ui.style.pipelineConnectorSize
    property color color:       Ui.style.pipelineConnectorColorIn
    property color borderColor: Ui.style.pipelineLineColor
    property int   borderWidth: Ui.style.pipelineLineWidth

    Row
    {
        id: row

        anchors.left: parent.left
        anchors.top:  parent.top
        spacing:      Ui.style.paddingSmall

        Rectangle
        {
            id: connector

            anchors.verticalCenter: parent.verticalCenter
            width:                  item.radius
            height:                 item.radius

            radius:       width / 2.0
            color:        item.color
            border.color: item.borderColor
            border.width: item.borderWidth
        }

        Label
        {
            id: label

            anchors.verticalCenter: parent.verticalCenter

            text:  'Value'
            color: Ui.style.pipelineTextColor
        }
    }
}