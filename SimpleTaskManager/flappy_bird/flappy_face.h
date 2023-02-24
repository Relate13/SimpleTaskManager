#ifndef FLAPPY_FACE_H
#define FLAPPY_FACE_H

#include <QMainWindow>
#include <QTimer>
#include <QMouseEvent>
#include "Objects.h"
#include "cpu_monitor.h"
#include <vector>
#include <QTime>
#include <QSound>
#include <QMediaPlayer>
using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class Flappy_face; }
QT_END_NAMESPACE

class Flappy_face : public QMainWindow
{
    Q_OBJECT

public:
    Flappy_face(QWidget *parent = nullptr);
    ~Flappy_face();
    int cpu_percent;
    int score;
    int missle_magzine;
    void start();
    void stop();
    void restart();
    void update_score(int num);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent * event);
    bool status;
    bool with_gun;
    bool have_syringe;
    bool with_umbrella;
    cpu_monitor monitor;
    vector<Object*> blocks;
    vector<Object*> missles;
    Object* face;
    Object* background;
    Object* ground;
    QPixmap shop;
    QPixmap numbers;
    QTimer *timer;
    QTimer *block_creator_timer;
    QTimer *auto_shoot;
    QSound *player;
    void launch_missle();
    void release_umbrella();
    void launch_rocket();
public slots:
    void action();
    void new_block();
    void wait_1_sec();
    void syringe_starts();
    void syringe_ends();
    void med_starts();
    void med_ends();
    void gun_starts();
    void gun_ends();
    void gun_shoot();
private:
    Ui::Flappy_face *ui;
    void paintEvent(QPaintEvent * event);
};
#endif // FLAPPY_FACE_H
