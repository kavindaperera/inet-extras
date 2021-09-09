/*
 * TrapFProtocolDissector.cc
 *
 *  Created on: Sep 9, 2021
 *      Author: KAVINDA
 */


#include "inet/common/packet/dissector/ProtocolDissectorRegistry.h"
#include "inet/networklayer/extras/tarpf/TarpFHeader_m.h"
#include "inet/networklayer/extras/tarpf/TarpFProtocolDissector.h"

namespace inet {

Register_Protocol_Dissector(&Protocol::tarpf, TarpFProtocolDissector);

void TarpFProtocolDissector::dissect(Packet *packet, const Protocol *protocol, ICallback& callback) const
{
    auto header = packet->popAtFront<TarpFHeader>();
    auto trailerPopOffset = packet->getBackOffset();
    auto payloadEndOffset = packet->getFrontOffset() + header->getPayloadLengthField();
    callback.startProtocolDataUnit(&Protocol::tarpf);
    callback.visitChunk(header, &Protocol::tarpf);
    packet->setBackOffset(payloadEndOffset);
    callback.dissectPacket(packet, header->getProtocol());
    ASSERT(packet->getDataLength() == B(0));
    packet->setFrontOffset(payloadEndOffset);
    packet->setBackOffset(trailerPopOffset);
    callback.endProtocolDataUnit(&Protocol::tarpf);
}

} // namespace inet
