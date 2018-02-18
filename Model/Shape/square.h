#ifndef RECT_H
#define RECT_H

#include "shape.h"

namespace SegWiz {
    namespace Model {
        namespace Shape {
            class Square : public Shape
            {
            public:
                explicit Square(const DrawingBuffer *annotator);
                void draw(QPainter* painter, const QPoint& position);
            };
        }
    }
}

#endif // RECT_H
