#include "dataset.h"

#include "label.h"

#include <QDebug>

namespace SegWiz {
    namespace Model {
        Dataset::Dataset(QObject *parent) : QObject(parent), m_currentLabel(0)
        {
            m_labels.append(new Label("Background", QColor(0, 0, 0)));
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
            if(*location) {
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

        void Dataset::setSeed(quint32 seed)
        {
            m_random.seed(seed);
        }

        bool Dataset::next()
        {
            QImage img;
            do {
                do {
                    if(m_current >= m_end || m_locations.size() == 0) {
                        return false;
                    }
                    quint32 index = m_random.bounded(m_locations.size());
                    m_currentFile = m_locations[index]->nextImage(m_random);
                    if(!(*m_locations[index])) {
                        delete m_locations[index];
                        m_locations.removeAt(index);
                    }
                    ++m_current;
                } while(m_current < m_start);

                img = QImage(m_currentFile.path());
                if(img.isNull()) {
                    qWarning() << "Image file " << m_currentFile << "invalid. Loading next...";
                }
            } while(img.isNull());

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

        QDir Dataset::Location::nextImage(QRandomGenerator &random)
        {
            QDir result;
            if(m_files.size() > 0) {
                do {
                    quint32 index = random.bounded(m_files.size());
                    result = m_dir.filePath(m_files.at(index));
                    m_files.removeAt(index);
                } while(!result.exists());
            }
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
