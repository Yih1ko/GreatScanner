#include "undo_stack_singleton.h"

UndoStackSingleton& UndoStackSingleton::GetInstance() {
    static UndoStackSingleton instance;
    return instance;
}

UndoStackSingleton::UndoStackSingleton() {
    m_undoStack = new QUndoStack();
}

UndoStackSingleton::~UndoStackSingleton() {
    delete m_undoStack;
}

QUndoStack* UndoStackSingleton::getUndoStack() {
    return m_undoStack;
}
