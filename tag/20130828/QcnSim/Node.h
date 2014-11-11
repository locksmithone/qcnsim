/**
 * @author Marcos Portnoi
 * @date August 2013
 * 
 * @copyright Copyright (C) 2013 University of Delaware.
 * @copyright QCNSim uses elements of TARVOS simulator, Copyright (C) 2005, 2006, 2007 Marcos Portnoi.
 * @par
 * This file is part of QCNSim.  QCNSim is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.<br>
 * QCNSim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.<br>
 * You should have received a copy of the GNU Lesser General Public License
 * along with QCNSim.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "Entity.h"
#include "Token.h"
#include "ProtocolDataUnit.h"
#include "NodeReturnType.h"
#include <memory>
#include <iostream>

/**
 * @brief Node class.
 * 
 * @par Description
 * The Node class describes several statistics for counting PDUs and bytes received, for instance, and to measure jitter and delay for PDUs.
 * Typically, a node measures statistics for network layer (network PDU, therefore).
 * Entities can be "attached" to nodes: these entities are typically traffic generators (sources), links, and application servers.
 *
 * @par Functionalities
 * This simulator works by abstracting a computer network into traffic generators, nodes, links, and possibly sinks and application servers.
 * A traffic generator is attached to a node, which servers as the source of the PDU/token. The generated PDU/token will have a destination
 * node, and this node will typically be the sink for the PDU. An application server may be connected to a node. This application server Entity
 * may receive the contents of the PDU/token and process it, generating some result or more PDUs/tokens.
 * A node with a sink simply means that node is the end destination of the PDU/token, and the PDU/token will be dropped upon receival, after
 * proper statistics updated.
 * Links will be attached to nodes, and each link connects two nodes. There can be many links attached to a single node.
 * The node will perform PDU/token forwarding based on some routing rule. Forwarding consists of consulting a routing table or rule upon receival
 * of a PDU/token, if that node is not the final destination of the PDU/token. The routing table or rule will define the next link (attached to the
 * current node) through which the PDU/token will be forwarded. Note that this "next link" is similar to the "next hop" information on typical
 * routing tables.
 * "Next link," "attached application server," "traffic generators" are all Entity objects.
 */
class Node: public Entity, public std::enable_shared_from_this<Node> {
private:
	unsigned int receivedBytesCount; //!< Count of received bytes by this node.
	unsigned int receivedPdusOrTokensCount; //!< Count of received PDUs by this node (packets for network PDUs, for example).
	unsigned int forwardedPdusOrTokensCount; //!< Count of forwarded PDUs from this node to next Entity.
	unsigned int forwardedBytesCount; //!< Count of forwarded bytes from this node to next Entity.
	unsigned int droppedPdusOrTokensCount; //!< Count of dropped PDUs by this node.
	double lastDelay; //!< Delay measured for last PDU received by this node.
	double sumDelay; //!< Sum of delays measured for all PDUs received by this node.
	double meanDelay; //!< Mean delay as measured for all PDUs received by this node.
	double lastJitter; //!< Jitter measured between the last two PDUs received by this node.
	double sumJitter; //!< Sum of jitters measured for all PDUs received by this node.
	double meanJitter; //!< Mean jitter as measured for all PDUs received by this node.
	double previousDelay; //!< Delay measured for previous PDU (the PDU before the current received one) received by this node. Necessary for jitter calculation.
	SimulatorGlobals &simulatorGlobals;  //!< Reference to SimulatorGlobals object, to get clock time.

	void updateArrivalStatistics(std::shared_ptr<Token> token);
	void updateForwardingStatistics(std::shared_ptr<Token> token);
	void updateArrivalStatistics(std::shared_ptr<ProtocolDataUnit> pdu);
	void updateForwardingStatistics(std::shared_ptr<ProtocolDataUnit> pdu);
	NodeReturnType updateForwardHops(std::shared_ptr<Token> token);

public:
	Node(SimulatorGlobals &simulatorGlobals);

	unsigned int getReceivedBytesCount() const;
	unsigned int getReceivedPdusOrTokensCount() const;
	unsigned int getForwardedPdusOrTokensCount() const;
	unsigned int getForwardedBytesCount() const;
	unsigned int getDroppedPdusOrTokensCount() const;
	double getLastPduOrTokenDelay() const;
	double getSumPduOrTokenDelay() const;
	double getMeanPduOrTokenDelay() const;
	double getLastPduOrTokenJitter() const;
	double getSumPduOrTokenJitter() const;
	double getMeanPduOrTokenJitter() const;

	NodeReturnType processAndForward(std::shared_ptr<Token> token);
	NodeReturnType processAndForward(std::shared_ptr<ProtocolDataUnit> pdu);

};
