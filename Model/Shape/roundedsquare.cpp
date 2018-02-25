#include "roundedsquare.h"

#include "../drawingbuffer.h"

#include <QPainter>

namespace SegWiz {
    namespace Model {
        namespace Shape {
            RoundedSquare::RoundedSquare(const DrawingBuffer *annotator): Shape(annotator)
            {

            }

            void RoundedSquare::draw(QPainter* painter, const QPoint& position)
            {
                const quint16 shapeSize = m_annotator->shapeSize();
                painter->drawRoundedRect(QRectF(qreal(position.x()) - qreal(shapeSize) / 2, qreal(position.y()) - qreal(shapeSize) / 2, shapeSize, shapeSize), 25, 25, Qt::RelativeSize);
            }

            QString RoundedSquare::name() const
            {
                return QString("Squircle");
            }
        }
    }
}
