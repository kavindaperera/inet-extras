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
#include <map>

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
class INET_API TarpF: public NetworkProtocolBase, public INetworkProtocol {
protected:

    /** @brief Network layer sequence number*/
    unsigned long seqNum = 0;

    /** @brief cached variable of my networ address */
    L3Address myNetwAddr;

    /** @brief Length of the header*/
    int headerLength = 0;

    /** @brief Default time-to-live () used for this module*/
    int maxHopCount = 0;

    /** @brief Defines whether to use plain flooding or not*/
    bool plainFlooding = false;

    bool spdRule = false;


    class Bcast {
    public:
        unsigned long seqNum;
        L3Address srcAddr;
        simtime_t delTime;
    public:
        Bcast(unsigned long n = 0, const L3Address &s = L3Address(),
                simtime_t_cref d = SIMTIME_ZERO) :
                seqNum(n), srcAddr(s), delTime(d) {
        }

    };

    class SpdEntry {
    public:
        int         hopCount;
        simtime_t   delTime;
    public:
        SpdEntry(int hopCount = 0, simtime_t_cref d = SIMTIME_ZERO) :
            hopCount(hopCount), delTime(d) {

        }
    };

    typedef std::list<Bcast> cTarpCache;

    /** @brief List of already broadcasted messages*/
    cTarpCache ddCache;

    /** @brief Max number of entries in the duplicate discard cache*/
    unsigned int ddMaxEntries = 0;

    /** @brief Time after which a duplicate discard cache entry can be deleted*/
    simtime_t ddDelTime;


    typedef std::map<L3Address, SpdEntry> cTarpSpdCache;

    /** @brief Map of <L3Address,SpdEntry> indexed by Node*/
    cTarpSpdCache spdCache;

    /** @brief Max number of entries in the Sub-optimal Path Discard cache*/
    unsigned int spdMaxEntries = 0;

    /** @brief Time after which a Sub-optimal Path Discard entry can be deleted*/
    simtime_t spdDelTime;

    unsigned int slack = 0;
    unsigned int relax = 0;


    long nbDataPacketsReceived = 0;
    long nbDataPacketsSent = 0;
    long nbDataPacketsForwarded = 0;
    long nbSpdDrops = 0;
    long nbHops = 0;

public:
    TarpF() {
    }

    /** @brief Initialization of omnetpp.ini parameters*/
    virtual int numInitStages() const override {
        return NUM_INIT_STAGES;
    }

    virtual void initialize(int) override;

    virtual void finish() override;

    const Protocol& getProtocol() const override {
        return Protocol::tarpf;
    }

protected:

    /** @brief Handle messages from upper layer */
    virtual void handleUpperPacket(Packet *packet) override;

    /** @brief Handle messages from lower layer */
    virtual void handleLowerPacket(Packet *packet) override;

    /** @brief Checks whether a message was already broadcasted*/
    bool notBroadcasted(const TarpFHeader*);

    bool isSubOptimal(const TarpFHeader*);

    void decapsulate(Packet *packet);
    void encapsulate(Packet *packet);

    virtual void setDownControlInfo(Packet *const pMsg, const MacAddress& pDestAddr);

    // OperationalBase:
    virtual void handleStartOperation(LifecycleOperation *operation) override {
    }    //TODO implementation
    virtual void handleStopOperation(LifecycleOperation *operation) override {
    }    //TODO implementation
    virtual void handleCrashOperation(LifecycleOperation *operation) override {
    }    //TODO implementation

};

} //namespace inet

#endif
