#include "reTextedit.h"

RETextEdit::RETextEdit(QWidget *parent) : QTextEdit (parent)
{

}

void RETextEdit::keyReleaseEvent(QKeyEvent *event)
{
    if ((event->key() == Qt::Key_Return) &&
            (event->modifiers() & Qt::ControlModifier))
    {
        emit entered();
        return;
    }
    QTextEdit::keyPressEvent(event);////what's this???
}
