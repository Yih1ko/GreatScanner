#include "undo_stack_singleton.h"

Q_SINGLETON_CREATE_CPP(UndoStackSingleton)

UndoStackSingleton::UndoStackSingleton()
{
    m_undoStack = new QUndoStack();
}

UndoStackSingleton::~UndoStackSingleton() {
    delete m_undoStack;
}

QUndoStack* UndoStackSingleton::getUndoStack() {
    return m_undoStack;
}
