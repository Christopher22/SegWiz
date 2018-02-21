#include "annotationwidget.h"

#include "../Model/drawingbuffer.h"
#include "../Model/dataset.h"
#include "../Model/label.h"
#include "../Model/Shape/shape.h"

#include <QPaintEvent>

namespace SegWiz {
    namespace View {
        AnnotationWidget::AnnotationWidget(Model::Dataset *dataset, QWidget *parent) :
            QWidget(parent),
            m_buffer(new Model::DrawingBuffer(dataset, QSize(300, 300), parent)),
            m_data(dataset),
            m_mode(ViewingMode::All),
            m_opacity(0.2),
            m_overlayPen(QPen(QBrush(m_data->currentLabel()->color()), 5))
        {
            this->resize(300, 300);
            this->setMouseTracking(true);
            this->setAutoFillBackground(true);
            this->setCursor(Qt::CrossCursor);

            connect(m_data, &Model::Dataset::dataChanged, this, &AnnotationWidget::changeImage);
            connect(m_data, &Model::Dataset::labelChanged, this, [this](const Model::Label* label) {
                m_overlayPen.setColor(label->color());
                this->repaint();
            });

            connect(m_buffer, &Model::DrawingBuffer::shapeChanged, [this]() {
                this->repaint();
            });
        }

        void AnnotationWidget::paintEvent(QPaintEvent *paint)
        {
            if(!m_currentImage.isNull()) {
                QPainter painter(this);
                if(m_mode == ViewingMode::All) {
                    painter.drawImage(paint->rect(), m_currentImage, paint->rect());
                    painter.setOpacity(m_opacity);
                    painter.drawPixmap(paint->rect(), m_buffer->image(), paint->rect());

                    // Draw overlay
                    painter.setPen(m_overlayPen);
                    m_buffer->currentShape()->draw(&painter, m_overlayPos);
                } else if (m_mode == ViewingMode::OverlayOnly) {
                    painter.drawPixmap(paint->rect(), m_buffer->image(), paint->rect());
                } else {
                    painter.drawImage(paint->rect(), m_currentImage, paint->rect());
                }
            }
        }

        void AnnotationWidget::wheelEvent(QWheelEvent *event)
        {
            m_buffer->handleMouse(event);
            this->repaint();
        }

        void AnnotationWidget::mousePressEvent(QMouseEvent *event)
        {
            m_overlayPos = event->pos();
            if (m_mode != ViewingMode::ImageOnly) {
                m_buffer->handleMouse(event);
                this->repaint();
            }
        }

        void AnnotationWidget::mouseMoveEvent(QMouseEvent *event)
        {
            m_overlayPos = event->pos();
            if (m_mode != ViewingMode::ImageOnly) {
                m_buffer->handleMouse(event);
                this->repaint();
            }
        }

        void AnnotationWidget::changeImage(const QImage &image)
        {
            Q_ASSERT(!image.isNull());

            m_currentImage = image;
            this->resize(m_currentImage.width(), m_currentImage.height());
            m_buffer->reset(m_currentImage.size());

            this->repaint();
        }

        AnnotationWidget::ViewingMode AnnotationWidget::mode() const
        {
            return m_mode;
        }

        void AnnotationWidget::setMode(ViewingMode mode)
        {
            m_mode = mode;
            this->repaint();
        }

        void AnnotationWidget::reset()
        {
            m_buffer->reset();
            this->repaint();
        }

        Model::DrawingBuffer *AnnotationWidget::annotation()
        {
            return m_buffer;
        }
    }
}
