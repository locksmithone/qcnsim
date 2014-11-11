/**
 * @author Marcos Portnoi
 * @date May 2013
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

#include "FacilityServer.h"
#include "FacilityQueueElement.h"
#include "Token.h"
#include "EventType.h"
#include "FacilityReturnType.h"
#include "SimulatorGlobals.h"
#include "Scheduler.h"
#include <vector>
#include <list>
#include <string>
#include <memory>

using std::string;

/**
 * @brief Facility class.
 * 
 * @par Description
 * The Facility class is used to model a resource or service center, i.e., something from which a process requests service.
 * A facility consists of one or more servers and one priority queue, in which processes await access to the next available server. Only one process at a time can use a server.
 * In the priority queue, processes are ordered from highest priority to lowest priority (default is zero).
 */
class Facility: public Entity {
private:
	std::string name;					//!< Facility name.	
	//unsigned int id;					//!< Facility ID.
	//int numberOfServers;				//!< Number of servers in this facility.
	//int numberOfBusyServers;			//!< Number of busy servers.
	//int queueSize;					//!< Current queue size (in number of processes). The queue is infinite.
	unsigned int maxRecordedQueueSize;	//!< Maximum recorded queue size up to this moment.
	unsigned int queueSizeLimit;		//!< The maximum size the queue can reach (default is std::numeric_limits<unsigned int>::max()); if reached, all requests for service will be dropped until queue decreases. Preemptions will be dropped if going into queue (see preempt function).
	unsigned int dequeuedTokensCount;	//!< Count of dequeues up to this moment.
	double lastQueueChangeTime;			//!< Absolute time of last queue change.
	double sumBusyTime;					//!< Total (relative, elapsed) time this facility was busy. Sum (integral) of busy time of all this facility's servers.
	unsigned int preemptedTokensCount;	//!< Count of effective preemptions.
	double sumLengthTimeProduct;		//!< += Current queue size * (currentAbsoluteTime - absolute time of last queue change); used to obtain average queue size.
	unsigned int releasedTokensCount;	//!< Count of processes which were served (released) by facility.
	std::vector<FacilityServer> servers; //!< List of servers in this facility.
	std::list<FacilityQueueElement> queue; //!< Priority queue of tokens in this facility.
	bool isUp_;							//!< Status of facility: true for up (operational), false for down (non-operational).
	unsigned int droppedTokensCount;	//!< Count of discarded or dropped tokens in this facility. When facility enters down state, all tokens in queue are discarded, and this counter
										//!< is updated.
										//!< If a token is scheduled for service in a facility, and this facility is down, the token is considered dropped and this counter is also incremented.
	SimulatorGlobals &simulatorGlobals; //!< Reference to SimulatorGlobals object (typically used to fetch the current clock time).
	Scheduler &scheduler;				//!< Reference to Scheduler object, such that dequeued tokens can be rescheduled.
	unsigned int requestsPreemptsCount;	//!< Count of number of service requests and preemptions made to this facility (regardless of the outcome). Notice that a token might have more than one request, e.g., if the first request finds a busy facility, token is enqueued. Dequeuing will generated another service request.

private:
	void enqueue(std::shared_ptr<const Token> token, EventType eventType, double serviceTime, bool isPreempted);
	bool dequeue();
	unsigned int purgeQueue(); //!< Purge queue, return number of dropped elements.
	std::vector<FacilityServer>::iterator nextFreeServer();
	std::vector<FacilityServer>::const_iterator nextFreeServer() const;
	void initializeMembers();
	unsigned int dropTokensInService();

public:
	Facility(const std::string &name, unsigned int numberOfServers, SimulatorGlobals &simulatorGlobals, Scheduler &scheduler);
	Facility(const std::string &name, SimulatorGlobals &simulatorGlobals, Scheduler &scheduler);

	/// @todo Dequeued tokens will be placed at front of event chain, with corresponding event, such that service time can be calculated real time... this is why we need EventType here.
	FacilityReturnType request(std::shared_ptr<const Token> token, EventType eventType);
	FacilityReturnType preempt(std::shared_ptr<const Token> token, EventType eventType);
	FacilityReturnType release(std::shared_ptr<const Token> token);
	bool isBusy() const;
	bool isUp() const;
	void setUp();
	unsigned int setDown();
	std::list<FacilityQueueElement>::size_type getQueueSize() const;
	double getUtilization() const;
	double getMeanBusyPeriod() const; //!< See Jain, MacDougall.
	double getMeanBusyPeriod(bool onlyFullyServiced) const; //!< See Jain, MacDougall.
	double getMeanQueueLength() const; //!< See Jain, MacDougall.
	std::string getName() const;
	unsigned int getMaxRecordedQueueSize() const;
	unsigned int getDequeuedTokensCount() const;
	double getLastQueueChangeTime() const;
	double getSumBusyTime() const;
	unsigned int getPreemptedTokensCount() const;
	double getSumLengthTimeProduct() const;
	unsigned int getReleasedTokensCount() const;
	unsigned int getFullyServicedTokensCount() const;
	unsigned int getDroppedTokensCount() const;
	double getMeanServiceRate() const;
	void setName(const std::string &name);
	unsigned int getNumberOfServers() const;
	unsigned int getRequestsPreemptsCount() const;
	void setQueueSizeLimit(unsigned int limit);
	unsigned int getQueueSizeLimit() const;
};