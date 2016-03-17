/*
 * The preferences window should just be a basic Settings window. The first thing I wanted
 * it to do was be able to change the directory where the log files are located. I put this
 * part of the GUI on hold, but truthfully it can probably do without a Settings window at this
 * stage of the project.
 */

#include "preferences.h"
#include "ui_preferences.h"

preferences::preferences(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::preferences)
{
    ui->setupUi(this);
    this->setWindowTitle("Preferences");

    connect(ui->ok_button,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->cancel_button,SIGNAL(clicked()),this,SLOT(close()));

}

preferences::~preferences()
{
    delete ui;
}
