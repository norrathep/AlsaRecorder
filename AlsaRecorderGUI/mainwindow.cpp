#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "preferences.h"
#include <QPushButton>
#include <QProcess>

#include <QDir> // For testing: Trying to find the current working directory

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Setting up MainWindow
    ui->setupUi(this);
    setWindowTitle("Alsa Recorder");

    // Setting Menu and Status Bar options
//    create_statusbar_menu();
    ui->statusBar->showMessage("Welcome to Alsa Recorder"); // Sets a "temporary" message. Use QLabel and add to make a "normal"/"permanent" message


    // Modify UI widgets
//    QIcon recordIcon("player_record.png");
//    QIcon stopIcon("stop-flat.png");
//    ui->recordButton->setIcon(recordIcon);
//    ui->stopButton->setIcon(stopIcon);

    // Connect buttons to scripts
    connect(ui->recordButton, SIGNAL(clicked()), this, SLOT(startRecording()));
    connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(stopRecording()));
    connect(ui->playbackButton, SIGNAL(clicked()), this, SLOT(playback()));


    // Get the current working directory
    QDir currentDir(QDir::currentPath());
    pwd = currentDir.dirName();
    ui->pwd_label->setText(pwd);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//*************MainWindow Style-related functions***************

void MainWindow::create_statusbar_menu()
{
    /*Creates a QMenu object in the mainwindow;
     *Allows user to access the Preferences window from the GUI
     */

    //Note: QMenuBar object contains QMenu objects, which in turn contain QAction objects

    QMenu *menu;
    QAction *preferences;

    menu = menuBar()->addMenu("&File");
    preferences = menu->addAction("&Preferences");

    connect(preferences,SIGNAL(triggered()), this, SLOT(openPreferences()));
}

void MainWindow::openPreferences()
{
    //Creates a preferences window and then opens it.
    preferences *dialog = new preferences(this);
    dialog->exec();
}

// Prepares LD_PRELOAD and runs start.sh
void MainWindow::startRecording()
{
    // TODO: Prepare LD_PRELOAD

    // TODO: Write QProcess code here
    QProcess *startScript = new QProcess(this);
    QString program = "sh";
    QStringList args;
    args << "./start.sh"; // Assuming that this is being run from the preload directory
    startScript->start(program,args);

    ui->statusBar->showMessage("Recording...");
}


// Runs stop.sh
void MainWindow::stopRecording()
{
    // TODO: Write QProcess code here
    QProcess *stopScript = new QProcess(this);
    QString program = "sh";
    QStringList args;
    args << "./stop.sh"; // Assuming that this is being run from the preload directory
    stopScript->start(program,args);

    ui->statusBar->showMessage("Recording stopped.");
}

void MainWindow::playback()
{
    QProcess *playAudio = new QProcess(this);
    QString program = "aplay";
    QStringList args;
    args << "out.wav"; // Assuming that this is being run from the preload directory

    // TODO: Work on the timing to display the "playing","end of audio", and "error" messages
    playAudio->start(program,args);
    ui->statusBar->showMessage("Playing recorded audio...");
}
