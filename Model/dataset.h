#ifndef DATASET_H
#define DATASET_H

#include <QObject>
#include <QVector>
#include <QImage>
#include <QDir>
#include <QRandomGenerator>

class QFile;

namespace SegWiz {
    namespace Model {
        class Label;
        class Dataset : public QObject
        {
            Q_OBJECT
        public:
            enum class LoadingStatus {
                Success,
                FileError,
                ParsingError,
                InputError,
                OutputError,
                MissingInformation
            };

            explicit Dataset(const QDir& output, const QString& outputFilename="%1.png", QObject *parent = nullptr);
            virtual ~Dataset();

            static LoadingStatus load(QFile* file, Dataset **result, QObject *parent = nullptr);
            bool save(QFile* file) const;

            bool addImages(const QDir &dir, const QStringList &include = {"*.bmp", "*.png", "*.jpg"}, const QStringList &exclude = QStringList());
            void addLabel(const QString& name, const QColor& color);

            bool next(bool save);

            bool setCurrentLabel(quint16 label);
            const Label *currentLabel() const;
            const Label *label(quint16 label) const;
            quint32 labels() const;
            void nextLabel();
            void previousLabel();

            void setSeed(quint32 seed);

            quint32 start() const;
            void setStart(quint32 start);

            quint32 end() const;
            void setEnd(quint32 end);

            qreal scalingFactor() const;
            bool setScalingFactor(qreal scalingFactor);

            quint32 currentElement() const;
        signals:
            void saveAnnotation(QImage &output);
            void labelChanged(const Label* label);
            void dataChanged(const QFile* file, const QImage& image);

        private:
            Q_DISABLE_COPY(Dataset)

            class Location {
            public:
                Location(const QDir& dir, const QStringList& include, const QStringList& exclude);
                explicit operator bool() const;

                QFile* nextImage(QRandomGenerator &random);
                QStringList include() const;
                QStringList exclude() const;
                QDir dir() const;
                quint32 files() const;
            private:
                const QDir m_dir;
                const QStringList m_include, m_exclude;
                QStringList m_files;
            };

            quint32 m_seed;
            QRandomGenerator m_random;
            quint32 m_current, m_start, m_end;
            QVector<Label*> m_labels;
            QVector<Location*> m_locations;
            quint16 m_currentLabel;
            QFile *m_currentFile;
            QDir m_output;
            QString m_outputFileFormat;
            qreal m_scalingFactor;
        };
    }
}

#endif // DATASET_H
