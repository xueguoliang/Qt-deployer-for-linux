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
    fileExe = QFileDialog::getOpenFileName(this,"Open Executable File");
    ui->lineExe->setText(fileExe);

    //menjalankan perintah ldd
    exe->start("ldd "+ QString('"') + fileExe + QString('"'));

    //membaca output ldd
    exe->waitForFinished();
    while(exe->canReadLine())
    {
        QString lib = exe->readLine();
        ui->txtExe->append(lib);

        //item found
        jumlah++;
        ui->lblFoundExe->setText("Dependencies Found : "+QString::number(jumlah));
    }
}

void MainWindow::on_btnPlatform_clicked()
{
    int jumlah = 0;

    //mencari file Plugin Platform
    filePlatform = QFileDialog::getOpenFileName(this,"Open Plugin Platform File");
    ui->linePlatform->setText(filePlatform);

    //menjalankan perintah ldd
    platform->start("ldd "+ QString('"') + filePlatform + QString('"'));

    //membaca output ldd
    platform->waitForFinished();
    while(platform->canReadLine())
    {
        QString lib = platform->readLine();
        ui->txtPlatform->append(lib);

        //set item found
        jumlah++;
        ui->lblFoundPlugin->setText("Dependencies Found : "+QString::number(jumlah));
    }
}

void MainWindow::on_btnApplyExe_clicked()
{
    exe->start("ldd " + QString('"') + fileExe + QString('"'));
    exe->waitForFinished();

    //copy semua library
    if(ui->radioCopyExeAll->isChecked())
    {
        while(exe->canReadLine())
        {
            QString lib = exe->readLine();

            QStringList libSplit = lib.split(QRegExp("\\s"));
            QString dirAsal = libSplit.at(3);
            QString dirTujuan = projectFolder + "/" + libSplit.at(1);
            QFile::copy(dirAsal,dirTujuan);
        }
    }

    //hanya copy qt libraries
    if(ui->radioCopyExeQt->isChecked())
    {
        while(exe->canReadLine())
        {
            QString lib = exe->readLine();

            QStringList libSplit = lib.split(QRegExp("\\s"));
            QString dirAsal = libSplit.at(3);
            QString dirTujuan = projectFolder + "/" + libSplit.at(1);

            if(dirAsal.contains("Qt"))
                QFile::copy(dirAsal,dirTujuan);
        }
    }

    //jika chrpath dicawang
    if(ui->checkRpathExe->isChecked())
    {
        exe->start("chrpath -r . " + QString('"') + fileExe + QString('"'));
        exe->waitForFinished();
    }
}

void MainWindow::on_btnApplyPlatform_clicked()
{
    platform->start("ldd " + QString('"') + filePlatform + QString('"'));
    platform->waitForFinished();

    //copy semua libraries
    if(ui->radioCopyPlatformAll->isChecked())
    {
        while(platform->canReadLine())
        {
            QString lib = platform->readLine();

            QStringList libSplit = lib.split(QRegExp("\\s"));
            QString dirAsal = libSplit.at(3);
            QString dirTujuan = projectFolder + "/" + libSplit.at(1);

            QFile::copy(dirAsal,dirTujuan);
        }
    }

    //hanya copy Qt libraries
    if(ui->radioCopyPlatformQt->isChecked())
    {
        while(platform->canReadLine())
        {
            QString lib = platform->readLine();

            QStringList libSplit = lib.split(QRegExp("\\s"));
            QString dirAsal = libSplit.at(3);
            QString dirTujuan = projectFolder + "/" + libSplit.at(1);

            if(dirAsal.contains("Qt"))
                QFile::copy(dirAsal,dirTujuan);
        }
    }

    //jika copy platform file dicawang
    if(ui->checkCopyPlatformFile->isChecked())
    {
        //membuat folder platforms
        platform->start("mkdir " + QString('"') + projectFolder + "/platforms" + QString('"'));
        platform->waitForFinished();

        //copy plugin platforms
        QFile::copy(filePlatform, projectFolder + "/platforms/libqxcb.so");
    }

    //jika chrpath dicawang
    if(ui->checkRpathPlatform->isChecked())
    {
        platform->start("chrpath -r ../ " + QString('"') + projectFolder + "/platforms/libqxcb.so" + QString('"'));
        platform->waitForFinished();
    }
}

void MainWindow::on_txtExe_textChanged()
{
    ui->frameExe->setEnabled(true);
}

void MainWindow::on_txtPlatform_textChanged()
{
    ui->framePlatform->setEnabled(true);
}

void MainWindow::on_checkCopyPlatformFile_toggled(bool checked)
{
    ui->checkRpathPlatform->setEnabled(checked);

    if(checked == false)
        ui->checkRpathPlatform->setChecked(false);
}
