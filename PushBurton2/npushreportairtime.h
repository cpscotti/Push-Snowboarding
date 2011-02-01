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

#ifndef NPUSHREPORTAIRTIME_H
#define NPUSHREPORTAIRTIME_H

#include "FilesystemConventions.h"

#include "npushgenreport.h"
#include <QList>
#include <QObject>
#include <QMutex>
#include <QMutexLocker>

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>

class NPushReportAirTime : public NPushGenReport
{
    Q_OBJECT
public:
    NPushReportAirTime();
    ~NPushReportAirTime();

    void append_air(quint64 air_time, quint64 tstamp);

    bool save_to_dir(QString& dirName);
    bool load_from_dir(QString& dirName);

    bool start_run();
    bool end_run();

    void set_start_tstamp(double t);
    void set_end_tstamp(double t);

    bool running;

    //All values in SECONDS/and/or/fractions of seconds
    double longest_air_time;
    double total_air_time;
    int n_of_airs;

    //start-end timestamps
    double tRunStart;
    double tRunEnd;

    QList<double> air_times;
    QList<double> tstamps;


private:

    void read_bar_graph_from_xml(QXmlStreamReader& xml);

    QMutex dataAccessMutex;
};

#endif // NPUSHREPORTAIRTIME_H
