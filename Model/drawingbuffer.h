#ifndef ANNOTATOR_H
#define ANNOTATOR_H

#include <QObject>
#include <QPicture>
#include <QVector>
#include <QSize>
#include <QPen>
#include <QPainter>

class QMouseEvent;
class QWheelEvent;

namespace SegWiz {
    namespace Model {
        namespace Shape {
            class Shape;
        }

        class Dataset;
        class Label;

        class DrawingBuffer : public QObject
        {
            Q_OBJECT
        public:
            static const quint16 MIN_SHAPE_SIZE = 1;
            static const quint16 MAX_SHAPE_SIZE = 100;

            explicit DrawingBuffer(Dataset *dataset, const QSize& size, QObject *parent = nullptr);
            virtual ~DrawingBuffer();

            void reset();
            void reset(const QSize& size);

            void handleMouse(QMouseEvent *event, bool movement);
            void handleMouse(QWheelEvent *event);

            const QPixmap& image() const;

            quint16 shapeSize() const;
            void setShapeSize(quint16 shapeSize);

            quint16 width() const;
            quint16 height() const;

            Shape::Shape *currentShape();
            bool setCurrentShape(quint16 shapeId);

            const Shape::Shape *shape(quint16 shapeId) const;
            quint32 shapes() const;

            bool annotateWithMovement() const;
            void setAnnotateWithMovement(bool annotateWithMovement);

        signals:
            void shapeChanged(const Shape::Shape* shape);

        private:
            Q_DISABLE_COPY(DrawingBuffer)

            void setLabel(const Label *label);
            void setEraser();

            QPixmap* m_buffer;
            Dataset *m_dataset;

            QVector<Shape::Shape*> m_shapes;
            int m_shapeId;
            quint16 m_shapeSize;

            QPainter m_painter;
            bool m_annotateWithMovement;
        };
    }
}
#endif // ANNOTATOR_H
