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

#ifndef NPUSHREPORTALTITUDE_H
#define NPUSHREPORTALTITUDE_H

#include "FilesystemConventions.h"

#include "npushgenreport.h"
#include <QVector>
#include <QObject>
#include <QMutex>
#include <QMutexLocker>

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>

#include "selfshrinkinglist.h"

class NPushReportAltitude : public NPushGenReport
{
    Q_OBJECT
public:
    NPushReportAltitude();
    ~NPushReportAltitude();

    bool save_to_dir(QString& dirName);
    bool load_from_dir(QString& dirName);

    bool start_run();
    bool end_run();

    void append_new_reading(double speed);

    bool running;

    SelfShrinkingList graphPoints;

    double Altitude_max;
    double Altitude_min;

private:

    void read_points_from_xml(QXmlStreamReader& xml);

    QMutex dataAccessMutex;
};

#endif // NPUSHREPORTALTITUDE_H
