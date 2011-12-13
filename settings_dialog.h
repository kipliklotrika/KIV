#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include "settings.h"

#include <QDialog>

namespace Ui {
    class Settings_Dialog;
}

class Settings_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Settings_Dialog(QWidget *parent = 0);
    ~Settings_Dialog();

private slots:
    void on_buttonBox_accepted();
    void on_cbScrollChangesPage_clicked(bool checked);
    void on_ddWheel_currentIndexChanged(int index);

private:
    Ui::Settings_Dialog *ui;
};

#endif // SETTINGS_DIALOG_H
