#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "LexicAnalizer.h"
#include "SyntacticAnalizer.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    LexicAnalizer parser;
    SyntacticAnalizer synAnalizer;

    ~MainWindow();

private slots:
    void on_actionCompile_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
