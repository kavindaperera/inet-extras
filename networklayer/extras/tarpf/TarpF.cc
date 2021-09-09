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
        nbDataPacketsReceived = 0;
        nbDataPacketsSent = 0;
        nbDataPacketsForwarded = 0;
        nbHops = 0;

        //parameters
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
    // TODO - Generated method body
}

} //namespace inet
