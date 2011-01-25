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

#include "selfshrinkinglist.h"

SelfShrinkingList::SelfShrinkingList(int maxSize)
{
    if(maxSize%2 == 0) {
        maxLength = maxSize;
    }else {
        maxLength = maxSize+1;
    }
    valsPrescaler = 1;

    invalid_val = -1.0;
}

SelfShrinkingList::SelfShrinkingList(int maxSize, double invVal)
{
    if(maxSize%2 == 0) {
        maxLength = maxSize;
    }else {
        maxLength = maxSize+1;
    }
    valsPrescaler = 1;

    invalid_val = invVal;
}

SelfShrinkingList::~SelfShrinkingList()
{

}

void SelfShrinkingList::push_back(double x)
{
    if(valsPrescaler > 1)
    {
        inputBuffer.enqueue(x);
        if(inputBuffer.count() == valsPrescaler)
        {
            double sp_acc = 0.0;

            int invalidsCount = 0;

            while(!inputBuffer.empty())
            {
                double this_val = inputBuffer.dequeue();
                if(this_val == invalid_val) {
                    invalidsCount++;
                } else {
                    sp_acc += this_val;
                }
            }
            if(valsPrescaler > invalidsCount) {
                sp_acc /= (valsPrescaler - invalidsCount);
            } else {
                sp_acc = invalid_val;
            }
            QList<double>::push_back(sp_acc);
        }
    } else {
        //this is not necessary but is a LOT faster and in a way, makes the code more self-explainable
        QList<double>::push_back(x);
    }

    //maxLength is ALWAYS EVEN (%2 == 0)
    if(QList<double>::count() == maxLength)
    {
        qDebug() << "Downscaling graph pts. Scaler = " << valsPrescaler << "; Qt=" << maxLength;
        QList<double>::iterator it;
        for(it = QList<double>::begin(); it != QList<double>::end(); ++it)
        {
            double newVal, lVal, rVal;
            lVal = *it;

            ++it;
//            ASSERT(it != QList<double>::end());

            rVal = *it;

            if(lVal == invalid_val) {
                newVal = rVal;
            } else if(rVal == invalid_val) {
                newVal = lVal;
            } else{
                newVal = (rVal+lVal)/2.0;
            }

            //result is stored in the odd elements (see next comment!!)
            *it = newVal;
        }

        //Erasing the even elements
        for(it = QList<double>::begin(); it != QList<double>::end();)
        {
            it = QList<double>::erase(it);

//            ASSERT(it != QList<double>::end());
            ++it;
        }

        valsPrescaler *= 2;
    }
}

void SelfShrinkingList::clear()
{
    QList<double>::clear();
    valsPrescaler = 1;
    inputBuffer.clear();
}
