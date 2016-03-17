#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QString pwd;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openPreferences();
    void startRecording();
    void stopRecording();
    void playback();

private:
    Ui::MainWindow *ui;
    void create_statusbar_menu();

};

#endif // MAINWINDOW_H
