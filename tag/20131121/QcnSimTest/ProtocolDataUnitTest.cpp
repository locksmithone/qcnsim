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

#include "ProtocolDataUnitTest.h"

/**
 * Constructor.
 *
 * Do initializations here.
 */
ProtocolDataUnitTest::ProtocolDataUnitTest(): contents(new Message("This is PDU contents.")),
		simulatorGlobals(SimulatorGlobals(0.0, 0.0, false, "PDUTest")) {
}


/// Tests routing functions in PDU.
TEST_F(ProtocolDataUnitTest, Routing) {
	// Build 5 nodes. Will test routing between them using explicit route attached to pdus. No need for links or facilities here.
	//std::shared_ptr<Message> dummyContents(new Message("This is a dummy ProtocolDataUnit contents."));
	std::vector<std::shared_ptr<Node>> nodeVector;
	std::vector<std::shared_ptr<Entity>> explicitRoute;
	for (int i = 0; i < 5; ++i) {
		nodeVector.push_back(std::shared_ptr<Node>(new Node(simulatorGlobals)));
	}
	// Push each node in a certain order to the explicitRoute vector. Let's have the first one as the source, the last one as the destination.
	explicitRoute.push_back(nodeVector.at(0));
	explicitRoute.push_back(nodeVector.at(1));
	explicitRoute.push_back(nodeVector.at(2));
	explicitRoute.push_back(nodeVector.at(3));
	explicitRoute.push_back(nodeVector.at(4));

	// Create one explicit Route.
	//Route route(explicitRoute, true);
	//EXPECT_EQ(explicitRoute, route.getExplicitRoute());

	// Let's test the basic functions for setting and getting explicit route. PDU size will be 1000 bytes.
	std::shared_ptr<ProtocolDataUnit> pdu(new ProtocolDataUnit(simulatorGlobals, 1, nullptr, nodeVector.at(0), nodeVector.at(4), 1000));
	// Get initial TTL
	unsigned int ttl = pdu->getTtl();
	EXPECT_EQ(128, ttl);
	EXPECT_EQ(pdu->source, nodeVector.at(0));
	EXPECT_EQ(pdu->destination, nodeVector.at(4));
	EXPECT_EQ(1000, pdu->getPduSize());
	EXPECT_FALSE(pdu->previous); // That's the way to test for nullptr smart pointers.
	EXPECT_FALSE(pdu->next);
	// Attach route vector (NOT route object) to pdu's Route object and set it to record route.
	pdu->setExplicitRoute(explicitRoute);
	pdu->setRecordThisRoute();
	EXPECT_EQ(explicitRoute, pdu->getExplicitRoute());
	// Give the pdu to the source node and let it forward.
	nodeVector.at(0)->processAndForward(pdu);
	// Is the first element of recorded route really the source?
	EXPECT_EQ(nodeVector.at(0), pdu->getRecordedRoute().front());
	// Was TTL decremented?
	EXPECT_EQ(ttl - 1, pdu->getTtl());
	// Set the explicit route again, check whether the recorded route was erased.
	pdu->setExplicitRoute(explicitRoute);
	EXPECT_EQ(0, pdu->getRecordedRoute().size());
	// Se the record route to false; route should not be recorded.
	pdu->setDoNotRecordThisRoute();
	// Let's push hops into the recordedRoute.
	pdu->addHopToRecordedRoute(nodeVector.at(0));
	pdu->addHopToRecordedRoute(nodeVector.at(1));
	pdu->addHopToRecordedRoute(nodeVector.at(2));
	pdu->addHopToRecordedRoute(nodeVector.at(3));
	pdu->addHopToRecordedRoute(nodeVector.at(4));
	// Recorded route should be empty.
	EXPECT_TRUE(pdu->getRecordedRoute().empty());
	// Now enable route recording and repeat.
	pdu->setRecordThisRoute();
	// Let's push hops into the recordedRoute.
	pdu->addHopToRecordedRoute(nodeVector.at(0));
	pdu->addHopToRecordedRoute(nodeVector.at(1));
	pdu->addHopToRecordedRoute(nodeVector.at(2));
	pdu->addHopToRecordedRoute(nodeVector.at(3));
	pdu->addHopToRecordedRoute(nodeVector.at(4));
	// That was basically explicitRoute vector. Verify.
	EXPECT_EQ(explicitRoute, pdu->getRecordedRoute());
	// Now manually furnish nodes to pdu and allow it to update its previous/next members by calling the forward function.
	// Route is Node 0, 1, 2, 3, 4. Source is 0, destination is 4.
	// Begin with source node. Since the next hop in the list is node 0, previous will be zero, next will be zero. The update function within PDU does not treat this.
	pdu->updateHopsFromExplicitRoute(nodeVector.at(0));
	EXPECT_EQ(nodeVector.at(0), pdu->previous);
	EXPECT_EQ(nodeVector.at(0), pdu->next); 
	// Observe that while the pdu is still "at" current node 0, the next one will be 1 and the previous is 0 after updating forward members. PDU is "about" to exit node 0.
	// When the pdu reaches node 1, the current node is 1, the previous node is 0, and the next node is 1 (before updating forward members).
	// Now "forward" to node 1, the next hop.
	pdu->updateHopsFromExplicitRoute(nodeVector.at(0));
	EXPECT_EQ(nodeVector.at(0), pdu->previous);
	EXPECT_EQ(nodeVector.at(1), pdu->next);
	// Now "forward" to node 2, the next hop.
	pdu->updateHopsFromExplicitRoute(nodeVector.at(1));
	EXPECT_EQ(nodeVector.at(1), pdu->previous);
	EXPECT_EQ(nodeVector.at(2), pdu->next);
	// Now "forward" to node 3, the next hop.
	pdu->updateHopsFromExplicitRoute(nodeVector.at(2));
	EXPECT_EQ(nodeVector.at(2), pdu->previous);
	EXPECT_EQ(nodeVector.at(3), pdu->next);
	// Now "forward" to node 4, the next hop.
	pdu->updateHopsFromExplicitRoute(nodeVector.at(3));
	EXPECT_EQ(nodeVector.at(3), pdu->previous);
	EXPECT_EQ(nodeVector.at(4), pdu->next);
	// Now attempt forwarding past node 4. The pdu should get stuck at node 4, since the pdu has no intelligence to consider itself reaching a destination. Attempt this three times.
	pdu->updateHopsFromExplicitRoute(nodeVector.at(4));
	EXPECT_EQ(nodeVector.at(4), pdu->previous);
	EXPECT_EQ(nodeVector.at(4), pdu->next);
	pdu->updateHopsFromExplicitRoute(nodeVector.at(4));
	EXPECT_EQ(nodeVector.at(4), pdu->previous);
	EXPECT_EQ(nodeVector.at(4), pdu->next);
	pdu->updateHopsFromExplicitRoute(nodeVector.at(4));
	EXPECT_EQ(nodeVector.at(4), pdu->previous);
	EXPECT_EQ(nodeVector.at(4), pdu->next);
	
	// Let's attach no route.
	pdu->setExplicitRoute(std::vector<std::shared_ptr<Entity>>());
	EXPECT_TRUE(pdu->getExplicitRoute().empty());
	EXPECT_TRUE(pdu->getRecordedRoute().empty());
	// Now if we attempt to have a node forward the pdu, it should not work.
	pdu->updateHopsFromExplicitRoute(nodeVector.at(0));
	EXPECT_EQ(nodeVector.at(0), pdu->previous);
	EXPECT_FALSE(pdu->next); // This should be null.
	// Try again. Same outcome.
	pdu->updateHopsFromExplicitRoute(nodeVector.at(0));
	EXPECT_EQ(nodeVector.at(0), pdu->previous);
	EXPECT_FALSE(pdu->next); // This should be null.
	// Have another node forward the pdu. Only the previous field should be modified.
	pdu->updateHopsFromExplicitRoute(nodeVector.at(3));
	EXPECT_EQ(nodeVector.at(3), pdu->previous);
	EXPECT_FALSE(pdu->next); // This should be null.

	
}
