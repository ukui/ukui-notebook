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

#ifndef EDIT_PAGE_H
#define EDIT_PAGE_H

#include <QWidget>
#include <QPainter>
#include <QMenu>
#include <QPainter>
#include <QColor>
#include <QModelIndex>
#include <QTextList>
#include <QPointer>
#include <QFocusEvent>

#include "noteHead.h"
#include "noteHeadMenu.h"
#include "ui_noteHeadMenu.h"
#include "selectColorPage.h"
#include "ui_selectColorPage.h"
#include "paletteWidget.h"
#include "ui_paletteWidget.h"
#include "setFontColorPage.h"
#include "ui_setFontColorPage.h"
#include "setFontSizePage.h"
#include "ui_setFontSizePage.h"

namespace Ui {
class Edit_page;
}
class Widget;

class EditPage : public QWidget
{
    Q_OBJECT

public:
    EditPage(Widget* p , int noteId, QWidget *parent = nullptr);
    ~EditPage();
    Ui::Edit_page *ui;
    Widget* m_notebook = nullptr;
    QColor m_editColor;
    noteHead *m_noteHead = nullptr;
    noteHeadMenu *m_noteHeadMenu = nullptr;
    QModelIndex m_index;
    SetFontSize *m_setSizePage = nullptr ;
    SetFontColor *m_setColorFontPage = nullptr ;
    QLabel *m_fontLabel = nullptr;
    QVBoxLayout *m_fontBtnLayout = nullptr;

    int m_id;
    int m_noteId;

    bool m_isFullscreen = false;
    //bool m_ignoreShowHideEvents;
    bool m_isInsImg = false;

    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *obj, QEvent *event);

    Qt::WindowFlags m_flags;
    void setStayOnTopSlot(bool b);
    void setHints();
    void setWindowKeepAbove();
    void setWindowStatusClear();

private:
    void initSetup();                                                // ????????????
    void slotsSetup();                                               // ???????????????
    void btnSetup();                                                 // ???????????????
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);// ????????????
    void fontChanged(const QFont &f);                                // ?????????????????????????????????
    void fontColorChanged(const QColor &c);                          // ?????????????????????????????????
    void initColor();
    void listenToGsettings();

private:
    SelectColor *m_colorPage = nullptr;
    QTimer *m_timer = nullptr;
//    QString color[6];
    QColor m_colorNum[6];

    QPointer<QTextList> m_lastBlockList;

    QPoint m_dragPosition;                                             // ????????????
    bool m_mousePressed = false;                                               // ??????????????????
    bool m_defaultFontColorChanged = false;                                    // ??????????????????

private slots:
    void cursorPositionChangedSlot();                                // ????????????????????????
    void currentCharFormatChangedSlot(const QTextCharFormat &format);// ????????????????????????
    void textChangedSlot();                                          // ?????????????????????id
    void setBoldSlot();                                              // ??????
    void setItalicSlot();                                            // ??????
    void setUnderlineSlot();                                         // ?????????
    void setStrikeOutSlot();                                         // ?????????
    void setUnorderedListSlot(bool checked);                         // ????????????
    void setOrderedListSlot(bool checked);                           // ????????????
    void list(bool checked, QTextListFormat::Style style);
    void setFontSizeSlot();                                          // ??????????????????
    void setFontColorSlot(QListWidgetItem *item);                                         // ??????????????????
    // ????????????
    void onFontColorClicked();
    void blueBtnSlot();
    void redBtnSlot();
    void darkGreenBtnSlot();
    void orangeBtnSlot();
    void purpleBtnSlot();
    void goldenBtnSlot();
    void greyBtnSlot();
    void lightGreenBtnSlot();
    void yellowBtnSlot();
    void defaultTextColorSlot();
    void defaultBtnSlot();
    void pinkBtnSlot();
    void showFullScreenSlot();
    void textRightMenu(QPoint);
    void textForNewEditpageSlot();
    void insertpicture();
    void dropImage(const QImage& image, const QString& format);

signals:
    void texthasChanged(int noteId, int id);
    void colorhasChanged(const QColor &color,int);
    void isEmptyNote(int noteId);
    void requestDel(int noteId);
    void textForNewEditpage();

};

#endif // EDIT_PAGE_H
