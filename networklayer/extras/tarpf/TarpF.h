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

#ifndef __INET_TARPF_H_
#define __INET_TARPF_H_

#include <list>

#include "inet/common/packet/Packet.h"
#include "inet/networklayer/base/NetworkProtocolBase.h"
#include "inet/networklayer/common/L3Address.h"
#include "inet/networklayer/contract/INetworkProtocol.h"
#include "inet/networklayer/extras/tarpf/TarpFHeader_m.h"

namespace inet {

/**
 * @brief A controlled flooding protocol
 *
 * TODO - Generated class
 *
 * @ingroup netwLayer
 * @author Kavinda Perera
 *
 */
class INET_API TarpF : public NetworkProtocolBase, public INetworkProtocol
{
    public:


        /** @brief Initialization of omnetpp.ini parameters*/
        virtual int numInitStages() const override { return NUM_INIT_STAGES; }

        virtual void initialize(int) override;

        virtual void finish() override;

        const Protocol& getProtocol() const override { return Protocol::tarpf; }

    protected:

        /** @brief Handle messages from upper layer */
        virtual void handleUpperPacket(Packet *packet) override;

        /** @brief Handle messages from lower layer */
        virtual void handleLowerPacket(Packet *packet) override;

        /** @brief Checks whether a message was already broadcasted*/
        bool notBroadcasted(const TarpFHeader *);

        void decapsulate(Packet *packet);
        void encapsulate(Packet *packet);


        // OperationalBase:
        virtual void handleStartOperation(LifecycleOperation *operation) override {}    //TODO implementation
        virtual void handleStopOperation(LifecycleOperation *operation) override {}    //TODO implementation
        virtual void handleCrashOperation(LifecycleOperation *operation) override {}    //TODO implementation

};

} //namespace inet

#endif
