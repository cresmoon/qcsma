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

simple AccessPoint
{
    parameters:
    	@display("i=block/queue;q=queue");
        //@signal[qlen](type="int");
        //@signal[busy](type="bool");
        //@signal[queueingTime](type="simtime_t");
       	//@statistic[qlen](title="queue length";record=vector,timeavg,max;interpolationmode=sample-hold);
        //@statistic[busy](title="server busy state";record=vector?,timeavg;interpolationmode=sample-hold);
        //@statistic[queueingTime](title="queueing time at dequeue";unit=s;record=vector,mean,max;interpolationmode=none);
    gates:
        inout gate[];
}