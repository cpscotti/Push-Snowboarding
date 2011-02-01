/*
 * Copyright (c) 2011 Nokia Corporation
 *
 * This file is part of the Push Snowboarding Project, More info at:
 * www.pushsnowboading.com
 *
 * Author: Clovis Scotti <scotti@ieee.org>
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 *
 */

#ifndef UBIQLOGSAVER_H
#define UBIQLOGSAVER_H

#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>
#include <QQueue>
#include <QFile>
#include <QFSFileEngine>
#include <QXmlStreamWriter>

#include "FilesystemConventions.h"

#include "npushlogtick.h"
#include "npushgenreport.h"

class UbiqLogSaver : public QThread
{
    Q_OBJECT
public:
    UbiqLogSaver(uint a_start_time);
    ~UbiqLogSaver();
    void run();

    void run_end();

public slots:
    void log_tick_in(NPushLogTick*);
    void report_in(NPushGenReport*);

private:
    void CreateRunDir();
    QQueue<NPushLogTick*> ticksBuffer;
    QQueue<NPushGenReport*> reportBuffer;

    uint start_time;
    bool saving_ticks;

    QMutex ticksLock;
    QMutex reportsLock;

    QWaitCondition runControlSignal;
    QMutex runControlLock;

    bool runEnd;
    QString dirName;

    QFile* dataFile;
    QXmlStreamWriter xml;
};

#endif // UBIQLOGSAVER_H
