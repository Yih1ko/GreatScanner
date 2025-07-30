#include "movepointcommand.h"


void MovePointCommand::undo() {
    if (m_point) {
        m_point->setPoint(m_oldPos);
        emit m_point->positionChanged();
    }
}

void MovePointCommand::redo() {
    if (m_point) {
        m_point->setPoint(m_newPos);
        emit m_point->positionChanged();
    }
}
