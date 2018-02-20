#ifndef DATASET_H
#define DATASET_H

#include <QObject>
#include <QVector>
#include <QImage>
#include <QDir>
#include <QRandomGenerator>

namespace SegWiz {
    namespace Model {
        class Label;
        class Dataset : public QObject
        {
            Q_OBJECT
        public:
            explicit Dataset(QObject *parent = nullptr);
            virtual ~Dataset();

            bool addImages(const QDir &dir, const QStringList &include, const QStringList &exclude);
            void addLabel(const QString& name, const QColor& color);
            bool next();

            bool setCurrentLabel(quint16 label);
            const Label *currentLabel() const;

            void setSeed(quint32 seed);

            quint32 start() const;
            void setStart(quint32 start);

            quint32 end() const;
            void setEnd(quint32 end);

        signals:
            void labelChanged(const Label* label);
            void dataChanged(const QImage& image);

        private:
            class Location {
            public:
                Location(const QDir& dir, const QStringList& include, const QStringList& exclude);
                explicit operator bool() const;

                QDir nextImage(QRandomGenerator& random);
                QStringList include() const;
                QStringList exclude() const;
                QDir dir() const;
                quint32 files() const;
            private:
                const QDir m_dir;
                const QStringList m_include, m_exclude;
                QStringList m_files;
            };

            QRandomGenerator m_random;
            quint32 m_current, m_start, m_end;
            QVector<Label*> m_labels;
            QVector<Location*> m_locations;
            quint16 m_currentLabel;
            QDir m_currentFile;
        };
    }
}

#endif // DATASET_H
