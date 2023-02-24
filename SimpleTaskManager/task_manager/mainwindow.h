#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QTimer>
#include <QMainWindow>
#include "TASK_MANAGER.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    int sort_by;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTimer *timer;
    Task_Manager service;
    void operate();
    void stuff_table();
    void create_items_row(QString process_name,int pid ,double cpu_percent ,bool is_64bit);
public slots:
    void refresh();
    void get_sort(int what);
    void fix_process_status();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
