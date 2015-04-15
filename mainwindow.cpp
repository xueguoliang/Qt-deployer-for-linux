#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnProject_clicked()
{
    //mencari folder project
    projectFolder = QFileDialog::getExistingDirectory(this,"Open Folder Project");
    ui->lineProject->setText(projectFolder);

    if(ui->lineProject->text() != "")
    {
        ui->btnExe->setEnabled(true);
        ui->btnPlatform->setEnabled(true);
    }
}

void MainWindow::on_btnExe_clicked()
{
    int jumlah = 0;

    //mencari file executable
    QString filename = QFileDialog::getOpenFileName(this,"Open Executable File");
    ui->lineExe->setText(filename);


    //menjalankan perintah ldd
    filename.replace(" ","\\ ");
    QProcess *exe = new QProcess(this);
    exe->start("ldd "+filename);

    //membaca output ldd
    exe->waitForFinished();
    exe->waitForReadyRead();
    while(exe->canReadLine())
    {
        QString lib = exe->readLine();
        ui->txtExe->append(lib);

        QStringList libSplit = lib.split(QRegExp("\\s"));
        QString dirAsal = libSplit.at(3);
        QString dirTujuan = projectFolder + "/" + libSplit.at(1);

        QFile::copy(dirAsal,dirTujuan);

        //item found
        jumlah++;
        ui->lblFoundExe->setText("Dependencies Found : "+QString::number(jumlah));
    }

    //mengganti lokasi RPATH
    exe->start("chrpath -r . "+filename);
}

void MainWindow::on_btnPlatform_clicked()
{
    int jumlah = 0;

    //mencari file Plugin Platform
    QString filename = QFileDialog::getOpenFileName(this,"Open Plugin Platform File");
    ui->linePlatform->setText(filename);

    //menjalankan perintah ldd
    QProcess *platform = new QProcess(this);
    platform->start("ldd "+filename);

    //membaca output ldd
    platform->waitForFinished();
    while(platform->canReadLine())
    {
        QString lib = platform->readLine();
        ui->txtPlatform->append(lib);

        QStringList libSplit = lib.split(QRegExp("\\s"));
        QString dirAsal = libSplit.at(3);
        QString dirTujuan = projectFolder + "/" + libSplit.at(1);

        QFile::copy(dirAsal,dirTujuan);

        //set item found
        jumlah++;
        ui->lblFoundPlugin->setText("Dependencies Found : "+QString::number(jumlah));
    }

    //membuat folder platforms
    platform->start("mkdir " + projectFolder + "/platforms");
    platform->waitForFinished();

    //copy file Plugin Platform
    QFile::copy(filename, projectFolder + "/platforms/libqxcb.so");

    //mengganti lokasi RPATH
    platform->start("chrpath -r ../ " + projectFolder + "/platforms/libqxcb.so");
    platform->waitForFinished();
}
