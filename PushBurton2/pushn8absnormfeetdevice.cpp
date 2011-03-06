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

#include "pushn8absnormfeetdevice.h"

PushN8AbsNormFeetDevice::PushN8AbsNormFeetDevice()
{

    lastReading = 0;
    lastReadingSide = 'R';

    reset_bounds();
}

PushN8AbsNormFeetDevice::~PushN8AbsNormFeetDevice()
{

}


void PushN8AbsNormFeetDevice::reset_bounds()
{
    pdata.reset();

    normRunCount = 0;

    flying = false;
}

bool PushN8AbsNormFeetDevice::subscribesToAny()
{
    return true;
}

bool PushN8AbsNormFeetDevice::subscribesTo(PushBurtonGenericDevice* deviceType)
{
    if(typeid(*deviceType) == typeid(PushN8FootDevice))
    {
        return true;
    } else {
        return false;
    }
}

QString PushN8AbsNormFeetDevice::get_description()
{
    return "NormalizedFeetAnalizer";
}

void PushN8AbsNormFeetDevice::start_readings()
{
    reset_bounds();
}

void PushN8AbsNormFeetDevice::stop_readings()
{

}

bool PushN8AbsNormFeetDevice::is_online()
{
    return (normRunCount > 0);
}


void PushN8AbsNormFeetDevice::incoming_reading(NPushLogTick * gtick)
{
    if(typeid(*gtick) == typeid(NPushFootTick)) {
        NPushFootTick * ftick = (NPushFootTick *)gtick;

        if(ftick->side == 'L') {
            pdata.LToe.update(
                    (ftick->toes > FOOT_PRESSURE_INVALID_THRESHOLD)?
                    ftick->toes : pdata.RToe.rawVal
                    );

            pdata.LHeel.update(
                    (ftick->heel > FOOT_PRESSURE_INVALID_THRESHOLD)?
                    ftick->heel : pdata.RHeel.rawVal
                    );
        } else {
            pdata.RToe.update(
                    (ftick->toes > FOOT_PRESSURE_INVALID_THRESHOLD)?
                    ftick->toes : pdata.LToe.rawVal
                    );

            pdata.RHeel.update(
                    (ftick->heel > FOOT_PRESSURE_INVALID_THRESHOLD)?
                    ftick->heel : pdata.LHeel.rawVal
                    );
        }

        if((ftick->msecsToEpoch - lastReading) < 100 &&
           (ftick->side != lastReadingSide)) {

            pdata.computeSum();
            pdata.computeNormFactor();

            normRunCount++;

            NPushNormFeetTick * newTick = new NPushNormFeetTick(pdata, lastReading);

            if(receivers(SIGNAL(reading_ready(NPushLogTick*))) > 0)
            {
                emit reading_ready(newTick);
            } else {
                delete newTick;
            }

        }

        lastReading = ftick->msecsToEpoch;
        lastReadingSide = ftick->side;

    }
}
