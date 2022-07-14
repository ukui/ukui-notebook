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

#include "widget.h"
#include "singleApplication.h"
#include "utils/xatom-helper.h"
#include "utils/utils.h"

#include <QApplication>
#include <X11/Xlib.h>
//#include <KWindowEffects>

#include "log.h"
#include "information_collector.h"

int getScreenWidth() {
    Display *disp = XOpenDisplay(NULL);
    Screen *scrn = DefaultScreenOfDisplay(disp);
    if (NULL == scrn) {
        return 0;
    }
    int width = scrn->width;

    if (NULL != disp) {
        XCloseDisplay(disp);
    }
    return width;
}

/*!
 * \brief main
 */
int main(int argc, char *argv[])
{
#ifndef TEST_DEBUG
    qInstallMessageHandler(logOutput);
#else
    qInstallMessageHandler(myMessageOutput);
#endif
    InformationCollector::getInstance().addMessage("便签应用启动!");
    if (getScreenWidth() > 2560) {
        #if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
                QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
                QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
        #endif
    }

    SingleApplication a(argc, argv);

    a.setWindowIcon(QIcon::fromTheme("kylin-notebook"));
    a.setApplicationVersion ("1.0.0");
    
    QCommandLineParser parser;
    Utils::setCLIName(parser);
    /* 处理命令行参数。
       除了解析选项（如parse（））外，此函数还处理内置选项并处理错误。
       如果调用了addVersionOption，则内置选项为--version，如果调用了addHelpOption，则为--help --help-all。
       当调用这些选项之一时，或者当发生错误（例如，传递了未知选项）时，当前进程将使用exit（）函数停止。
    */
    parser.process(a);
    Widget w;

    if(!a.isRunning()){
        a.w = &w;
        w.setProperty("useSystemStyleBlur", true);
        // 添加窗管协议
        MotifWmHints hints;
        hints.flags = MWM_HINTS_FUNCTIONS|MWM_HINTS_DECORATIONS;
        hints.functions = MWM_FUNC_ALL;
        hints.decorations = MWM_DECOR_BORDER;
        XAtomHelper::getInstance()->setWindowMotifHint(w.winId(), hints);
        //w.setAttribute(Qt::WA_TranslucentBackground);
        //KWindowEffects::enableBlurBehind(w.winId(),true);

        QObject::connect(&a, SIGNAL(messageReceived(/*const QString&*/)), &w, SLOT(sltMessageReceived(/*const QString&*/)));

        if (QApplication::arguments().length() > 1) {
            if (QApplication::arguments().at(1) == "--show") {
                QString arg = QApplication::arguments().at(2);
                qDebug() << "main" << arg.toInt();
                w.openMemoWithId(arg.toInt());
            }
        }

        return a.exec();
    } else {
        if (QApplication::arguments().length() > 1) {
            if (QApplication::arguments().at(1) == "--show") {
                QString arg = QApplication::arguments().at(2);
                qDebug() << "main" << arg.toInt();
                w.openMemoWithId(arg.toInt());
            }
        }
    }
    return 0;
}
