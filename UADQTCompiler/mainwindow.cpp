#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ErrorModule.h"
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

void MainWindow::on_actionCompile_triggered()
{
    ui->listWidget->clear();
    ErrorModule::m_errors.clear();
    parser.Parse(const_cast<char*>(ui->plainTextEdit->toPlainText().toStdString().c_str()));
    synAnalizer.Analize(parser);
    ErrorModule::SaveFile();

    for (auto &err : ErrorModule::m_errors){
        std::string errorString;
        errorString = (err.m_phase + "\t" + err.m_description + "\t"+ std::to_string(err.m_lineNum) + "\t"+err.m_line + "\r");
        ui->listWidget->addItem(QString::fromStdString(errorString));
    }

}
