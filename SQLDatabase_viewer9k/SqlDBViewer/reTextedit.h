#ifndef RETEXTEDIT_H
#define RETEXTEDIT_H

#include <QObject>
#include <QtWidgets>

class RETextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit RETextEdit(QWidget *patent = nullptr);
    ~RETextEdit() override = default; //////????????????

    void keyReleaseEvent (QKeyEvent *event) override;

signals:
    void entered();
};

#endif // RETEXTEDIT_H
