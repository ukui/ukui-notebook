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

#include "setFontColorPage.h"
#include "ui_setFontColorPage.h"
#include "paletteWidget.h"

QString SetFontColor::KY_BLUE = "#1D52E1";
QString SetFontColor::KY_RED = "#EE0000";
QString SetFontColor::KY_GREEN = "#00D600";
QString SetFontColor::KY_ORANGE = "#E39900";
QString SetFontColor::KY_PURPLE = "#FF2F96";

SetFontColor::SetFontColor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetFontColor)
{
    ui->setupUi(this);

    initSetup();
}

SetFontColor::~SetFontColor()
{
    delete ui;
    //释放内存
    for (int i=0; i < 6;i++)
    {
        delete m_listAItem[i];
    }
}

void SetFontColor::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
//    系统默认 255 、 248  深色模式 30 34
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);

    p.setBrush(opt.palette.color(QPalette::Base));
    //qDebug() << "paintEvent" << p.brush().color().value();
    //p.setOpacity(0.3);
    p.setOpacity(0.9);
    p.setPen(Qt::NoPen);

    p.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    p.drawRoundedRect(opt.rect,6,6);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void SetFontColor::setColor()
{
    m_color[0] = QString("background:%1;").arg(SetFontColor::KY_BLUE);
    m_color[1] = QString("background:%1;").arg(SetFontColor::KY_RED);
    m_color[2] = QString("background:%1;").arg(SetFontColor::KY_GREEN);
    m_color[3] = QString("background:%1;").arg(SetFontColor::KY_ORANGE);
    m_color[4] = QString("background:%1;").arg(SetFontColor::KY_PURPLE);
    //监听主题改变
    const QByteArray id(THEME_QT_SCHEMA);
    if(QGSettings::isSchemaInstalled(id)){
        QGSettings *styleSettings = new QGSettings(id, QByteArray(), this);
        QString style = styleSettings->get(MODE_QT_KEY).toString();
        if(style == "ukui-default" || style == "ukui-white"
                || style == "ukui-light" || style == "ukui"){
            m_color[5]="background:rgba(0,0,0,1);";
        }else if(style == "ukui-dark" || style == "ukui-black"){
            m_color[5]="background:rgba(255,255,255,1);";
        }
        connect(styleSettings, &QGSettings::changed, this, [=](const QString &key){
            if (key == "styleName"){
                QString currentTheme = styleSettings->get(MODE_QT_KEY).toString();
                if(currentTheme == "ukui-default" || currentTheme == "ukui-white"
                        || currentTheme == "ukui-light" || currentTheme == "ukui"){
                    m_color[5]="background:rgba(0,0,0,1);";
                    m_listPage[5]->ui->label->setStyleSheet(m_color[5]+"border-radius:3px;");
                }else if(currentTheme == "ukui-dark" || currentTheme == "ukui-black"){
                    m_color[5]="background:rgba(255,255,255,1);";
                    m_listPage[5]->ui->label->setStyleSheet(m_color[5]+"border-radius:3px;");
                }
            }
        });
    }
}

void SetFontColor::initSetup()
{
    setColor();
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    setAttribute(Qt::WA_TranslucentBackground);

    ui->listWidget->clear();
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget->setSelectionMode(QAbstractItemView::NoSelection);

    for(int n=0; n<6; n++){
        m_listAItem[n] = new QListWidgetItem;
        m_listAItem[n]->setSizeHint(QSize(28,27));

        m_listPage[n] = new paletteButton(this);
        m_listPage[n]->resize(QSize(28,23));
        m_listPage[n]->ui->label->setStyleSheet(m_color[n]+"border-radius:3px;");

        ui->listWidget->addItem(m_listAItem[n]);
        ui->listWidget->setItemWidget(m_listAItem[n],m_listPage[n]);
    }
}
