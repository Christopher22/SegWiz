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

    if(parser.positionalArguments().length() < 1) {
        parser.showHelp(-1);
    }

    SegWiz::Model::Dataset *data = nullptr;
    QFile* file = new QFile(parser.positionalArguments().at(0));

    switch (SegWiz::Model::Dataset::load(file, &data)) {
    case SegWiz::Model::Dataset::LoadingStatus::Success: {
        SegWiz::MainWindow w(data);
        w.show();
        return app.exec();
    }
    case SegWiz::Model::Dataset::LoadingStatus::FileError: {
        qCritical("The description file does not exist or could not be opened!");
        return -1;
    }
    case SegWiz::Model::Dataset::LoadingStatus::OutputError: {
        qCritical("The output folder does not exist and could bot be created!");
        return -1;
    }
    case SegWiz::Model::Dataset::LoadingStatus::InputError: {
        qCritical("The input folder does not exist and could bot be created!");
        return -1;
    }
    case SegWiz::Model::Dataset::LoadingStatus::ParsingError: {
        qCritical("The content of the description file was invalid!");
        return -1;
    }
    case SegWiz::Model::Dataset::LoadingStatus::MissingInformation: {
        qCritical("The description file does not contain all required information!");
        return -1;
    }
    default:
        return -1;
    }   
}
