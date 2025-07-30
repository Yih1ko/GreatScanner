#ifndef ROTATEIMGCOMMAND_H
#define ROTATEIMGCOMMAND_H

#include <QUndoStack>       // 撤销栈
#include <QUndoCommand>     // 撤销命令基类
#include <QSlider>

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
