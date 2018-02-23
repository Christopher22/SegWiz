#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace SegWiz {
    namespace View {
        class AnnotationWidget;
    }

    namespace Model {
        class Dataset;
    }

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        MainWindow(Model::Dataset* dataset  = nullptr, QWidget *parent = nullptr);

    private:
        void addFileMenu();
        void addAnnotationMenu();
        void addViewMenu();
        void nextImage(bool save);

        Model::Dataset *m_data;
        View::AnnotationWidget *m_annotation;
    };
}

#endif // MAINWINDOW_H
