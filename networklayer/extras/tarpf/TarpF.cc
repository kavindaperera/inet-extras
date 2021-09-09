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

void TarpF::initialize(int stage)
{
    NetworkProtocolBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL ) {
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

        EV << "defaultTtl = " << defaultTtl << " plainFlooding = " << plainFlooding << endl;

        if (plainFlooding) {
            // settings duplicate discard cache parameters
            ddMaxEntries = par("ddMaxEntries");
            ddDelTime = par("ddDelTime");
            EV << "ddMaxEntries = " << ddMaxEntries << " ddDelTime = " << ddDelTime << endl;
        }

    } else if (stage == INITSTAGE_NETWORK_LAYER) {
        auto ie = interfaceTable->findFirstNonLoopbackInterface();
        if (ie != nullptr)
            myNetwAddr = ie->getNetworkAddress();
        else
            throw cRuntimeError("No non-loopback interface found!");
    }

}

void TarpF::finish()
{
    // TODO - Generated method body
    throw cRuntimeError("Debug!.....finish");
}

void TarpF::handleUpperPacket(Packet *packet)
{
    encapsulate(packet);

    auto tarpfHeader = packet->peekAtFront<TarpFHeader>();

    if (plainFlooding) {

        // TODO - check and update DD cache

    }

    // TODO - Generated method body

    throw cRuntimeError("Debug!.....handleUpperPacket");
}

void TarpF::handleLowerPacket(Packet *packet)
{
    // TODO - Generated method body
    throw cRuntimeError("Debug!.....handleLowerPacket");
}

bool TarpF::notBroadcasted(const TarpFHeader *msg)
{
    // TODO - Generated method body
    throw cRuntimeError("Debug!.....notBroadcasted");
    return true;
}

/**
 * Decapsulates the packet from the received Network packet
 **/
void TarpF::decapsulate(Packet *packet)
{
    // TODO - Generated method body
}

/**
 * Encapsulates the received ApplPkt into a NetwPkt and set all needed
 * header fields.
 **/
void TarpF::encapsulate(Packet *appPkt)
{
    L3Address netwAddr;

    EV << " encapsulating.."<< appPkt << endl;

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
        EV << "warning: Application layer did not specify a destination L3 address\n"
                << "\t using broadcast address instead\n";
    }
    else {
        pkt->setProtocol(appPkt->getTag<PacketProtocolTag>()->getProtocol());
        netwAddr = addressReq->getDestAddress();
        EV << " cInfo removed, netw addr=" << netwAddr << endl;
        delete cInfo;
    }

    pkt->setSrcAddr(myNetwAddr);
    pkt->setDestAddr(netwAddr);

    EV << " pkt: seqNum=" << pkt->getSeqNum()
                   << " ttl=" << pkt->getTtl()
                   << " srcAddr=" << pkt->getSrcAddr()
                   << " destAddr=" << pkt->getDestAddr()
                   << endl;

    EV << " netw " << myNetwAddr << " sending packet" << endl;


    // TODO - Generated method body

    appPkt->insertAtFront(pkt);
    EV << " pkt encapsulated\n";


}

} //namespace inet
