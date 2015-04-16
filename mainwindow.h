#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString projectFolder,
            fileExe,
            filePlatform;
    QProcess *exe = new QProcess(this);
    QProcess *platform = new QProcess(this);


private slots:
    void on_btnProject_clicked();

    void on_btnExe_clicked();

    void on_btnPlatform_clicked();

    void on_btnApplyExe_clicked();

    void on_btnApplyPlatform_clicked();

    void on_txtExe_textChanged();

    void on_txtPlatform_textChanged();

    void on_checkCopyPlatformFile_toggled(bool checked);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
