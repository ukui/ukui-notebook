/*
 * Copyright (C) 2020, KylinSoft Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <kysdk/applications/kaboutdialog.h>
#include "about.h"

About::About(QWidget *parent) :
    kdk::KAboutDialog(parent)

{
    // ui->setupUi(this);

    setAppName(tr("Notes"));
    setAppIcon(QIcon::fromTheme("kylin-notebook"));
    setAppVersion("3.2.0");
    setBodyText(tr("Notes is a self-developed sidebar application plug-in, "
                   "which provides a rich interface, convenient operation and stable functions, "
                   "aiming at a friendly user experience."));
    setBodyTextVisiable(true);
}

About::~About()
{

}