//
// Generated file, do not edit! Created by nedtool 5.6 from inet/networklayer/diffserv/Dscp.msg.
//

#ifndef __INET_DSCP_M_H
#define __INET_DSCP_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0506
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif

// dll export symbol
#ifndef INET_API
#  if defined(INET_EXPORT)
#    define INET_API  OPP_DLLEXPORT
#  elif defined(INET_IMPORT)
#    define INET_API  OPP_DLLIMPORT
#  else
#    define INET_API
#  endif
#endif


namespace inet {

} // namespace inet

#include "inet/common/INETDefs_m.h" // import inet.common.INETDefs


namespace inet {

/**
 * Enum generated from <tt>inet/networklayer/diffserv/Dscp.msg:28</tt> by nedtool.
 * <pre>
 * //
 * // DiffServ code points.
 * //
 * enum Dscp
 * {
 *     // Best Effort
 *     DSCP_BE = 0;
 * 
 *     // Assured Forwarding, RFC 2597
 *     DSCP_AF11 = 0x0A; // 001010
 *     DSCP_AF12 = 0x0C; // 001100
 *     DSCP_AF13 = 0x0E; // 001110
 * 
 *     DSCP_AF21 = 0x12; // 010010
 *     DSCP_AF22 = 0x14; // 010100
 *     DSCP_AF23 = 0x16; // 010110
 * 
 *     DSCP_AF31 = 0x1A; // 011010
 *     DSCP_AF32 = 0x1C; // 011100
 *     DSCP_AF33 = 0x1E; // 011110
 * 
 *     DSCP_AF41 = 0x22; // 100010
 *     DSCP_AF42 = 0x24; // 100100
 *     DSCP_AF43 = 0x26; // 100110
 * 
 *     // Expedited Forwarding, RFC 2598
 *     DSCP_EF = 0x2E; // 101110
 * 
 *     // Class Selector Code Points, RFC 2474
 *     DSCP_CS1 = 0x08; // 001000
 *     DSCP_CS2 = 0x10; // 010000
 *     DSCP_CS3 = 0x18; // 011000
 *     DSCP_CS4 = 0x20; // 100000
 *     DSCP_CS5 = 0x28; // 101000
 *     DSCP_CS6 = 0x30; // 110000
 *     DSCP_CS7 = 0x38; // 111000
 * 
 *     // upper bound (exclusive)
 *     DSCP_MAX = 0x40;
 * }
 * </pre>
 */
enum Dscp {
    DSCP_BE = 0,
    DSCP_AF11 = 0x0A,
    DSCP_AF12 = 0x0C,
    DSCP_AF13 = 0x0E,
    DSCP_AF21 = 0x12,
    DSCP_AF22 = 0x14,
    DSCP_AF23 = 0x16,
    DSCP_AF31 = 0x1A,
    DSCP_AF32 = 0x1C,
    DSCP_AF33 = 0x1E,
    DSCP_AF41 = 0x22,
    DSCP_AF42 = 0x24,
    DSCP_AF43 = 0x26,
    DSCP_EF = 0x2E,
    DSCP_CS1 = 0x08,
    DSCP_CS2 = 0x10,
    DSCP_CS3 = 0x18,
    DSCP_CS4 = 0x20,
    DSCP_CS5 = 0x28,
    DSCP_CS6 = 0x30,
    DSCP_CS7 = 0x38,
    DSCP_MAX = 0x40
};

} // namespace inet

#endif // ifndef __INET_DSCP_M_H

