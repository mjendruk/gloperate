
import QtQuick 2.0

import gloperate.base 1.0


/**
*  Connectors
*
*  Draws the connections between stages
*/
Item
{
    property Item stages:    null
    property Item pipeline:  null
    property int  arrowSize: Ui.style.pipelineConnectorSize / 3.0

    Canvas
    {
        id: canvas

        anchors.fill: parent

        antialiasing: true

        onPaint:
        {
            var ctx = canvas.getContext('2d');

            ctx.clearRect(0, 0, width, height);

            drawConnectors(ctx);
        }

        function drawConnectors(ctx)
        {
            // Get pipeline
            var pl     = pipeline.getStage(pipeline.path);
            var plDesc = pl.getDescription();

            // Get stages
            for (var i in plDesc.stages)
            {
                var name = plDesc.stages[i];

                // Get stage
                var st     = pl[name];
                var stDesc = st.getDescription();

                // Get connections of the stage
                var connections = st.getConnections();
                for (var j in connections)
                {
                    // Get connection
                    var connection = connections[j];
                    var from = connection.from;
                    var to   = connection.to;

                    var p0 = pipeline.getSlotPos(from, 'output');
                    var p1 = pipeline.getSlotPos(to, 'input');

                    if (p0 != null && p1 != null)
                    {
                        drawConnector(ctx, p0, p1);
                    }
                }
            }
        }

        function drawConnector(ctx, p0, p1)
        {
            var x0 = p0.x;
            var y0 = p0.y;
            var x1 = p1.x;
            var y1 = p1.y;

            ctx.strokeStyle = '#000000';
            ctx.lineWidth   = 2;

            ctx.beginPath();
            ctx.moveTo(x0, y0);
            ctx.bezierCurveTo(x0, y0, x0 + 100, y0, (x0 + x1) / 2.0, (y0 + y1) / 2.0);
            ctx.bezierCurveTo((x0 + x1) / 2.0, (y0 + y1) / 2.0, x1 - 100, y1, x1, y1);
            ctx.stroke();

            ctx.beginPath();
            ctx.moveTo(x1, y1);
            ctx.lineTo(x1 - arrowSize, y1 - arrowSize);
            ctx.lineTo(x1 - arrowSize, y1 + arrowSize);
            ctx.lineTo(x1, y1);
            ctx.fill();
        }
    }

    function requestPaint()
    {
        canvas.requestPaint();
    }
}