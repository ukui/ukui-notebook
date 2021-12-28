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

#include "paletteWidget.h"
#include "ui_paletteWidget.h"

#include <QDebug>

extern void qt_blurImage(QImage &blurImage, qreal radius, bool quality, int transposed);
QString PaletteWidget::BORDER_RADIUS_2PX =  "border-radius:2px";
QString PaletteWidget::KY_BLUE = "#3790FA";
QString PaletteWidget::KY_PURPLE = "#BA7BD8";
QString PaletteWidget::KY_PINK = "#F559AB";
QString PaletteWidget::KY_RED = "#FF4D4F";
QString PaletteWidget:: KY_ORANGE = "#F68C27";
QString PaletteWidget::KY_YELLOW = "#F9C53D";
QString PaletteWidget::KY_GREEN = "#52C429";
QString PaletteWidget::KY_GREY = "#8C8C8C";
PaletteWidget::PaletteWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaletteWidget)
{
    ui->setupUi(this);

    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.setRenderHint(QPainter::Antialiasing);
    p.drawRoundedRect(bmp.rect(),6,6);
    setMask(bmp);
    btnSetup();
}

PaletteWidget::~PaletteWidget()
{
    delete ui;
}



void PaletteWidget::btnSetup()
{
    QString styleSheet = QString("background: %1;%2;").arg(KY_BLUE).arg(BORDER_RADIUS_2PX);
    ui->blueBtn->setStyleSheet(styleSheet);
    styleSheet = QString("background:%1;%2;").arg(KY_RED).arg(BORDER_RADIUS_2PX);
    ui->redBtn->setStyleSheet(styleSheet);
    styleSheet = QString("background:%1;%2;").arg(KY_GREEN).arg(BORDER_RADIUS_2PX);
    ui->darkGreenBtn->setStyleSheet(styleSheet);
    styleSheet = QString("background:%1;%2;").arg(KY_ORANGE).arg(BORDER_RADIUS_2PX);
    ui->orangeBtn->setStyleSheet(styleSheet);
    styleSheet = QString("background:%1;%2;").arg(KY_PURPLE).arg(BORDER_RADIUS_2PX);
    ui->purpleBtn->setStyleSheet(styleSheet);
    styleSheet = QString("background:%1;%2;").arg(KY_YELLOW).arg(BORDER_RADIUS_2PX);
    ui->goldenBtn->setStyleSheet(styleSheet);
    styleSheet = QString("background:%1;%2;").arg(KY_GREY).arg(BORDER_RADIUS_2PX);
    ui->greyBtn->setStyleSheet(styleSheet);
    styleSheet = QString("background:%1;%2;").arg(KY_PINK).arg(BORDER_RADIUS_2PX);
    ui->pinkBtn->setStyleSheet(styleSheet);
}

void PaletteWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);  // 反锯齿;

    QPainterPath rectPath;
    rectPath.addRoundedRect(this->rect().adjusted(1, 1, -1, -1), 6, 6);

    // 画一个黑底
    QPixmap pixmap(this->rect().size());
    pixmap.fill(Qt::transparent);
    QPainter pixmapPainter(&pixmap);
    pixmapPainter.setRenderHint(QPainter::Antialiasing);
    pixmapPainter.setPen(Qt::transparent);
    pixmapPainter.setBrush(Qt::black);
    pixmapPainter.drawPath(rectPath);
    pixmapPainter.end();

    // 模糊这个黑底
    QImage img = pixmap.toImage();
    qt_blurImage(img, 10, false, false);

    // 挖掉中心
    pixmap = QPixmap::fromImage(img);
    QPainter pixmapPainter2(&pixmap);
    pixmapPainter2.setRenderHint(QPainter::Antialiasing);
    pixmapPainter2.setCompositionMode(QPainter::CompositionMode_Clear);
    pixmapPainter2.setPen(Qt::transparent);
    pixmapPainter2.setBrush(Qt::transparent);
    pixmapPainter2.drawPath(rectPath);

    // 绘制阴影
    p.drawPixmap(this->rect(), pixmap, pixmap.rect());

    // 绘制一个背景
    p.save();
    p.fillPath(rectPath,palette().color(QPalette::Base));
    p.restore();
}
