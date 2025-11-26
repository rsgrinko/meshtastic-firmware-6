#include "ReplyModule.h"
#include "MeshService.h"
#include "configuration.h"
#include "main.h"

#include <assert.h>
/*
meshtastic_MeshPacket *ReplyModule::allocReply()
{
    assert(currentRequest); // should always be !NULL
#ifdef DEBUG_PORT
    auto req = *currentRequest;
    auto &p = req.decoded;
    // The incoming message is in p.payload
    LOG_INFO("Received message from=0x%0x, id=%d, msg=%.*s", req.from, req.id, p.payload.size, p.payload.bytes);
#endif

    screen->print("Send reply\n");

    const char *replyStr = "Message Received";
    auto reply = allocDataPacket();                 // Allocate a packet for sending
    reply->decoded.payload.size = strlen(replyStr); // You must specify how many bytes are in the reply
    memcpy(reply->decoded.payload.bytes, replyStr, reply->decoded.payload.size);

    return reply;
}*/

ProcessMessage ReplyModule::handleReceived(const meshtastic_MeshPacket &mp)
{
    uint32_t nodeId = nodeDB->getNodeNum();
    // Проверяем, нам ли адресован этот пакет
    // Как вариант защиты от броадкаста можно проверять через  isBroadcast(mp.to)
    // if (isBroadcast(mp.to)) {

    if (false && mp.to == nodeId) {
        auto reply = allocDataPacket();
        float P = mp.rx_rssi + mp.rx_snr - 10*log10f(pow10f(mp.rx_snr/10)+1);
        sprintf(reinterpret_cast<char*>(reply->decoded.payload.bytes), "RSSI: %ddBm, SNR: %.2fdB, Signal: %.2fdBm", mp.rx_rssi, mp.rx_snr, P);
        reply->decoded.payload.size = strlen(reinterpret_cast<char*>(reply->decoded.payload.bytes));

        setReplyTo(reply, mp);
        service->sendToMesh(reply);
    }

    return ProcessMessage::CONTINUE;
}