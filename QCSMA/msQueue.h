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

#ifndef MSQUEUE_H_
#define MSQUEUE_H_

#include "Qmodule.h"
#include "string.h"

namespace csma {

class msQueue : public Qmodule
{
protected:
    virtual simtime_t startService(cMessage *msg);
    virtual void endService(cMessage *msg);
};

}; /* namespace csma */
#endif /* MSQUEUE_H_ */
