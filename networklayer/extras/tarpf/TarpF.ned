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

package inet.networklayer.extras.tarpf;

import inet.networklayer.base.NetworkProtocolBase;
import inet.networklayer.contract.INetworkProtocol;

//
// A controlled flooding protocol for network-level broadcast.
//
// TODO auto-generated module
//
// @author Kavinda Perera
//
simple TarpF extends NetworkProtocolBase like INetworkProtocol
{
    parameters:
        @display("i=block/fork");
        
        // length of the network packet header (in bits)
        int headerLength @unit(b) = default(96b); 
        
        // flag whether to use plain flooding
        bool plainFlooding = default(true); 
        
        // Max number of entries in the duplicate discard cache
        int ddMaxEntries = default(100); 
        
        // Time after which a duplicate discard cache entry can be deleted.
        int ddDelTime @unit(s) = default(100 s); 
        
        // time-to-live () used for this module expressed in  number of hops.
        int maxHopCount = default(10); 
        
        bool spdRule = default(true); 
        
        int spdMaxEntries = default(100); 
        
        int spdDelTime @unit(s) = default(100 s); 
        
        @class(TarpF);
        
}
