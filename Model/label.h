#ifndef LABEL_H
#define LABEL_H

#include <QColor>
#include <QString>

namespace SegWiz {
    namespace Model {
        class Label {
        public:
            Label(const QString& name, const QColor& color);
            QString name() const;
            QColor color() const;
        private:
            const QString m_name;
            const QColor m_color;
        };
    }
}
#endif // LABEL_H
