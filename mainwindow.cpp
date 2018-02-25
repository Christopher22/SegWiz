#include "mainwindow.h"

#include "View/annotationwidget.h"

#include "Model/dataset.h"
#include "Model/drawingbuffer.h"
#include "Model/label.h"
#include "Model/Shape/shape.h"

#include <QMenuBar>
#include <QWidgetAction>
#include <QLabel>
#include <QInputDialog>
#include <QMessageBox>
#include <QApplication>
#include <QStatusBar>

namespace SegWiz {
    MainWindow::MainWindow(Model::Dataset *dataset, QWidget *parent)
        : QMainWindow(parent), m_data(dataset), m_annotation(new View::AnnotationWidget(m_data, this)), m_statusBarLabel(new QLabel("SegWiz", this))
    {
        Q_ASSERT(m_data);

        this->setCentralWidget(m_annotation);
        this->setFixedSize(m_annotation->size());

        connect(m_data, &Model::Dataset::dataChanged, [this](const QFile* file, const QImage& image) {
            this->setFixedSize(image.width(), image.height() + this->menuBar()->height() + this->statusBar()->height());

            QString end(QString::number(m_data->end(), 10));
            m_statusBarLabel->setText(
                        QString("Image %2/%3: \"%1\"")
                        .arg(file->fileName().rightRef(file->fileName().size() - file->fileName().lastIndexOf('/') - 1))
                        .arg(m_data->currentElement(), end.size(), 10, QChar('0'))
                        .arg(end));
        });

        // Add menu
        this->addFileMenu();
        this->addAnnotationMenu();
        this->addViewMenu();

        this->statusBar()->addPermanentWidget(m_statusBarLabel);
        this->statusBar()->setSizeGripEnabled(false);

        // Show first image
        m_data->next(false);
    }
    void MainWindow::addFileMenu()
    {
        QAction* saveAnnotation = new QAction(tr("&Save"), this);
        saveAnnotation->setShortcut(QKeySequence(Qt::Key_Space));
        saveAnnotation->setStatusTip(tr("Annotate next image"));
        connect(saveAnnotation, &QAction::triggered, [this] {
            this->nextImage(true);
        });

        QAction* skipAnnotation = new QAction(tr("&Skip"), this);
        skipAnnotation->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Space));
        skipAnnotation->setStatusTip(tr("Skip annotation"));
        connect(skipAnnotation, &QAction::triggered, [this] {
            this->nextImage(false);
        });

        QAction* closeSegWiz = new QAction(tr("&Close"), this);
        closeSegWiz->setShortcut(QKeySequence::Close);
        closeSegWiz->setStatusTip(tr("Closes SegWiz"));
        connect(closeSegWiz, &QAction::triggered, this, &QMainWindow::close);

        QMenu* fileMenu = this->menuBar()->addMenu(tr("&File"));
        fileMenu->addAction(saveAnnotation);
        fileMenu->addAction(skipAnnotation);
        fileMenu->addSeparator();
        fileMenu->addAction(closeSegWiz);
    }

    void MainWindow::addAnnotationMenu()
    {
        QAction* annotateWithMove = new QAction(tr("&Annotate with movement"), this);
        annotateWithMove->setCheckable(true);
        annotateWithMove->setChecked(true);
        annotateWithMove->setShortcut(QKeySequence(Qt::Key_M));
        annotateWithMove->setStatusTip(tr("Annotate while moving the mouse and hold left button down"));
        connect(annotateWithMove, &QAction::toggled, [this](bool checked) {
            m_annotation->annotation()->setAnnotateWithMovement(checked);
        });

        QActionGroup* labelGroup = new QActionGroup(this);
        for(quint32 i = 1; i < m_data->labels(); ++i) {
            QAction *labelAction = new QAction(m_data->label(i)->name(), labelGroup);

            QPixmap color(32, 32);
            color.fill(m_data->label(i)->color());
            labelAction->setIcon(QIcon(color));

            labelAction->setCheckable(true);
            labelAction->setShortcut(QKeySequence(QString::number(i)));
            connect(labelAction, &QAction::triggered, [this, i] {
                this->m_data->setCurrentLabel(i);
            });

            if(i == 1) {
                labelAction->setChecked(true);
            }
        }

        QActionGroup* shapeGroup = new QActionGroup(this);
        for(quint32 i = 0; i < m_annotation->annotation()->shapes(); ++i) {
            QAction *shapeAction = new QAction(this->m_annotation->annotation()->shape(i)->name(), shapeGroup);
            shapeAction->setCheckable(true);
            shapeAction->setShortcut(QKeySequence(QString("Ctrl+%1").arg(i + 1)));
            connect(shapeAction, &QAction::triggered, [this, i] {
                this->m_annotation->annotation()->setCurrentShape(i);
            });
            if(i == 0) {
                shapeAction->setChecked(true);
            }
        }

        QAction* resetAnnotation = new QAction(tr("&Reset"), this);
        resetAnnotation->setShortcut(QKeySequence::Refresh);
        resetAnnotation->setStatusTip(tr("Reset annotation"));
        connect(resetAnnotation, &QAction::triggered, [this] {
            m_annotation->reset();
        });

        QMenu* annotationMenu = this->menuBar()->addMenu(tr("&Annotation"));
        annotationMenu->addAction(annotateWithMove);
        annotationMenu->addSeparator()->setText(tr("Labels"));
        annotationMenu->addActions(labelGroup->actions());
        annotationMenu->addSeparator()->setText(tr("Shape"));
        annotationMenu->addActions(shapeGroup->actions());
        annotationMenu->addSeparator();
        annotationMenu->addAction(resetAnnotation);
    }

    void MainWindow::addViewMenu()
    {
        QAction* markerVisibility = new QAction(tr("&Set marker opacity"), this);
        markerVisibility->setShortcut(QKeySequence(Qt::Key_O));
        markerVisibility->setStatusTip(tr("Set the opacity of the marker"));
        connect(markerVisibility, &QAction::triggered, [this] {
            bool ok;
            double newOpacity = QInputDialog::getDouble(this,
                                                        tr("Set marker visibility"),
                                                        tr("Please enter marker opacity (0: Hidden, 1: Completely visible):"),
                                                        m_annotation->opacity(),
                                                        0.0,
                                                        1.0,
                                                        2,
                                                        &ok
                                                        );
            if (ok) {
                m_annotation->setOpacity(newOpacity);
                m_annotation->repaint();
            }
        });

        QAction* showCursor = new QAction(tr("&Show cursor"), this);
        showCursor->setCheckable(true);
        showCursor->setChecked(false);
        showCursor->setShortcut(QKeySequence(Qt::Key_C));
        showCursor->setStatusTip(tr("Show cursor"));
        connect(showCursor, &QAction::toggled, [this](bool checked) {
            m_annotation->showCursor(checked);
        });

        /*QAction* zoomIn = new QAction(tr("&Zoom in"), this);
        zoomIn->setShortcut(QKeySequence::ZoomIn);
        zoomIn->setStatusTip(tr("Zoom in"));

        QAction* zoomOut = new QAction(tr("&Zoom out"), this);
        zoomOut->setShortcut(QKeySequence::ZoomOut);
        zoomOut->setStatusTip(tr("Zoom out"));*/

        QActionGroup* viewMode = new QActionGroup(this);
        QAction* viewModeAll = new QAction(tr("&Both"), viewMode);
        viewModeAll->setShortcut(QKeySequence(Qt::Key_Q));
        viewModeAll->setStatusTip(tr("Show annotation and overlay"));
        viewModeAll->setCheckable(true);
        viewModeAll->setChecked(true);
        connect(viewModeAll, &QAction::triggered, [this] {
            m_annotation->setMode(View::AnnotationWidget::ViewingMode::All);
        });

        QAction* viewModeOverlay = new QAction(tr("&Annotation only"), viewMode);
        viewModeOverlay->setShortcut(QKeySequence(Qt::Key_W));
        viewModeOverlay->setStatusTip(tr("Show annotation only"));
        viewModeOverlay->setCheckable(true);
        connect(viewModeOverlay, &QAction::triggered, [this] {
            m_annotation->setMode(View::AnnotationWidget::ViewingMode::OverlayOnly);
        });

        QAction* viewModeImage = new QAction(tr("&Image only"), viewMode);
        viewModeImage->setShortcut(QKeySequence(Qt::Key_E));
        viewModeImage->setStatusTip(tr("Show image only"));
        viewModeImage->setCheckable(true);
        connect(viewModeImage, &QAction::triggered, [this] {
            m_annotation->setMode(View::AnnotationWidget::ViewingMode::ImageOnly);
        });

        QMenu* viewMenu = this->menuBar()->addMenu(tr("&View"));
        viewMenu->addAction(markerVisibility);
        viewMenu->addAction(showCursor);
        /*viewMenu->addSeparator();
        viewMenu->addAction(zoomIn);
        viewMenu->addAction(zoomOut);*/
        viewMenu->addSeparator()->setText(tr("Current view"));
        viewMenu->addAction(viewModeAll);
        viewMenu->addAction(viewModeOverlay);
        viewMenu->addAction(viewModeImage);
    }

    void MainWindow::nextImage(bool save)
    {
        if(!m_data->next(save)) {
            m_annotation->setVisible(false);
            QMessageBox::information(
                        this,
                        tr("Success"),
                        tr("Congratulation: All samples were annotated. This application will close now."),
                        QMessageBox::Ok
                        );
            QApplication::quit();
        }
    }
}
