#include "rotateimgcommand.h"


void RotateImgCommand::undo()
{
    if (m_slider) {
        m_slider->setValue(m_oldAngle);
    }
}

void RotateImgCommand::redo()
{
    if (m_slider) {
        m_slider->setValue(m_newAngle);
    }
}
