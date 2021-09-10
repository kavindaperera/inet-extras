//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//
// @author Kavinda Perera
// 

#include "inet/common/ProtocolTag_m.h"
#include "inet/linklayer/common/MacAddressTag_m.h"
#include "inet/networklayer/common/HopLimitTag_m.h"
#include "inet/networklayer/common/L3AddressTag_m.h"
#include "inet/networklayer/contract/IL3AddressType.h"
#include "inet/networklayer/extras/tarpf/TarpF.h"

namespace inet {

using std::endl;

Define_Module(TarpF);

void TarpF::initialize(int stage) {
    NetworkProtocolBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        // initialize sequence number to 0
        seqNum = 0;

        // statistics
        nbDataPacketsReceived = 0;
        nbDataPacketsSent = 0;
        nbDataPacketsForwarded = 0;
        nbHops = 0;

        //parameters
        headerLength = par("headerLength");
        defaultTtl = par("defaultTtl");
        plainFlooding = par("plainFlooding");

        EV << "defaultTtl = " << defaultTtl << " plainFlooding = "
                  << plainFlooding << endl;

        if (plainFlooding) {
            // settings duplicate discard cache parameters
            ddMaxEntries = par("ddMaxEntries");
            ddDelTime = par("ddDelTime");
            EV << "ddMaxEntries = " << ddMaxEntries << " ddDelTime = "
                      << ddDelTime << endl;
        }

    } else if (stage == INITSTAGE_NETWORK_LAYER) {
        auto ie = interfaceTable->findFirstNonLoopbackInterface();
        if (ie != nullptr)
            myNetwAddr = ie->getNetworkAddress();
        else
            throw cRuntimeError("No non-loopback interface found!");
    }

}

void TarpF::finish() {
    if (plainFlooding) {
        ddCache.clear();
    }
    recordScalar("nbDataPacketsReceived", nbDataPacketsReceived);
    recordScalar("nbDataPacketsSent", nbDataPacketsSent);
    recordScalar("nbDataPacketsForwarded", nbDataPacketsForwarded);
    if (nbDataPacketsReceived > 0) {
        recordScalar("meanNbHops",
                (double) nbHops / (double) nbDataPacketsReceived);
    } else {
        recordScalar("meanNbHops", 0);
    }
}

void TarpF::handleUpperPacket(Packet *packet) {

    encapsulate(packet);
    auto tarpfHeader = packet->peekAtFront<TarpFHeader>();

    if (plainFlooding) {
        // check and update DD cache

        if (ddCache.size() >= ddMaxEntries) {
            // search and delete out-dated messages
            for (auto it = ddCache.begin(); it != ddCache.end();) {
                if (it->delTime < simTime())
                    it = ddCache.erase(it);
                else
                    ++it;

            }

            // delete oldest entry if still max size reached
            if (ddCache.size() >= ddMaxEntries) {
                EV << "ddCache is full, delete oldest entry" << endl;
                ddCache.pop_front();
            }
        }

        ddCache.push_back(
                Bcast(tarpfHeader->getSeqNum(), tarpfHeader->getSourceAddress(),
                        simTime() + ddDelTime));

    }

    // send to broadcast
    sendDown(packet);
    nbDataPacketsSent++;
}

void TarpF::handleLowerPacket(Packet *packet) {

    auto tarpfHeader = packet->peekAtFront<TarpFHeader>();

    if (notBroadcasted(tarpfHeader.get())) {

        // if msg is for me -> sendUp
        if (interfaceTable->isLocalAddress(
                tarpfHeader->getDestinationAddress())) {

            EV << " data msg for me! send to Upper" << endl;
            nbHops = nbHops + (defaultTtl + 1 - tarpfHeader->getTtl());
            decapsulate(packet);
            sendUp(packet);
            nbDataPacketsReceived++;
        }

        // else if broadcast message
        else if (tarpfHeader->getDestinationAddress().isBroadcast()) {

            // check ttl and rebroadcast
            if (tarpfHeader->getTtl() > 1) {
                EV << " data msg BROADCAST! ttl = " << tarpfHeader->getTtl()
                          << " > 1 -> rebroadcast msg & send to upper\n";

                auto dMsg = packet->dup();
                auto newTarpFHeader = dMsg->removeAtFront<TarpFHeader>();
                newTarpFHeader->setTtl(newTarpFHeader->getTtl() - 1);
                dMsg->insertAtFront(newTarpFHeader);
                setDownControlInfo(dMsg, MacAddress::BROADCAST_ADDRESS);
                sendDown(dMsg);
                nbDataPacketsForwarded++;
            } else
                EV << " max hops reached (ttl = " << tarpfHeader->getTtl()
                          << ") -> only send to upper\n";

            // message has to be forwarded to upper layer
            nbHops = nbHops + (defaultTtl + 1 - tarpfHeader->getTtl());
            decapsulate(packet);
            sendUp(packet);
            nbDataPacketsReceived++;

        }

        // else not for me -> rebroadcast
        else {
            // check ttl and rebroadcast
            if (tarpfHeader->getTtl() > 1) {
                EV << " data msg not for me! ttl = " << tarpfHeader->getTtl()
                          << " > 1 -> forward" << endl;
                decapsulate(packet);

                auto packetCopy = new Packet();
                packetCopy->insertAtBack(
                        packet->peekDataAt(b(0), packet->getDataLength()));

                auto tarpfHeaderCopy = staticPtrCast<TarpFHeader>(
                        tarpfHeader->dupShared());
                tarpfHeaderCopy->setTtl(tarpfHeader->getTtl() - 1);

                packetCopy->insertAtFront(tarpfHeaderCopy);

                // needs to set the next hop address again to broadcast
                cObject *const pCtrlInfo = packetCopy->removeControlInfo();
                if (pCtrlInfo != nullptr)
                    delete pCtrlInfo;

                setDownControlInfo(packetCopy, MacAddress::BROADCAST_ADDRESS);
                sendDown(packetCopy);
                nbDataPacketsForwarded++;
                delete packet;
            } else {
                // max hops reached -> delete
                EV << " max hops reached (ttl = " << tarpfHeader->getTtl()
                          << ") -> delete msg\n";
                delete packet;
            }
        }

    } else {
        EV << " data msg already BROADCASTed! delete msg\n";
        delete packet;
    }

}

bool TarpF::notBroadcasted(const TarpFHeader *msg) {

    if (!plainFlooding)
        return true;

    // search ddCacahe for out-dated entries and delete them
    for (auto it = ddCache.begin(); it != ddCache.end();) {
        if (it->delTime < simTime()) {
            it = ddCache.erase(it);
        }
        // message was already broadcasted
        else if ((it->srcAddr == msg->getSourceAddress())
                && (it->seqNum == msg->getSeqNum())) {
            // update
            it->delTime = simTime() + ddDelTime;
            return false;
        } else
            ++it;
    }

    // delete oldest entry if max size is reached
    if (ddCache.size() >= ddMaxEntries) {
        EV << "ddCache is full, delete oldest entry\n";
        ddCache.pop_front();
    }

    ddCache.push_back(
            Bcast(msg->getSeqNum(), msg->getSourceAddress(),
                    simTime() + ddDelTime));
    return true;
}

/**
 * Decapsulates the packet from the received Network packet
 **/
void TarpF::decapsulate(Packet *packet) {

    EV << " decapsulating...\n";

    auto tarpfHeader = packet->popAtFront<TarpFHeader>();
    auto payloadLength = tarpfHeader->getPayloadLengthField();
    if (packet->getDataLength() < payloadLength) {
        throw cRuntimeError("Data error: illegal payload length");
    }
    if (packet->getDataLength() > payloadLength)
        packet->setBackOffset(packet->getFrontOffset() + payloadLength);

    auto payloadProtocol = tarpfHeader->getProtocol();

    packet->addTagIfAbsent<NetworkProtocolInd>()->setProtocol(&getProtocol());
    packet->addTagIfAbsent<NetworkProtocolInd>()->setNetworkProtocolHeader(
            tarpfHeader);
    packet->addTagIfAbsent<DispatchProtocolReq>()->setProtocol(payloadProtocol);
    packet->addTagIfAbsent<PacketProtocolTag>()->setProtocol(payloadProtocol);

    auto addressInd = packet->addTagIfAbsent<L3AddressInd>();

    addressInd->setSrcAddress(tarpfHeader->getSourceAddress());
    addressInd->setDestAddress(tarpfHeader->getDestinationAddress());
    packet->addTagIfAbsent<HopLimitInd>()->setHopLimit(tarpfHeader->getTtl());

    EV << " pkt decapsulated\n";

}

/**
 * Encapsulates the received ApplPkt into a NetwPkt and set all needed
 * header fields.
 **/
void TarpF::encapsulate(Packet *appPkt) {
    L3Address netwAddr;

    EV << " encapsulating.." << appPkt << endl;

    auto cInfo = appPkt->removeControlInfo();
    auto pkt = makeShared<TarpFHeader>();
    pkt->setChunkLength(b(headerLength));

    auto hopLimitReq = appPkt->removeTagIfPresent<HopLimitReq>();
    int ttl = (hopLimitReq != nullptr) ? hopLimitReq->getHopLimit() : -1;
    delete hopLimitReq;
    if (ttl == -1)
        ttl = defaultTtl;

    pkt->setSeqNum(seqNum);
    seqNum++;
    pkt->setTtl(ttl);

    auto addressReq = appPkt->findTag<L3AddressReq>();
    if (addressReq == nullptr) {
        EV
                  << "warning: Application layer did not specify a destination L3 address\n"
                  << "\t using broadcast address instead\n";
    } else {
        pkt->setProtocol(appPkt->getTag<PacketProtocolTag>()->getProtocol());
        netwAddr = addressReq->getDestAddress();
        EV << " cInfo removed, netw addr=" << netwAddr << endl;
        delete cInfo;
    }

    pkt->setSrcAddr(myNetwAddr);
    pkt->setDestAddr(netwAddr);

    EV << " pkt: seqNum=" << pkt->getSeqNum() << " ttl=" << pkt->getTtl()
              << " srcAddr=" << pkt->getSrcAddr() << " destAddr="
              << pkt->getDestAddr() << endl;

    EV << " netw " << myNetwAddr << " sending packet" << endl;

    EV << "sendDown: nHop=L3BROADCAST -> message has to be broadcasted"
              << " -> set destMac=L2BROADCAST" << endl;

    pkt->setPayloadLengthField(appPkt->getDataLength());

    //encapsulate the application packet
    setDownControlInfo(appPkt, MacAddress::BROADCAST_ADDRESS);

    // TODO - Generated method body

    appPkt->insertAtFront(pkt);
    EV << " pkt encapsulated\n";

}

/**
 * Attaches a "control info" structure (object) to the down message pMsg.
 */
void TarpF::setDownControlInfo(Packet *const pMsg,
        const MacAddress &pDestAddr) {
    pMsg->addTagIfAbsent<MacAddressReq>()->setDestAddress(pDestAddr);
    pMsg->addTagIfAbsent<PacketProtocolTag>()->setProtocol(&getProtocol());
    pMsg->addTagIfAbsent<DispatchProtocolInd>()->setProtocol(&getProtocol());
}

} //namespace inet
