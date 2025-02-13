#ifndef UNDO_STACK_SINGLETON_H
#define UNDO_STACK_SINGLETON_H

#include "global.h"

class UndoStackSingleton{
public:
    static UndoStackSingleton& GetInstance();
    ~UndoStackSingleton();

    QUndoStack* getUndoStack();

private:
    UndoStackSingleton(); // 私有构造函数
    UndoStackSingleton(const UndoStackSingleton&); // 禁止拷贝构造
    UndoStackSingleton& operator=(const UndoStackSingleton&); // 禁止赋值操作

    QUndoStack* m_undoStack;
};

#endif // UNDO_STACK_SINGLETON_H
