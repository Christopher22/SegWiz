#include "circle.h"

#include "../drawingbuffer.h"

#include <QPainter>

namespace SegWiz {
    namespace Model {
        namespace Shape {
            Circle::Circle(const DrawingBuffer *annotator): Shape(annotator)
            {

            }

            void Circle::draw(QPainter* painter, const QPoint& position)
            {
                painter->drawEllipse(position, m_annotator->shapeSize(), m_annotator->shapeSize());
            }
        }
    }
}

