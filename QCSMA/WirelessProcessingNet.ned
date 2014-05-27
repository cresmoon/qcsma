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
import csma.msQueue;
import csma.WMedium;
import csma.AccessPoint;
import csma.Dest;

network WirelessProcessingNet
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
        Q[netsize]: msQueue;
        Q[0]: msQueue {
            @display("p=176,116");
        }
        WMedium: WMedium {
            @display("p=365,148");
        }
        AP: AccessPoint {
            @display("p=560,116");
        }
        //dest[4]: Dest;
        dest[netsize]: Dest;
        dest[0]: Dest {
            @display("p=709,214");
        }
    connections:
        for i=0..sizeof(src)-1 {
        	src[i].gate <--> Q[i].gate++;
        	Q[i].gate++ <--> WMedium.gate++;
        }
        	
        //src[0].gate <--> Q[0].gate++;
        //Q[0].gate++ <--> WMedium.gate++;

        //src[1].gate <--> Q[1].gate++;
        //Q[1].gate++ <--> WMedium.gate++;

        //src[2].gate <--> Q[2].gate++;
       // Q[2].gate++ <--> WMedium.gate++;

        //src[3].gate <--> Q[3].gate++;
        //Q[3].gate++ <--> WMedium.gate++;

        WMedium.gate++ <--> AP.gate++;

		for i=0..sizeof(dest)-1{
		    AP.gate++ <--> dest[i].gate;
		}
        //AP.gate++ <--> dest[0].gate;
        //AP.gate++ <--> dest[1].gate;
       // AP.gate++ <--> dest[2].gate;
       // AP.gate++ <--> dest[3].gate;
}

