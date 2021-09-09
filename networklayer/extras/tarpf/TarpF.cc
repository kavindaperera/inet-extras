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

#include "inet/networklayer/extras/tarpf/TarpF.h"

namespace inet {

using std::endl;

Define_Module(TarpF);

void TarpF::initialize(int stage)
{
    NetworkProtocolBase::initialize(stage);
    // TODO - Generated method body
}

void TarpF::finish()
{
    // TODO - Generated method body
}

void TarpF::handleUpperPacket(Packet *packet)
{
    // TODO - Generated method body
}

void TarpF::handleLowerPacket(Packet *packet)
{
    // TODO - Generated method body
}

bool TarpF::notBroadcasted(const TarpFHeader *msg)
{
    // TODO - Generated method body
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
