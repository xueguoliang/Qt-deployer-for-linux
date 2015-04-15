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
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString projectFolder;

private slots:
    void on_btnProject_clicked();

    void on_btnExe_clicked();

    void on_btnPlatform_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
