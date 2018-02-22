#include "dataset.h"

#include "label.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

namespace SegWiz {
    namespace Model {
        Dataset::Dataset(const QDir &output, const QString &outputFilename, QObject *parent) :
            QObject(parent),
            m_seed(QRandomGenerator::global()->generate()),
            m_random(QRandomGenerator(m_seed)),
            m_currentLabel(0),
            m_currentFile(nullptr),
            m_output(output),
            m_outputFileFormat(outputFilename)
        {
            Q_ASSERT(output.exists());

            if(!outputFilename.contains("%1")) {
                qWarning("Invalid pattern. Using default.");
                m_outputFileFormat = QString("%1.png");
            }

            m_labels.append(new Label("Background", QColor(0, 0, 0, 0)));
        }

        Dataset::~Dataset()
        {
            for(Label* label: m_labels) {
                delete label;
                label = nullptr;
            }

            for(Location* location: m_locations) {
                delete location;
                location = nullptr;
            }
        }

        bool Dataset::save(QFile* file) const
        {
            if(!file->open(QFile::WriteOnly)) {
                return false;
            }

            QJsonObject config;
            config.insert("start", QJsonValue(qint64(m_current)));
            config.insert("end", QJsonValue(qint64(m_end)));
            config.insert("seed", QJsonValue(qint64(m_seed)));
            config.insert("scaling", QJsonValue(m_scalingFactor));

            QJsonArray labels;
            for(int i = 1; i < m_labels.size(); i++) {
                QJsonObject label;
                label.insert("name", QJsonValue(m_labels[i]->name()));

                QJsonObject color;
                color.insert("r", QJsonValue(m_labels[i]->color().red()));
                color.insert("g", QJsonValue(m_labels[i]->color().green()));
                color.insert("b", QJsonValue(m_labels[i]->color().blue()));
                label.insert("color", QJsonValue(color));

                labels.append(label);
            }
            config.insert("labels", QJsonValue(labels));

            QJsonArray input;
            for(const Location* loc: m_locations) {
                QJsonObject location;
                location.insert("path", QJsonValue(loc->dir().path()));
                location.insert("include", QJsonValue(QJsonArray::fromStringList(loc->include())));
                location.insert("exclude", QJsonValue(QJsonArray::fromStringList(loc->exclude())));
                input.append(location);
            }
            config.insert("input", QJsonValue(input));

            QJsonObject output;
            output.insert("path", QJsonValue(m_output.path()));
            output.insert("pattern", QJsonValue(m_outputFileFormat));
            config.insert("output", QJsonValue(output));

            file->write(QJsonDocument(config).toJson(QJsonDocument::Indented));
            file->close();
            return true;
        }

        Dataset *Dataset::load(QFile* file, QObject *parent)
        {
            if(!file->open(QFile::ReadOnly)) {
                return nullptr;
            }

            QJsonDocument input(QJsonDocument::fromJson(file->readAll()));
            file->close();

            if(input.isNull()) {
                return nullptr;
            }

            QJsonObject config(input.object());
            if(!config["output"].isObject() || !config["input"].isArray()) {
                return nullptr;
            }

            QJsonObject outputObject(config["output"].toObject());
            QDir output(outputObject["path"].toString("/invalid/"));
            QString outputPattern(outputObject["pattern"].toString("%1.png"));
            if(!output.exists()) {
                qWarning("The output folder does not exists!");
                return nullptr;
            }

            Dataset *data = new Dataset(output, outputPattern, parent);
            for(const QJsonValue& inputElement: config["input"].toArray()) {
                QJsonObject inputObject(inputElement.toObject());
                if(!inputElement.isObject() || !inputObject["path"].isString() || !inputObject["include"].isArray() || !inputObject["exclude"].isArray()) {
                    delete data;
                    return nullptr;
                }

                QDir input(inputObject["path"].toString());
                if(!input.exists()) {
                    qWarning() << "The input folder " << input << "does not exists!";
                    return nullptr;
                }

                QStringList include;
                for(const QJsonValue& currentInclude: inputObject["include"].toArray()) {
                    include.append(currentInclude.toString());
                }

                QStringList exclude;
                for(const QJsonValue& currentExclude: inputObject["exclude"].toArray()) {
                    exclude.append(currentExclude.toString());
                }

                data->addImages(input, include, exclude);
            }

            for(const QJsonValue& labelElement: config["labels"].toArray()) {
                QJsonObject labelObject(labelElement.toObject());
                if(!labelElement.isObject() || !labelElement["name"].isString() || !labelElement["color"].isObject()) {
                    delete data;
                    return nullptr;
                }

                QJsonObject color(labelElement["color"].toObject());
                data->addLabel(labelElement["name"].toString(), QColor(quint32(color["r"].toDouble()), quint32(color["g"].toDouble()), quint32(color["b"].toDouble())));
            }

            if(config["start"].isDouble()) {
                data->setStart(config["start"].toDouble());
            }

            if(config["end"].isDouble()) {
                data->setEnd(config["end"].toDouble());
            }

            if(config["seed"].isDouble()) {
                data->setSeed(config["seed"].toDouble());
            }

            if(config["scaling"].isDouble()) {
                data->setScalingFactor(config["scaling"].toDouble());
            }

            return data;
        }

        bool Dataset::addImages(const QDir &dir, const QStringList &include, const QStringList &exclude)
        {
            Location* location = new Location(dir, include, exclude);
            if(location->files() > 0) {
                m_locations.append(location);
                m_end += location->files();
                return true;
            }  else {
                delete location;
                return false;
            }
        }

        void Dataset::addLabel(const QString &name, const QColor &color)
        {
            m_labels.append(new Label(name, color));

            // Ignore default label
            if(m_labels.size() == 2) {
                this->setCurrentLabel(1);
            }
        }

        bool Dataset::setCurrentLabel(quint16 label)
        {
            if(label < m_labels.size()) {
                m_currentLabel = label;
                emit labelChanged(m_labels[m_currentLabel]);
                return true;
            } else {
                return false;
            }
        }

        const Label *Dataset::currentLabel() const
        {
            return m_labels[m_currentLabel];
        }

        const Label *Dataset::label(quint16 label) const
        {
            return label < m_labels.size() ? m_labels[label] : nullptr;
        }

        quint32 Dataset::labels() const
        {
            return m_labels.size();
        }

        void Dataset::nextLabel()
        {
            if(m_labels.size() > 1) {
                m_currentLabel = (m_currentLabel == m_labels.size() - 1 ? 1 : m_currentLabel + 1);
                emit labelChanged(m_labels[m_currentLabel]);
            }
        }

        void Dataset::previousLabel()
        {
            if(m_labels.size() > 1) {
                m_currentLabel = (m_currentLabel == 1 ? m_labels.size() - 1 : m_currentLabel - 1);
                emit labelChanged(m_labels[m_currentLabel]);
            }
        }

        void Dataset::setSeed(quint32 seed)
        {
            m_seed = seed;
            m_random.seed(m_seed);
        }

        bool Dataset::next(bool save)
        {
            if(save && m_currentFile) {
                QImage output;
                emit saveAnnotation(output);
                if(!output.isNull()) {
                    const QString currentFilename(m_currentFile->fileName());
                    const int lastPart = currentFilename.lastIndexOf('/') + 1;

                    if(m_scalingFactor != 1.0) {
                        output = output.scaled(output.size() / m_scalingFactor, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    }

                    QString name(m_outputFileFormat.arg(currentFilename.mid(lastPart, currentFilename.lastIndexOf('.') - lastPart)));
                    output.save(m_output.filePath(name));
                }
            }

            QImage img;
            do {
                if(m_currentFile) {
                    delete m_currentFile;
                    m_currentFile = nullptr;
                }

                do {
                    if(m_current >= m_end || m_locations.size() == 0) {
                        return false;
                    }

                    do {
                        quint32 index = m_random.bounded(m_locations.size());
                        m_currentFile = m_locations[index]->nextImage(m_random);
                        if(!m_currentFile) {
                            delete m_locations[index];
                            m_locations.removeAt(index);
                        }
                        ++m_current;
                    } while(m_current < m_start);
                } while(!m_currentFile);
                img = QImage(m_currentFile->fileName());
            } while(img.isNull());

            if(m_scalingFactor != 1.0) {
                img = img.scaled(img.size() * m_scalingFactor, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            }

            emit dataChanged(img);
            return true;
        }

        quint32 Dataset::start() const
        {
            return m_start;
        }

        void Dataset::setStart(quint32 start)
        {
            m_start = start;
        }

        quint32 Dataset::end() const
        {
            return m_end;
        }

        void Dataset::setEnd(quint32 end)
        {
            m_end = end;
        }

        qreal Dataset::scalingFactor() const
        {
            return m_scalingFactor;
        }

        bool Dataset::setScalingFactor(qreal scalingFactor)
        {
            // Forbid resetting once loaded
            if(!m_currentFile) {
                m_scalingFactor = scalingFactor;
                return true;
            } else {
                return false;
            }
        }

        quint32 Dataset::currentElement() const
        {
            return m_current;
        }

        Dataset::Location::Location(const QDir &dir, const QStringList &include, const QStringList &exclude):
            m_dir(dir),
            m_include(include),
            m_exclude(exclude),
            m_files(m_dir.entryList(m_include, QDir::Files | QDir::Readable, QDir::Name))
        {
            for(const QString& exclude: m_exclude) {
                int index = 0;
                while((index = m_files.indexOf(exclude, index)) != -1) {
                    m_files.removeAt(index);
                }
            }
        }

        Dataset::Location::operator bool() const
        {
            return m_files.size() > 0;
        }

        QFile *Dataset::Location::nextImage(QRandomGenerator &random)
        {
            QFile *result = nullptr;
            do {
                if(m_files.size() == 0) {
                    return nullptr;
                } else if(result) {
                    delete result;
                }

                quint32 index = random.bounded(m_files.size());
                result = new QFile(m_dir.filePath(m_files.at(index)));
                m_files.removeAt(index);
            } while(!result->exists());
            return result;
        }

        quint32 Dataset::Location::files() const
        {
            return m_files.size();
        }

        QDir Dataset::Location::dir() const
        {
            return m_dir;
        }

        QStringList Dataset::Location::exclude() const
        {
            return m_exclude;
        }

        QStringList Dataset::Location::include() const
        {
            return m_include;
        }
    }
}
