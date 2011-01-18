#include "pushdevicesholder.h"

PushDevicesHolder::PushDevicesHolder()
{
    tickDisposer = new NPushTickDisposer();
}

PushDevicesHolder::~PushDevicesHolder()
{
    for(int i=0;i<this->count();i++)
    {
        this->at(i)->disconnect_from_backend();
        this->at(i)->deleteLater();
    }

    if(tickDisposer) {
        qDebug() << "Calling delete later for tick disposer";
        tickDisposer->deleteLater();
    }
}

int PushDevicesHolder::deleteAllAndClear()
{
    int i;
    for(i=0; i < this->count(); i++)
    {
        PushBurtonGenericDevice * iDev = this->at(i);
        iDev->deleteLater();
    }
    this->clear();
    return i;
}

bool PushDevicesHolder::CheckForDeviceType(QString& device_type)
{
//    qDebug() << "checking for: " << device_type;
    for(int i=0; i < this->count(); i++)
    {
        QString thisDevTypeName(typeid(*(this->at(i))).name());
//        qDebug() << "against: " << thisDevTypeName;
        if(thisDevTypeName == device_type)
        {
            return true;
        }
    }
    return false;
}

void PushDevicesHolder::push_back(PushBurtonGenericDevice* ptr)
{
    qDebug() << "Adding device to PushDevicesHolder internal list";
    QVector<PushBurtonGenericDevice *>::push_back(ptr);

    //Essential connection to logTicksDisposer! Without this the program is a dangling huge memory leaker
    ptr->connect(ptr, SIGNAL(reading_ready(NPushLogTick*)), tickDisposer, SLOT(log_ticks_sink(NPushLogTick*)), Qt::UniqueConnection);

    if(typeid(*ptr) == typeid(PushBurtonGenericDevice)){
        qDebug() << "Initial type check O.K.";
    }

    //checking if new device subscribes to any of the availables devices
    if(ptr->subscribesToAny()) {
        for(int i=0;i<this->count();i++) {
            PushBurtonGenericDevice* iptr = this->at(i);
            //If new device subscribes to it //PS: This enables recursion!!
            if(ptr->subscribesTo(iptr) || typeid(*iptr) == typeid(PushN8SimulationDevice)) {
                qDebug() << "Connected " << iptr->get_description() << "\'s output to " << ptr->get_description();
                ptr->connect(iptr, SIGNAL(reading_ready(NPushLogTick*)), ptr, SLOT(incomming_reading(NPushLogTick*)), Qt::UniqueConnection);
            }
        }
    }

    //Checking if previously available device subscribes to newly added device
    for(int i=0;i<this->count()-1;i++) {
        PushBurtonGenericDevice* iptr = this->at(i);
        if(iptr->subscribesToAny()) {
            if(iptr->subscribesTo(ptr)) {
                qDebug() << "Connected " << ptr->get_description() << "\'s output to " << iptr->get_description();
                iptr->connect(ptr, SIGNAL(reading_ready(NPushLogTick*)), iptr, SLOT(incomming_reading(NPushLogTick*)), Qt::UniqueConnection);
            }
        }
    }
}
