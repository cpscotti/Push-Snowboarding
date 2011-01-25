/*
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef QBTCONSTANTS_H
#define QBTCONSTANTS_H

#include <QBtGlobal.h>

/**
 * Basic enumerations of bluetooth UUIDs
 */
class DLL_EXPORT QBtConstants
{
public:
    enum ServiceClass{
            UndefinedClass = 0x0000,
            SerialPort = 0x1101,
            LANAccessUsingPPP,
            DialupNetworking,
            IrMCSync,
            OBEXObjectPush,
            OBEXFileTransfer,
            IrMCSyncCommand,
            Headset,
            CordlessTelephony,
            AudioSource,
            AudioSink,
            RemoteControlTarget,
            AdvancedAudioDistribution,
            AV_RemoteControl,
            VideoConferencing,
            Intercom,
            Fax,
            HeadsetAudioGateway,
            WAP,
            WAP_CLIENT,
            PANU,
            NAP,
            GN,
            DirectPrinting,
            ReferencePrinting,
            Imaging,
            ImagingResponder,
            ImagingAutomaticArchive,
            ImagingReferencedObjects,
            Handsfree,
            HandsfreeAudioGateway,
            DirectPrintingReferenceObjectsService,
            ReflectedUI,
            BasicPrinting,
            PrintingStatus,
            HumanInterfaceDeviceService,
            HardcopyCableReplacement,
            HCR_Print,
            HCR_Scan,
            Common_ISDN_Access,
            VideoConferencingGW,
            UDI_MT,
            UDI_TA,
            Audio_Video,
            SIM_Access,
            PnPInformation,
            GenericNetworking,
            GenericFileTransfer,
            GenericAudio,
            GenericTelephony,
            UPNP_Service,
            UPNP_IP_Service,
            ESDP_UPNP_IP_PAN,
            ESDP_UPNP_IP_LAP,
            ESDP_UPNP_L2CAP
        };

    enum ServiceProtocol{
            UndefinedProtocol 	= 0x0000,
            SDP,
            UDP,
            RFCOMM,
            TCP,
            TCS_BIN,
            TCS_AT,
            OBEX 		= 0x0008,
            IP,
            FTP,
            HTTP		= 0x000C,
            WSP			= 0x000E,
            BNEP,
            UPNP,
            HIDP,
            HardcopyControlChannel,
            HardcopyDataChannel = 0x0014,
            HardcopyNotification = 0x0016,
            AVCTP,
            AVDTP = 0x0019,
            CMPT = 0x001B,
            UDI_C_Plane = 0x001D,
            L2CAP = 0x0100
        };
};


#endif // QBTCONSTANTS_H
