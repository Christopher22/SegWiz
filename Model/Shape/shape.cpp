#include "shape.h"

namespace SegWiz {
    namespace Model {
        namespace Shape {
            Shape::Shape(const DrawingBuffer *annotator): m_annotator(annotator)
            {
                Q_ASSERT(annotator);
            }
        }
    }
}
