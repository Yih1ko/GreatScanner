#ifndef MOVEPOINTCOMMAND_H
#define MOVEPOINTCOMMAND_H

#include "pointitem.h"
#include <QUndoCommand>

class MovePointCommand : public QUndoCommand {
public:
    MovePointCommand(PointItem *point, const QPointF &oldPos, const QPointF &newPos, QUndoCommand *parent = nullptr)
        : QUndoCommand(parent), m_point(point), m_oldPos(oldPos), m_newPos(newPos) {}

    void undo() override;

    void redo() override;

private:
    PointItem *m_point;
    QPointF m_oldPos;
    QPointF m_newPos;
};

#endif // MOVEPOINTCOMMAND_H
