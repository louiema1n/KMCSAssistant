#include "pop.h"
#include "ui_pop.h"
#include "QMouseEvent"
#include "QPixmap"
#include <QAction>
#include <QMenu>
#include <mainwindow.h>
#include <QDesktopWidget>
#include <QRect>
#include <QDebug>

MainWindow *mainwindow;
Pop::Pop(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Pop)
{
    // 获取屏幕分辨率
    QDesktopWidget *desk = QApplication::desktop();
    QRect screen = desk->screenGeometry();
    setGeometry(screen.width() * 0.9, screen.height() * 0.1, 96, 96);
    ui->setupUi(this);
    // 取消窗体边框
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::WindowMinimizeButtonHint|Qt::Tool);
    // 设置窗体透明
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    // 设置鼠标样式为手型
    this->setCursor(QCursor(Qt::PointingHandCursor));

    // 显示为check图标
    pix = new QPixmap(ui->popLabel->size());
    pix->load(":/icon/check");
    ui->popLabel->setPixmap(*pix);

    // 创建一个菜单
    popMenu = new QMenu;
    popValid = new QAction(QObject::tr("Valid"), this);
    connect(popValid, SIGNAL(triggered()), this, SLOT(on_popValidAction()));
    popShowMainWindow = new QAction(QObject::tr("ShowMainWindow"), this);
    QObject::connect(popShowMainWindow, SIGNAL(triggered()), this, SLOT(on_popShowMainWindowAction()));
    popHidden = new QAction(QObject::tr("Hidden"), this);
    connect(popHidden, SIGNAL(triggered()), this, SLOT(on_popHiddenAction()));
    popExit = new QAction(QObject::tr("Exit"), this);
    QObject::connect(popExit, SIGNAL(triggered()), this, SLOT(on_popExitAction()));

    popMenu->addAction(popValid);
    popMenu->addAction(popShowMainWindow);
    popMenu->addAction(popHidden);
    popMenu->addAction(popExit);
}

Pop::~Pop()
{
    delete ui;
}

/** 重写鼠标按下事件
 * @brief Pop::mousePressEvent
 * @param event
 */
void Pop::mousePressEvent(QMouseEvent *event) {
   if(event->button() == Qt::LeftButton) {
       // 左键
       relativePos = event->globalPos() - this->pos();
   }
   if(event->button() == Qt::RightButton) {
       // 右键
       // 显示右键菜单
       popMenu->exec(event->globalPos());
   }
}

/** 重写鼠标移动事件
 * @brief Pop::mouseMoveEvent
 * @param event
 */
void Pop::mouseMoveEvent(QMouseEvent *event) {
    this->move(event->globalPos() - relativePos);
}

/** 隐藏Pop
 * @brief Pop::on_popHiddenAction
 */
void Pop::on_popHiddenAction() {
    this->hide();
}

/** 显示主界面
 * @brief Pop::on_popShowMainWindowAction
 */
void Pop::on_popShowMainWindowAction() {
    mainwindow->on_showMainAction();
}

/** 退出
 * @brief Pop::on_popExitAction
 */
void Pop::on_popExitAction() {
    mainwindow->on_exitAppAction();
}

/** 规则校验
 * @brief Pop::on_popValidAction
 */
void Pop::on_popValidAction() {
    mainwindow->on_actionValidRole_triggered();
}