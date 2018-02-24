#ifndef ROUNDEDSQUARE_H
#define ROUNDEDSQUARE_H

#include "shape.h"

namespace SegWiz {
    namespace Model {
        namespace Shape {
            class RoundedSquare : public Shape
            {
            public:
                explicit RoundedSquare(const DrawingBuffer *annotator);
                void draw(QPainter* painter, const QPoint& position) override;
                QString name() const override;
            };
        }
    }
}

#endif // ROUNDEDSQUARE_H
