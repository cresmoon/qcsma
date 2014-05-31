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

package csma;

import csma.Source;
import csma.CSMAQueue;
import csma.WMedium;
import csma.AccessPoint;
import csma.Dest;

network CSMANet
{
    parameters:
        int netsize;
    @display("bgb=744,371");
    types:
        channel Channel extends ned.DelayChannel
        {
            delay = 100ms;
        }
    submodules:
        //src[4]: Source;
        src[netsize]: Source;
        src[0]: Source {
            @display("p=20,222");
        }
        //Q[4]: msQueue;
        Q[netsize]: CSMAQueue;
        Q[0]: CSMAQueue {
            @display("p=176,116");
        }
        WMedium: WMedium {
            @display("p=365,148");
        }
        AP: AccessPoint {
            @display("p=560,116");
        }
 
        dest[netsize]: Dest;
        dest[0]: Dest {
            @display("p=709,214");
        }
    connections:
        for i=0..sizeof(src)-1 {
        	src[i].gate <--> Q[i].gate++;
        	Q[i].gate++ <--> WMedium.gate++;
        }
    
        WMedium.gate++ <--> AP.gate++;

		for i=0..sizeof(dest)-1{
		    AP.gate++ <--> dest[i].gate;
		}
}

