#include "Model/dataset.h"
#include "mainwindow.h"

#include <QApplication>
#include <QCommandLineParser>

#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("Christopher Gundler");
    app.setOrganizationDomain("github.com/Christopher22");
    app.setApplicationName("SegWiz");
    app.setApplicationVersion("1.0.0.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Easy annotations for image segmentation tasks on complex datasets.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "An file in JSON format describing the targets of the annotation.");
    parser.process(app);

    SegWiz::Model::Dataset *data = nullptr;
    QFile* file = nullptr;
    if(parser.positionalArguments().length() > 0 && (file = new QFile(parser.positionalArguments().at(0)))->exists() && (data = SegWiz::Model::Dataset::load(file))) {
        SegWiz::MainWindow w(data);
        w.show();
        return app.exec();
    } else {
        std::cerr << "The description file does not exist or is corruped!";
        return -1;
    }
}
