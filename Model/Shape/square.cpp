#include "square.h"

#include "../drawingbuffer.h"

#include <QPainter>

namespace SegWiz {
    namespace Model {
        namespace Shape {
            Square::Square(const DrawingBuffer *annotator): Shape(annotator)
            {

            }

            void Square::draw(QPainter* painter, const QPoint& position)
            {
                quint16 shapeSize = m_annotator->shapeSize();
                painter->drawRect(QRectF(qreal(position.x()) - qreal(shapeSize) / 2, qreal(position.y()) - qreal(shapeSize) / 2, shapeSize, shapeSize));
            }

            QString Square::name() const
            {
                return QString("Square");
            }
        }
    }
}
