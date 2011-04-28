#include "virtualbrosdevice.h"

VirtualBrosDevice::VirtualBrosDevice(QObject *parent) :
    PushBurtonGenericDevice(parent)
{
    applause = 0;


    QString applausePath;

#ifdef Q_OS_SYMBIAN
    //This is a tricky part but all we need here is to retrieve the path to where we deployed the sound file.
    //read more at:
    //http://wiki.forum.nokia.com/index.php/Get_private_path_in_Qt#Converting_path_from_Qt_format_to_Symbian_format
    QString privatePathQt(QApplication::applicationDirPath() );
    QString privatePathSymbian(QDir::toNativeSeparators(privatePathQt));

    applausePath = privatePathSymbian + "\\applause.mp3";
#else
    applausePath = "applause.mp3";
    QCoreApplication::setApplicationName(QString("PushSnowboarding"));
#endif

    //This loads the file and leaves it in a "ready to play" state.
    //Note also that "createPlayer" is the simplest way to playing one sound using Phonon
    //and it basically "takes ownership" of the sound sink and that sort of thing so if we
    //wanted more sounds playing concurrently we would need to use the less minimalistic calls to phonon.

    qDebug() << "Before creating media";

    applause = Phonon::createPlayer(Phonon::MusicCategory, Phonon::MediaSource(applausePath));

    qDebug() << applause->totalTime();
    qDebug() << applause->errorString();

    if(applause->isValid()) {
        qDebug() << "is valid";
    } else {
        qDebug() << "not valid";
    }

    qDebug() << "State is: " << applause->state();

}

VirtualBrosDevice::~VirtualBrosDevice()
{

    if(applause) {
        delete applause;
    }
}

QString VirtualBrosDevice::get_description()
{
    //Just giving it a name for sake of id
    return "VirtualBros";
}

bool VirtualBrosDevice::is_online()
{
    //if pointer is valid, it's good to go
    return (applause);
}

//This function is called by PushDevicesHolder before calling "subscribesTo" to all other devices
//obviously, if this returns false, PushDevicesHolder will skip that check.
bool VirtualBrosDevice::subscribesToAny()
{
    return true;
}


//This is an important bit. The code here checks if the device passed as a parameter is actually a
//PushN8AirTimeDetector. Obviously, for all other devices it'll return false.
bool VirtualBrosDevice::subscribesTo(PushBurtonGenericDevice* deviceType)
{
    if(typeid(*deviceType) == typeid(PushN8AirTimeDetector)){
        return true;
    } else {
        return false;
    }
}

void VirtualBrosDevice::incoming_reading(NPushLogTick * tick)
{
    //Checking if input tick is a AirTimeTick
    if(typeid(*tick) == typeid(NPushAirTimeTick)) {
        NPushAirTimeTick * airTimeTick = (NPushAirTimeTick *)tick;

        //The AirTimeTick provides all known data from the "current"
        //jump/airtime. For example, it'll tell you if the rider just
        //landed a jump through the bool "landed"
        if(airTimeTick->landed) {
            applause->seek(0);
            applause->play();
        }

        //The airTimeTick also brings airTimeTick->msecsOnAir which
        //tells you for how long the rider is in the air
        //Basically, when a rider does a jump, this slot will receive
        //lots of ticks with msecsOnAir increasing until the rider hits
        //the ground. Then it'll receive a tick with
        //airTimeTick->landed == true
        //and with msecsOnAir holding the final measure of how long
        //the air time was.

    }
}
