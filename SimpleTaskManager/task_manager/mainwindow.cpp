#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    sort_by=1;
    ui->setupUi(this);
    setFixedSize(902,600);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));
    QHeaderView *headerGoods = ui->tableWidget->horizontalHeader();
    headerGoods->setSortIndicator(0, Qt::AscendingOrder);
    headerGoods->setSortIndicatorShown(true);
    connect(headerGoods, SIGNAL(sectionClicked(int)), ui->tableWidget, SLOT (sortByColumn(int)));
    connect(headerGoods, SIGNAL(sectionClicked(int)), this, SLOT (get_sort(int)));
    ui->tableWidget->setSelectionBehavior ( QAbstractItemView::SelectRows);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    ui->progressBar->setOrientation(Qt::Vertical);
    ui->progressBar->setMinimum(0);  // 最小值
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);
    //setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::get_sort(int what)
{
    sort_by=what;
}
void MainWindow::fix_process_status()
{
    service.lib.Read_System_Process2();
    stuff_table();
    ui->tableWidget->sortByColumn(sort_by);
}
void MainWindow::refresh()
{
    service.lib.Read_System_Process();
    QTimer::singleShot(500,this,SLOT(fix_process_status()));
}
void MainWindow::operate()
{
    timer->start(1000);
}
void MainWindow::stuff_table()
{
    int rowNum =  ui->tableWidget->rowCount();
    for(int i = 0;i < rowNum;i++)
    {
        ui->tableWidget->removeRow(0);
    }
    double total_percentage=0;
    for(int i=0;i<service.lib.PROCESS_COUNT;++i)
    {
        QString name= QString::fromWCharArray(service.lib.LIB[i]->NAME);
        int pid=service.lib.LIB[i]->PID;
        double percent=service.lib.LIB[i]->CPU_PERCENT;
        if(percent>0)
            total_percentage+=percent;
        bool is_64bit=service.lib.LIB[i]->is_x64;
        create_items_row(name,pid,percent,is_64bit);
    }
    ui->progressBar->setValue(total_percentage);
    /*ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->resizeColumnsToContents();*/
}
void MainWindow::create_items_row(QString process_name,int pid ,double cpu_percent ,bool is_64bit)
{
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    int rowIdx = ui->tableWidget->rowCount()-1;
    QString pidstr = QString("%1").arg(pid,4);
    QString percent;
    if(cpu_percent<0)
        percent="0";
    else
        percent=QString("%1").arg(cpu_percent/100,0,'g',3);
    QString bit;
    if(is_64bit)
        bit="64";
    else
        bit="32";
    QTableWidgetItem *item_name = new QTableWidgetItem(process_name);
    item_name->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QTableWidgetItem *item_pid = new QTableWidgetItem(QString(pidstr));
    item_pid->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QTableWidgetItem *item_cpu = new QTableWidgetItem(QString(percent));
    item_cpu->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QTableWidgetItem *item_64bit=new QTableWidgetItem(bit);
    item_64bit->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QFont font;
    font.setBold(1);
    QColor color;
    if(cpu_percent<0)
        color.setHsl(0,0,255);
    else if(cpu_percent>100)
        color.setHsl(0,255,255);
    else
        color.setHsv(0,255*cpu_percent/100,255);
    item_name->setBackground(color);
    item_name->setFont(font);
    item_pid->setBackground(color);
    item_cpu->setBackground(color);
    item_64bit->setBackground(color);
    item_name->setFlags(item_name->flags() & ~Qt::ItemIsEditable);
    item_pid->setFlags(item_pid->flags() & ~Qt::ItemIsEditable);
    item_cpu->setFlags(item_cpu->flags() & ~Qt::ItemIsEditable);
    item_64bit->setFlags(item_64bit->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->setItem(rowIdx,0,item_name);
    ui->tableWidget->setItem(rowIdx,1,item_pid);
    ui->tableWidget->setItem(rowIdx,2,item_cpu);
    ui->tableWidget->setItem(rowIdx,3,item_64bit);
}

BOOL TerminateProcessFromId(DWORD pid)
{
 HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE,pid);
 if (hProcess == NULL)
 {
 printf("openprocess failed...");
 return FALSE;
 }

 BOOL bRet = ::TerminateProcess(hProcess, 0 );
 if (bRet==0) //failed
 {
 DWORD dwErr = ::GetLastError();
 printf("ERROR:error no is :%d\n",dwErr);
 return FALSE;
 }
 return TRUE;
}
void MainWindow::on_pushButton_clicked()
{
    timer->stop();
}

void MainWindow::on_pushButton_2_clicked()
{
    timer->start(1000);
}

void MainWindow::on_pushButton_3_clicked()
{
    //cout<<"clicked"<<endl;
    QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();
    int count = items.count();
    if (count<4)
        return;
    string order("taskkill /F /PID ");
    order+=items.at(1)->text().toStdString();
    int pid=items.at(1)->text().toInt();
    //cout<<order<<endl;
    const char *order_c=order.c_str();
    system(order_c);
    //TerminateProcessFromId(pid);
}
