#ifndef ANNOTATIONWIDGET_H
#define ANNOTATIONWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPen>
namespace SegWiz {
    namespace Model {
        class DrawingBuffer;
        class Dataset;
    }

    namespace View {
        class AnnotationWidget : public QWidget
        {
            Q_OBJECT
            Q_ENUMS(ViewingMode)

        public:
            enum ViewingMode { All, OverlayOnly, ImageOnly };

            explicit AnnotationWidget(Model::Dataset *dataset, QWidget *parent = nullptr);

            ViewingMode mode() const;
            void setMode(ViewingMode mode);
            void reset();
            Model::DrawingBuffer* annotation();

        protected:
            void paintEvent(QPaintEvent *event) override;
            void wheelEvent(QWheelEvent *event) override;
            void mouseMoveEvent(QMouseEvent *event) override;
            void mousePressEvent(QMouseEvent *event) override;

        private:
            Q_DISABLE_COPY(AnnotationWidget)

            void changeImage(const QImage& image);

            QPoint m_overlayPos;
            Model::DrawingBuffer *m_buffer;
            const Model::Dataset *m_data;
            QImage m_currentImage;
            ViewingMode m_mode;
            qreal m_opacity;
            QPen m_overlayPen;
        };
    }
}

#endif // ANNOTATIONWIDGET_H
