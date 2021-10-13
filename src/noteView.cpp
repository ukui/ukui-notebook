/*
* Copyright (C) 2020 Tianjin KYLIN Information Technology Co., Ltd.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, see <http://www.gnu.org/licenses/&gt;.
*
*/

#include <QDebug>
#include <QPainter>
#include <QApplication>
#include <QAbstractItemView>
#include <QPaintEvent>
#include <QSortFilterProxyModel>
#include <QTimer>
#include <QScrollBar>

#include "noteView.h"
#include "listViewModeDelegate.h"

NoteMenu::NoteMenu(QWidget* parent):QMenu(parent)
{
    m_index = QModelIndex();
}

NoteView::NoteView(QWidget *parent)
    : QListView( parent )
    , m_isScrollBarHidden(true)
    , m_animationEnabled(true)
    , m_isMousePressed(false)
    , m_rowHeight(38)
{
    //不可编辑
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setWindowOpacity(0.7);
    setAttribute(Qt::WA_TranslucentBackground);//设置窗口透明显示(毛玻璃效果)
    viewport()->setAttribute(Qt::WA_TranslucentBackground);
    initPopMenu();
    connect(this,&QListView::customContextMenuRequested,this,&NoteView::onCustemMenuRequested);
    //一次性定时器,槽函数只处理一次
    QTimer::singleShot(0, this, SLOT(init()));
}

NoteView::~NoteView()
{
    qDeleteAll(m_menuActs.begin(),m_menuActs.end());
    m_menuActs.clear();
}

void NoteView::animateAddedRow(const QModelIndex& parent, int start, int end)
{
    Q_UNUSED(parent)
    Q_UNUSED(end)

    //model(): 返回此视图显示的模型。
    QModelIndex idx = model()->index(start,0);
    // Note: 这一行添加了flikering，当动画运行缓慢时可以看到
    // QItemSelectionModel::ClearAndSelect 将清除完整的选择 | 将选择所有指定的索引
    // selectionModel(): Returns the current selection model.
    // select(): 使用指定的命令选择模型项索引，并发出selectionChanged（）
    selectionModel()->select(idx, QItemSelectionModel::ClearAndSelect);

    listViewModeDelegate* delegate = static_cast<listViewModeDelegate*>(itemDelegate());
    if(delegate != Q_NULLPTR){
        delegate->setState(listViewModeDelegate::Insert, idx);

        // TODO find a way to finish this function till the animation stops
        while(delegate->animationState() == QTimeLine::Running){
            qApp->processEvents();
        }
    }
}

void NoteView::animateRemovedRow(const QModelIndex& parent, int start, int end)
{
    qDebug() << "当前文件 :" << __FILE__ << "当前函数 :" << __FUNCTION__ << "当前行号 :" << __LINE__;
    Q_UNUSED(parent)
    Q_UNUSED(end)

    //model(): 返回此视图显示的模型。
    QModelIndex idx = model()->index(start,0);
    // Note: 这一行添加了flikering，当动画运行缓慢时可以看到
    // QItemSelectionModel::ClearAndSelect 将清除完整的选择 | 将选择所有指定的索引
    // selectionModel(): Returns the current selection model.
    // select(): 使用指定的命令选择模型项索引，并发出selectionChanged（）
    selectionModel()->select(idx, QItemSelectionModel::ClearAndSelect);

    listViewModeDelegate* delegate = static_cast<listViewModeDelegate*>(itemDelegate());
    if(delegate != Q_NULLPTR){
        delegate->setCurrentSelectedIndex(QModelIndex());
        delegate->setState(listViewModeDelegate::Remove, idx);

        // TODO find a way to finish this function till the animation stops
        while(delegate->animationState() == QTimeLine::Running){
            qApp->processEvents();
        }
    }
}

void NoteView::paintEvent(QPaintEvent *e)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this->viewport());
    //p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(opt.palette.color(QPalette::Base));
    //p.setOpacity(0.7);
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(opt.rect,1,1);

    QListView::paintEvent(e);
}

void NoteView::rowsAboutToBeMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd,
                                  const QModelIndex &destinationParent, int destinationRow)
{
    Q_UNUSED(sourceParent)
    Q_UNUSED(sourceEnd)
    Q_UNUSED(destinationParent)
    Q_UNUSED(destinationRow)

    if(model() != Q_NULLPTR){
        QModelIndex idx = model()->index(sourceStart,0);
        listViewModeDelegate* delegate = static_cast<listViewModeDelegate*>(itemDelegate());
        if(delegate != Q_NULLPTR){

            if(m_animationEnabled){
                delegate->setState(listViewModeDelegate::MoveOut, idx);
            }else{
                delegate->setState(listViewModeDelegate::Normal, idx);
            }

            // TODO find a way to finish this function till the animation stops
            while(delegate->animationState() == QTimeLine::Running){
                qApp->processEvents();
            }
        }
    }
}

void NoteView::rowsMoved(const QModelIndex &parent, int start, int end,
                         const QModelIndex &destination, int row)
{
    Q_UNUSED(parent)
    Q_UNUSED(start)
    Q_UNUSED(end)
    Q_UNUSED(destination)

    QModelIndex idx = model()->index(row,0);
    setCurrentIndex(idx);

    listViewModeDelegate* delegate = static_cast<listViewModeDelegate*>(itemDelegate());
    if(delegate == Q_NULLPTR)
        return;

    if(m_animationEnabled){
        delegate->setState(listViewModeDelegate::MoveIn, idx );
    }else{
        delegate->setState(listViewModeDelegate::Normal, idx);
    }

    // TODO find a way to finish this function till the animation stops
    while(delegate->animationState() == QTimeLine::Running){
        qApp->processEvents();
    }
}

void NoteView::init()
{
    setMouseTracking(true);
    setUpdatesEnabled(true);
    //当鼠标进入或离开小部件时，强制Qt生成绘制事件
    viewport()->setAttribute(Qt::WA_Hover);
}

void NoteView::mouseMoveEvent(QMouseEvent* e)
{
    if(!m_isMousePressed){
        QListView::mouseMoveEvent(e);
    }
}

void NoteView::mousePressEvent(QMouseEvent* e)
{
    m_isMousePressed = true;
    QListView::mousePressEvent(e);
}

void NoteView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    emit viewportPressed();
    if (selected.indexes().isEmpty()){
        return;
    }
    QListView::selectionChanged(selected, deselected);
}

void NoteView::mouseReleaseEvent(QMouseEvent* e)
{
    m_isMousePressed = false;
    QListView::mouseReleaseEvent(e);
}

void NoteView::setCurrentRowActive(bool isActive)
{
    qDebug() << "当前文件 :" << __FILE__ << "当前函数 :" << __FUNCTION__ << "当前行号 :" << __LINE__;
    listViewModeDelegate* delegate = static_cast<listViewModeDelegate*>(itemDelegate());
    if(delegate == Q_NULLPTR)
        return;

    delegate->setActive(isActive);
    viewport()->update(visualRect(currentIndex()));
}

void NoteView::setAnimationEnabled(bool isEnabled)
{
    m_animationEnabled = isEnabled;
}

void NoteView::initPopMenu()
{
    m_popMenu = new NoteMenu(this);
    connect(m_popMenu,&QMenu::triggered,this,&NoteView::onMenuTriggered);
    QAction *act1 = new QAction(); // 置顶
    QAction *act2 = new QAction(); // 发送到邮箱
    QAction *act3 = new QAction(); // 删除当前文档
    QAction *act4 = new QAction(); // 打开当前便签
    QAction *act5 = new QAction(); // 清空列表
    QAction *act6 = new QAction(); // 新建便签
    act1->setText(tr("Topping this note"));
    act1->setObjectName("Topping this note");
    act2->setText(tr("Send a mail"));
    act2->setObjectName("Send a mail");
    act3->setText(tr("Delete this note"));
    act3->setObjectName("Delete this note");
    act4->setText(tr("Open this note"));
    act4->setObjectName("Open this note");
    act5->setText(tr("Clear all notes"));
    act5->setObjectName("Clear all notes");
    act6->setText(tr("Create a new note"));
    act6->setObjectName("Create a new note");
    m_menuActs.insert(act1->objectName(),act1);
    m_menuActs.insert(act2->objectName(),act2);
    m_menuActs.insert(act3->objectName(),act3);
    m_menuActs.insert(act4->objectName(),act4);
    m_menuActs.insert(act5->objectName(),act5);
    m_menuActs.insert(act6->objectName(),act6);
    //Connect to slots...
}
bool NoteView::addAct(QString key)
{
    if(m_menuActs.contains(key) == true){
        m_popMenu->addAction(m_menuActs.find(key).value());
        return true;
    }
    return false;
}
void NoteView::onCustemMenuRequested(QPoint pt)
{
    auto index = this->indexAt(pt);
    qDebug() << pt << index;
    if(index.isValid()) {
        m_popMenu->clear();
        m_popMenu->setIndex(index);
        addAct("Topping this note");
        addAct("Send a mail");
        m_popMenu->addSeparator();
        addAct("Open this note");
        addAct("Delete this note");
        addAct("Clear all notes");
        m_popMenu->exec(QCursor::pos());
    }else {
        m_popMenu->clear();
        m_popMenu->setIndex(QModelIndex());
        addAct("Clear all notes");
        addAct("Create a new note");
        m_popMenu->exec(QCursor::pos());
    }
}

void NoteView::onMenuTriggered(QAction *action)
{
    if(action == nullptr)
        return ;
    if(action->objectName().isNull() == true)
        return ;
    QModelIndex idx = m_popMenu->getIndex();
    qDebug() << idx;
    if(action->objectName().compare("Topping this note")==0){
        onTopping(idx);
    } else if(action->objectName().compare("Send a mail") == 0) {
        onSendMail(idx);
    } else if(action->objectName().compare("Open this note") == 0) {
        onOpenNote(idx);
    } else if(action->objectName().compare("Delete this note") == 0) {
        onDeleteSingle(idx);
    } else if(action->objectName().compare("Clear all notes") == 0) {
        onClearAll();
    } else if(action->objectName().compare("Create a new note") == 0) {
        onCreateNew();
    } else {
        qDebug () << "Wrong action "<< action->objectName();
    }

}

void NoteView::onClearAll()
{
    qDebug () << __FUNCTION__;
}

void NoteView::onCreateNew()
{
    qDebug () << __FUNCTION__;
}

void NoteView::onTopping(QModelIndex idx)
{
    qDebug () << __FUNCTION__ << " " << idx;
}

void NoteView::onDeleteSingle(QModelIndex idx)
{
    qDebug () << __FUNCTION__ << " " << idx;
}

void NoteView::onSendMail(QModelIndex idx)
{
    qDebug () << __FUNCTION__ << " " << idx;
}

void NoteView::onOpenNote(QModelIndex idx)
{
    qDebug () << __FUNCTION__ << " " << idx;
}
