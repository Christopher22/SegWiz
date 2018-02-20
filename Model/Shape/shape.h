#ifndef SHAPE_H
#define SHAPE_H

#include <QPoint>
#include <QString>

class QPainter;

namespace SegWiz {
    namespace Model {
        class DrawingBuffer;

        namespace Shape {
            class Shape
            {
            public:
                explicit Shape(const DrawingBuffer *annotator);
                virtual ~Shape() {}

                virtual void draw(QPainter* painter, const QPoint& position) = 0;
                virtual QString name() const = 0;
            protected:
                const DrawingBuffer *m_annotator;
            };
        }
    }
}

#endif // SHAPE_H
