#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTableWidget>
#include <QDebug>
#include <QFile>
#include <QTextStream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"File name"<<"Source");
    ms=0;
    s=0;
    m=0;
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()), this, SLOT(TimerSlot()));
    timer->start(1);
}

void MainWindow::TimerSlot(){
    ms++;
    if(ms>=1000){
        ms=0;
        s++;
    }
    if(s >= 60){
        s=0;
        m++;
    }
    ui->label_2->setText(QString::number(m) + " : " + QString::number(s) + " : " + QString::number(ms));
    if (m == 5){
        close();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
QVector <QString> names,snames,cont = {};
QString output="";
int c=0;

void MainWindow::UpdateTable(){
    ui->tableWidget->setRowCount(c);

    for(int i=0; i<ui->tableWidget->rowCount(); i++){
    QTableWidgetItem * itm = new QTableWidgetItem(snames[i]);
    ui->tableWidget->setItem(i,0,itm);
    }

    for(int i=0; i<ui->tableWidget->rowCount(); i++){
    QTableWidgetItem * itm = new QTableWidgetItem(names[i]);
    ui->tableWidget->setItem(i,1,itm);
    }
}

void MainWindow::on_pushButton_clicked() //Entering Files!
{   ::c++;
    QString source = QFileDialog::getOpenFileName(0, "Открыть", "", "*.txt ;; *.bin ;; *.cpp ;; testFile.bin");
    //QString str = QFileDialog::getExistingDirectory(0, "Выбор папки", "");

        //if (source != ""){
            int lastSlash = source.lastIndexOf('/');
            QString filename = source.mid(lastSlash + 1);
            ::names.push_back(source);
            ::snames.push_back(filename);
            UpdateTable();
        //}
}


void MainWindow::on_pushButton_2_clicked() // Modification
{
    QString mod = ui->lineEdit->text(); // модификатор
//

    //
    int n = names.size();
    for (int i=0; i<n; i++){
        QString ex = output; //папка выхода файлов
        QString str= names[i];
        QFile file(str);
        QString content;
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream stream(&file);
                content = stream.readAll();
                file.close();
         } else {
         //ош
         }
        QString result="";
        int length = qMin(content.length(), mod.length());
        for (int i = 0; i < length; ++i) {
            if (content.at(i) == mod.at(i)) {
                result.append('0');
            } else {
                result.append('1');
            }
        }
        if (content.length() > length) {
            result.append(content.right(content.length() - length));
        }

        if (mod.length() > length) {
            result.append(mod.right(mod.length() - length));
        }


        qDebug() << result;
        QString fileName = "test" + QString::number(i+1) + ".txt";
        ex += "/" + fileName;
        qDebug() << ex;
        QFile mFile(ex);
        if(!mFile.open(QFile::WriteOnly | QFile::Text)){
            //QMessageBox::information(this, "Error", " Path not correct");
            return;
        }
        QTextStream stream(&mFile);
        stream << result;
        mFile.close();

        if(ui->checkBox->isChecked()){
            //remove
             QFile file(names[i]);
             file.remove();
        }
    }
}


void MainWindow::on_pushButton_3_clicked() // Outputing Files!
{
    ::output = QFileDialog::getExistingDirectory(0, "Выбор папки", "");

}

