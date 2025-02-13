#ifndef ROTATEIMGCOMMAND_H
#define ROTATEIMGCOMMAND_H

#include "global.h"

class RotateImgCommand : public QUndoCommand {
public:
    RotateImgCommand(QSlider *slider, const int &oldAngle, const int &newAngle, QUndoCommand *parent = nullptr)
        : QUndoCommand(parent), m_slider(slider), m_oldAngle(oldAngle), m_newAngle(newAngle) {}

    void undo() override;
    void redo() override;

private:
    QSlider *m_slider;
    int m_oldAngle;
    int m_newAngle;
};

#endif // ROTATEIMGCOMMAND_H
