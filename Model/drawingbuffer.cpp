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
        DrawingBuffer::DrawingBuffer(Dataset *dataset, const QSize& size, QObject *parent) : QObject(parent), m_buffer(new QPixmap(size)), m_shapeSize(5), m_painter(), m_shapes(), m_shapeId(0), m_dataset(dataset)
        {
            Q_ASSERT(dataset && dataset->currentLabel());

            connect(dataset, &Dataset::labelChanged, this, &DrawingBuffer::setLabel);
            connect(dataset, &Dataset::saveAnnotation, [this](QImage& img) {
                img = QImage(QSize(this->width(), this->height()), QImage::Format::Format_RGB32);
                img.fill(Qt::black);

                QPainter p(&img);
                //p.setBackground(QBrush(Qt::black));
                p.drawPixmap(QRect(0, 0, img.width(), img.height()), *m_buffer, QRect(0, 0, this->width(), this->height()));
            });

            m_shapes.append(new Shape::Square(this));
            m_shapes.append(new Shape::Circle(this));

            m_buffer->fill(Qt::transparent);
            m_painter.begin(m_buffer);

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
            m_painter.setCompositionMode (QPainter::CompositionMode_Source);
            m_painter.fillRect(QRect(0, 0, m_buffer->width(), m_buffer->height()), Qt::transparent);
            m_painter.setCompositionMode (QPainter::CompositionMode_SourceOver);
        }

        void DrawingBuffer::reset(const QSize &size)
        {
            m_painter.end();

            delete m_buffer;
            m_buffer = new QPixmap(size);
            m_buffer->fill(Qt::transparent);

            m_painter.begin(m_buffer);
            this->setLabel(m_dataset->currentLabel());
        }

        void DrawingBuffer::handleMouse(QMouseEvent *mouse)
        {
            if (mouse->buttons() & Qt::LeftButton) {
                m_shapes[m_shapeId]->draw(&m_painter, mouse->pos());
                emit painted();
            } else if(mouse->buttons() & Qt::RightButton) {
                this->setEraser();
                m_painter.setCompositionMode (QPainter::CompositionMode_Source);
                m_shapes[m_shapeId]->draw(&m_painter, mouse->pos());
                m_painter.setCompositionMode (QPainter::CompositionMode_SourceOver);
                this->setLabel(m_dataset->currentLabel());
                emit painted();
            } else if(mouse->buttons() & Qt::ForwardButton) {
                m_dataset->nextLabel();
            } else if(mouse->buttons() & Qt::BackButton) {
                m_dataset->previousLabel();
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
            return *m_buffer;
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
            return m_buffer->width();
        }

        quint16 DrawingBuffer::height() const
        {
            return m_buffer->height();
        }

        const Shape::Shape *DrawingBuffer::currentShape() const
        {
            return m_shapes[m_shapeId];
        }

        bool DrawingBuffer::setCurrentShape(quint16 shapeId)
        {
            if(shapeId < m_shapes.size()) {
                m_shapeId = shapeId;
                return true;
            } else {
                return false;
            }
        }

        const Shape::Shape *DrawingBuffer::shape(quint16 shapeId) const
        {
            return shapeId < m_shapes.size() ? m_shapes[shapeId] : nullptr;
        }

        quint32 DrawingBuffer::shapes() const
        {
            return m_shapes.size();
        }

        void DrawingBuffer::setLabel(const Label *label)
        {
            m_painter.setBrush(QBrush(label->color(), Qt::SolidPattern));
            m_painter.setPen(QPen(label->color(), m_shapeSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        }

        void DrawingBuffer::setEraser()
        {
            m_painter.setBrush(Qt::transparent);
            m_painter.setPen(QPen(Qt::transparent, m_shapeSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        }
    }
}
