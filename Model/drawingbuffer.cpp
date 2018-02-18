#include "drawingbuffer.h"


#include "Shape/shape.h"
#include "Shape/circle.h"
#include "Shape/square.h"
#include "dataset.h"
#include "label.h"

#include <QWheelEvent>
#include <QMouseEvent>

namespace SegWiz {
    namespace Model {
        DrawingBuffer::DrawingBuffer(const Dataset *dataset, const QSize& size, QObject *parent) : QObject(parent), m_buffer(size), m_shapeSize(5), m_painter(&m_buffer), m_shapes(), m_shapeId(0), m_dataset(dataset)
        {
            Q_ASSERT(dataset && dataset->currentLabel());
            connect(dataset, &Dataset::labelChanged, this, &DrawingBuffer::setLabel);

            m_shapes.append(new Shape::Square(this));
            m_shapes.append(new Shape::Circle(this));

            m_painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
            this->setLabel(dataset->currentLabel());
        }

        DrawingBuffer::~DrawingBuffer()
        {
            for (Shape::Shape* shape : m_shapes) {
                delete shape;
            }
        }

        void DrawingBuffer::reset()
        {
            m_painter.eraseRect(0, 0, m_buffer.width(), m_buffer.height());
        }

        void DrawingBuffer::handleMouse(QMouseEvent *mouse)
        {
            if(mouse->buttons() & Qt::LeftButton) {
                m_shapes[m_shapeId]->draw(&m_painter, mouse->pos());
                emit painted();
            }
        }

        void DrawingBuffer::handleMouse(QWheelEvent *wheel)
        {
            if(wheel->angleDelta().y() > 0 && m_shapeSize < MAX_SHAPE_SIZE) {
                ++m_shapeSize;
            } else if(wheel->angleDelta().y() < 0 && m_shapeSize > MIN_SHAPE_SIZE) {
                --m_shapeSize;
            } else {
                return;
            }
        }

        const QPixmap &DrawingBuffer::image() const
        {
            return m_buffer;
        }

        quint16 DrawingBuffer::shapeSize() const
        {
            return m_shapeSize;
        }

        void DrawingBuffer::setShapeSize(quint16 shapeSize)
        {
            m_shapeSize = shapeSize;
        }

        quint16 DrawingBuffer::width() const
        {
            return m_buffer.width();
        }

        quint16 DrawingBuffer::height() const
        {
            return m_buffer.height();
        }

        const Shape::Shape* DrawingBuffer::shape() const
        {
            return m_shapes[m_shapeId];
        }

        bool DrawingBuffer::setShape(quint16 shapeId)
        {
            if(m_shapes.size() < shapeId) {
                m_shapeId = shapeId;
                return true;
            } else {
                return false;
            }
        }

        void DrawingBuffer::setLabel(const Label *label)
        {
            m_painter.setBrush(label->color());
            m_painter.setPen(QPen(label->color(), m_shapeSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        }
    }
}
