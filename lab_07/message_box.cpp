#include "message_box.h"
#include "qmessagebox.h"


void message_box(QString message)
{
    QMessageBox msgBox;
    msgBox.setText("Ошибка!");
    // Тип иконки сообщения
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setInformativeText(message);
    // На какой кнопке фокусироваться по умолчанию
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}
