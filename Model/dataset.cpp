#include "dataset.h"

#include "label.h"

#include <QDebug>

namespace SegWiz {
    namespace Model {
        Dataset::Dataset(const QDir &output, const QString &outputFilename, QObject *parent) :
            QObject(parent),
            m_currentLabel(0),
            m_currentFile(nullptr),
            m_output(output),
            m_outputFileFormat(outputFilename)
        {
            Q_ASSERT(output.exists());
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

        void Dataset::setSeed(quint32 seed)
        {
            m_random.seed(seed);
        }

        bool Dataset::next(bool save)
        {
            if(save && m_currentFile) {
                QImage output;
                emit saveAnnotation(output);
                if(!output.isNull()) {
                    const QString currentFilename(m_currentFile->fileName());
                    const int lastPart = currentFilename.lastIndexOf('/') + 1;

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
                    quint32 index = m_random.bounded(m_locations.size());
                    m_currentFile = m_locations[index]->nextImage(m_random);
                    if(!m_currentFile) {
                        delete m_locations[index];
                        m_locations.removeAt(index);
                    }
                } while(!m_currentFile);
                img = QImage(m_currentFile->fileName());
            } while(img.isNull());
            ++m_current;

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
