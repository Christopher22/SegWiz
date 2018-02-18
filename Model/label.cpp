#include "label.h"

namespace SegWiz {
    namespace Model {
        Label::Label(const QString& name, const QColor& color): m_name(name), m_color(color)
        {

        }

        QString Label::name() const
        {
            return m_name;
        }

        QColor Label::color() const
        {
            return m_color;
        }
    }
}
