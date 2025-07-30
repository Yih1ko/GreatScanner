#ifndef UNDO_STACK_SINGLETON_H
#define UNDO_STACK_SINGLETON_H

#include "singleton.h"
#include <QUndoStack>       // 撤销栈
#include <QUndoCommand>     // 撤销命令基类

class UndoStackSingleton{
    Q_SINGLETON_CREATE_H(UndoStackSingleton)
public:

    ~UndoStackSingleton();
    QUndoStack* getUndoStack();
private:
    QUndoStack* m_undoStack;
    UndoStackSingleton();
};

#endif // UNDO_STACK_SINGLETON_H
