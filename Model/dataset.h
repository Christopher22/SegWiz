#ifndef DATASET_H
#define DATASET_H

#include <QObject>
#include <QVector>

namespace SegWiz {
    namespace Model {
        class Label;
        class Dataset : public QObject
        {
            Q_OBJECT
        public:
            explicit Dataset(QObject *parent = nullptr);

            void addLabel(const QString& name, const QColor& color);
            bool setCurrentLabel(quint16 label);
            const Label *currentLabel() const;

        signals:
            void labelChanged(const Label* label);

        private:
            QVector<Label*> m_labels;
            quint16 m_currentLabel;
        };
    }
}

#endif // DATASET_H
