/*
 * Copyright (C) 2016 ~ 2017 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "suspenddialog.h"

#include <QApplication>
#include <QDesktopWidget>

#include <QDebug>

SuspendDialog::SuspendDialog(QRect screenGeometry)
    : DDialog(tr("External monitor detected, suspend?"), tr("%1s").arg(15)),
      m_screenGeometry(screenGeometry),
      m_timerTick(0)
{
    const auto ratio = devicePixelRatioF();
    QPixmap icon = QIcon::fromTheme("computer").pixmap(QSize(48, 48) * ratio);
    icon.setDevicePixelRatio(ratio);

    setIconPixmap(icon);

    QStringList buttons;
    buttons << tr("Cancel") << tr("Suspend");
    addButtons(buttons);

    m_timer.setInterval(1000);
    m_timer.setSingleShot(false);
    connect(&m_timer, &QTimer::timeout, this, [this] {
        m_timerTick++;
        if (m_timerTick >= 15) {
            done(1);
        } else {
            setMessage(tr("%1s").arg(15 - m_timerTick));
        }
    });

    m_timer.start();
}

void SuspendDialog::showEvent(QShowEvent *event)
{
    DDialog::showEvent(event);

    move(m_screenGeometry.center() - rect().center());
}

Manager::Manager()
{
    setupDialogs();
}

void Manager::setupDialogs()
{
    const QDesktopWidget *desktop = QApplication::desktop();
    for (int i = 0; i < desktop->screenCount(); i++) {
        const QRect geom = desktop->screenGeometry(i);

        SuspendDialog *dialog = new SuspendDialog(geom);
        dialog->show();

        connect(dialog, &SuspendDialog::finished, this, &Manager::finished);
    }
}
