#ifndef ANNOTATIONWIDGET_H
#define ANNOTATIONWIDGET_H

#include <QWidget>
#include <QImage>

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

            explicit AnnotationWidget(const Model::Dataset* dataset, QWidget *parent = nullptr);

            ViewingMode mode() const;
            void setMode(ViewingMode mode);
            void reset();
            Model::DrawingBuffer* annotation();

        protected:
            void paintEvent(QPaintEvent *event) override;
            void wheelEvent(QWheelEvent *event) override;
            void mouseMoveEvent(QMouseEvent *event) override;

        private:
            void changeImage(const QImage& image);

            Model::DrawingBuffer *m_buffer;
            const Model::Dataset *m_data;
            QImage m_currentImage;
            ViewingMode m_mode;
            qreal m_opacity;
        };
    }
}

#endif // ANNOTATIONWIDGET_H
