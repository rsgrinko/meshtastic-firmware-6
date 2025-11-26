#include "ExtensionModule.h"
#include "MeshService.h"
#include "configuration.h"
#include "main.h"

#include <assert.h>

#define POW10F(x) powf(10, (x))
char buffer_[256];

ExtensionModule::ExtensionModule() : MeshModule("ExtensionModule")
{
}

ProcessMessage ExtensionModule::handleReceived(const meshtastic_MeshPacket &mp) {
    bool isDecoded = mp.which_payload_variant == meshtastic_MeshPacket_decoded_tag;
    bool toUs = isToUs(&mp);
    bool broadcast = isBroadcast(mp.to);

    if (isDecoded && toUs && mp.decoded.portnum == meshtastic_PortNum_TEXT_MESSAGE_APP) bot(mp);   

    return ProcessMessage::CONTINUE;
}

void ExtensionModule::bot(const meshtastic_MeshPacket &mp) {
    char *buffer = buffer_;
    *buffer = 0;

    if (mp.decoded.payload.size != 1)  return;

    switch(mp.decoded.payload.bytes[0]) {
        case 's':
        case 'S': 
        {
            float Pn = mp.rx_rssi - 10*log10f(POW10F(mp.rx_snr/10)+1);
            float Ps = mp.rx_snr + Pn;

            sprintf(buffer, "RSSI: %ddBm; SNR: %.2fdB; Signal: %.2fdBm; Noise: %.2fdBm", mp.rx_rssi, mp.rx_snr, Ps, Pn);
        }
        break;

        case 'p':
        case 'P': 
        {
            strcpy(buffer, "pong");
        }
        break;
        default:
        break;
    }

    // Отправка сообщения
    if (*buffer) {
        auto reply = router->allocForSending();
        reply->decoded.portnum = meshtastic_PortNum_TEXT_MESSAGE_APP;
        strcpy(reinterpret_cast<char*>(reply->decoded.payload.bytes), buffer);
        reply->decoded.payload.size = strlen(buffer);
        setReplyTo(reply, mp);
        service->sendToMesh(reply);
    }
}
