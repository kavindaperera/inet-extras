//
// TODO auto-generated module
//
// @author Kavinda Perera
//

#ifndef INET_NETWORKLAYER_EXTRAS_TARPF_TARPFPROTOCOLDISSECTOR_H_
#define INET_NETWORKLAYER_EXTRAS_TARPF_TARPFPROTOCOLDISSECTOR_H_

#include "inet/common/INETDefs.h"
#include "inet/common/packet/dissector/ProtocolDissector.h"


namespace inet {

class INET_API TarpFProtocolDissector : public ProtocolDissector
{
  public:
    virtual void dissect(Packet *packet, const Protocol *protocol, ICallback& callback) const override;
};

} // namespace inet


#endif /* INET_NETWORKLAYER_EXTRAS_TARPF_TARPFPROTOCOLDISSECTOR_H_ */
