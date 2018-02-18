#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.h"

namespace SegWiz {
    namespace Model {
        namespace Shape {
            class Circle : public Shape
            {
            public:
                explicit Circle(const DrawingBuffer *annotator);
                void draw(QPainter* painter, const QPoint& position);
            };
        }
    }
}

#endif // CIRCLE_H
