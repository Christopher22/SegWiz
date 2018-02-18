#include "dataset.h"

#include "label.h"

namespace SegWiz {
    namespace Model {
        Dataset::Dataset(QObject *parent) : QObject(parent), m_currentLabel(0)
        {
            m_labels.append(new Label("Background", QColor(0, 0, 0)));
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
    }
}
