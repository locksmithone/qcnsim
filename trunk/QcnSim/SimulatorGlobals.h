/**
 * @author Marcos Portnoi
 * @date April 2013
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

#include <string>
#include <random>

#define CURRENT_ABSOLUTE_TIME 0.0  //!< default currentAbsoluteTime
// Let the default SIMULATION_ABSOLUTE_START_TIME be equal to CURRENT_ABSOLUTE_TIME.
#define SIMULATION_ABSOLUTE_START_TIME 0.0  //!< Indicates the absolute time measurements should consider as the start of the simulation.
#define PRINT_TRACE_FLAG false //!< default printTraceFlag
#define VERSION "2013.08.02" //!< default version string

/**
 * @mainpage QCNSim - QCN Simulator
 *
 * @section introduction_section Introduction
 * Our simulation environment allows us to study the QCN cyber-infrastructure including an advanced network topology and multiple QCN servers that support
 * each other in case of network failures or overload. In particular, our environment is capable of constructing topologies in which sensors are connected
 * to multiple QCN servers via any number of links and intermediate network nodes. Modeling the links includes infinite or limited-size transmission queues
 * (buffers) and propagation delay for diverse media (e.g., copper wire, fiber optics, and air). Additionally, the environment supports dynamic behavior for
 * the topology, in which sensors can reroute their traffic and links can be shut down during a simulation.
 *
 * The simulation environment is composed of three layers and is based in part on EmBOINC for QCN and the TARVOS Computer Networks Simulator. Each of the three
 * layers implements different levels of abstraction for the QCN simulation and its components. We describe the layers in further detail in the next sections.
 *
 * @section toplayer_section Top Layer: QCN, Seismic Models, and QCN Explorer
 * The top layer integrates the scientific models of earthquake events and sensors. A generic seismology model includes the two seismic waves called P- and S-waves
 * that propagate outward from the hypocenter (the location that an earthquake starts beneath the surface of the Earth) in a spherical pattern. The speed of the
 * S-wave is generally around 3 km/s, but can vary in different regions as seismic waves travel slower through looser substrates and faster through denser ones.
 * The speed of the P-wave, or primary wave, is modeled as V_s √3, where V_s is the speed of the S-wave. The amplitude of the seismic waves, measured in m/s^2,
 * is a measurement obtained by the QCN accelerometers and is used to quantify the shaking effects at the sensor location. The amplitude of the seismic waves is
 * calculated from the attenuation relationship described in. The amplitude of the S-wave is based on the sensor’s hypocentral distance
 * (i.e., the straight-line distance within the Earth between the earthquake’s hypocenter and the sensor’s location on the surface). The amplitude of the P-wave
 * is assumed to be one fifth that of the S-wave. Because the goal of this layer is to offer high-level abstractions to augment functionalities of the underlying
 * layers and to assist the user in constructing seismic simulations, this layer also comprises configuration file parsers and the QCN Explorer user
 * interface.
 *
 * QCN Explorer has a web-based interface and provides graphical tools to the user to prepare earthquake simulation scenarios by controlling earthquake parameters
 * and placement of QCN sensors as well as drawing the network topology on a real-life geographic map. Through its earthquake simulation models, QCN Explorer
 * generates seismic events as input to the top layer file parsers.
 *
 * @section middle_section Middle Layer: Computer Network Components
 * The middle layer provides the QCN cyber-infrastructure with the high-level abstractions for computer network components, such as network links, Protocol Data
 * Units (PDUs), nodes, routing, link failures, and traffic generators. The layer also includes components to measure performance, calculate statistics, and
 * generate traces. Classes in this layer utilize kernel classes from the lower layer to perform their duties. The main classes are Node, Link, ProtocolDataUnit,
 * and Route.
 *
 * In the QCN cyber-infrastructure, nodes implemented in the Node class are either BOINC clients connected to low cost sensors, or intermediary entities (such
 * as intermediate network nodes and links). Nodes receive trickle messages from sensors or links, forward them to other nodes through links, take decisions on
 * paths and routes, and discard trickle messages. Nodes also collect statistics such as delay, jitter, and the number of received, forwarded, discarded trickle
 * message and bytes, behaving much like network routers. The Link class implements connections between BOINC clients and intra-topology nodes,  intra-topology
 * nodes and a QCN server, or a BOINC client and a QCN server. More specifically, links connects two QCN nodes in one way (i.e., simplex links). In more complex
 * frameworks than QCN, this class also supports two-way connections (i.e., duplex links that are de facto two simplex links). Links provide service by transporting
 * trickle messages from one end of the link to the other end (e.g., from a BOINC client to a QCN intra-topology node or between two intra-topology node). This is
 * done in two steps. First, upon being requested, the link first transmits the trickle messages at full bandwidth speed; the time to transmit a message is a
 * relationship between message length and bandwidth speed. If transmission is successful, the message is considered within the link’s medium (e.g., the wire or
 * airborne for a wireless link). Second, the message propagates through the medium until reaching the QCN server. This propagation is simulated as a simple fixed
 * delay defined by the user based on QCN traces. Consequently, a network link is modeled as a resource with one QCN server and a priority queue that is chained to
 * the infinite-capacity QCN server. The ProtocolDataUnit class models a network PDU representing a QCN trickle message. In particular, this class associates
 * Time-To-Live (TTL) and length members to PDUs. The Route class models routes within a QCN network of intra-nodes, to be followed by PDUs.
 *
 * @section bottom_section Bottom Layer: Discrete Event-based Queuing System Simulator
 * The bottom layer is a general-purpose, discrete event-driven queuing system simulator. This layer provides the classes used to implement the higher layer
 * components that allow the construction of the advanced network topologies proposed for QCN in this paper. The layer is composed by a kernel including elements,
 * such as resources that provide service, priority and FIFO queues for the resources, functions for event manipulation, and statistics collection and traffic
 * generators based on probability distributions.
 *
 * The kernel’s view is a queuing system with three main components: resources, tokens (Token class), and events (Event class). Resources and tokens are part of a parent class in the simulator
 * called Entity. These resources model real-life entities, such as routers, links, web servers, and processors that basically deliver some sort of service.
 * Resources have a series of related functions or methods (e.g., definition, reservation, release, preemption, and fail). The simulator defines a resource object
 * by a reference and the number of processing units within that resource. Processing units work independently from the other units within the same resource, and
 * they share a single priority queue. The number of processing units may model, for example, cores within a processor, allowing for modeling threads or parallel
 * processing of clients or web requests. When service is requested, a reservation is submitted for a resource to perform the service. If the resource has at least
 * one free processing unit, then service is granted and that processing unit becomes busy. When the service is finished, that unit is released and becomes free.
 * If a resource has no free processing units, then the resource is busy; otherwise, the resource is free if at least one of its processing units is free.
 * In addition, the kernel offers capabilities for making a resource temporarily non-operational. This ability can be used to model failures, such as a failed link
 * in the QCN network or an unavailable QCN server.
 *
 * The tokens (Token class) represent entities in the system (e.g., customers, tasks, Protocol Data Units or PDUs, and messages). The flow of tokens through the collection of
 * resources models the dynamic behavior of a system; in particular, this flow models the QCN cyber-infrastructure. A token class depicts the several data structures
 * and functions associated with tokens, including an associated object that can be used to carry trickle messages across resources. Resources, therefore, perform
 * service on the tokens. In a queuing system simulation context, a service is merely a delay; real processing of the token or associated data is not generally done.
 * However, upper simulator layers, such as the Top Layer described earlier, do implement classes to perform processing in the associated token data (e.g., to obtain
 * seismic analysis).
 *
 * Finally, an event (Event class) defines any change of state in the system. Changes of state include, for instance, the generation of a new token, a service request for a token,
 * a resource release, dequeuing of a token, and a subsequent request for service. Events have an occurrence time, the type of event, and an associated entity object
 * that is used by the simulator to manipulate the event.
 * A class called Scheduler is the main kernel class that coordinates interaction between the simulator components and controls the simulation clock. In contrast
 * with a real-time application or an emulator, the simulator works under simulated time, by simulating the passage of real time in a generally faster fashion.
 * In our event-driven simulator, this is achieved by generating events that occur in specific times in the future and putting these events into the Event Chain data
 * structure into the Scheduler class. Event Chain is a time-ordered list of events, and its basic data structure is described in the EventChainElement class.
 * The Scheduler class is the entity responsible for putting events into the list, ordered by occurrence in time,
 * and retrieving events from the beginning of the list. When the Scheduler retrieves an event at the head of the list, it causes the event; this activates the
 * agents responsible for executing the actions predicted in the event. For instance, an arrival event, which can be described as a token “arriving” at a specified
 * destination, can trigger a request-for-service event. To start properly, a simulation requires at least one initial event in the Event Chain. This initial event
 * schedules other events generating a chain, and the simulation proceeds by adding the events in the Event Chain. If there are no more events in the Event Chain,
 * then the simulation terminates. The simulation can also stop due to other conditions (e.g., when the simulation reaches the targeted simulation time).
 *
 * @section implementation_section Simulation Implementation
 * The simulation environment is coded in C++11, the version of C++ approved by ISO on August 12, 2011. This C++ standard offers modern coding resources, namely
 * controlled type inference; range-based loops resembling Java’s and Python’s capabilities; constructor delegation; constructor initialization lists; abstract
 * classes; robust general-purpose smart pointers. allowing the compiler to deal with heap memory management and thus avoid dangerous memory-leak situations; and
 * extensible random number generators, including several probability distributions, such as Poisson, exponential, and normal. By using this C++ standard, the
 * programmer has stronger support to generate streamlined code and, in particular, can allocate more effort on the purposes of the code and less on language-specific
 * issues like heap memory management. The coding employs the test-driven development technique. As benefits from this method, the test cases employed constitute
 * a valuable piece of documentation, and development can progress in a solid pace as larger components of the code are assured to rely on smaller tested and proven
 * components. We utilized solely standard C++11 libraries in the implementation of our tool, and no platform-specific customization. This way, the only basic 
 * requirement for running the tool in any computing platform is a C++11-compatible compiler.
 *
 *
 * @section changelog_section Changelog
 *
 * <b>20140102</b>: Complete unit tests and results. Suggestion: implement Topology class to accrue data structures related to network topology (vectors of nodes,
 *           vector of links, functions to find a link given two nodes, etc. See QcnSimCCGrid.cpp for examples.
 *
 * <b>20131121</b>: Tagging version utilized to generate CCGrid paper results.
 *
 * <b>20130828</b>: Ready to generate results for example scenarios. To do: Link class, seismic models. XML parameter reader. Trace/log functions.
 *
 * <b>20130809</b>: Tagging revision 337, 20130809 for demo. Most kernel functionality implemented. Simulating topology in which client traffic
 *           is rerouted to backup server mid-simulation.
 *
 * 
 *
 */

/**
 * @brief Simulator-scope variables and parameters.
 * 
 * @par Description
 * Simulator-scope variables and parameters will be here.
 * Most important one is currentAbsoluteTime, which indicates the current, clock time of the simulation.
 * It also includes resources to automatically generate unique token IDs and maintain a simulator-unique
 * random number generator engine and seed.
 * 
 * Changelog:
 * 
 *
 */
class SimulatorGlobals {
private:
	double currentAbsoluteTime; //!< Current simulation time.
	double simulationAbsoluteStartTime; //!< Indicates the absolute time measurements should consider as the start of the simulation. This allows a simulation to have a "warm-up" time, which statistics will disregard.  To use, just set this variable to the time after which statistics should be calculated, e.g., if set to 5.0, busy times and utilization measurements before 5.0 will be disregarded (by doing the subtraction currentAbsoluteTime - measurementsAbsoluteStartTime).
	bool printTraceFlag; //!< TRUE:  prints tracing information; FALSE:  does not print tracing information during simulation.
	std::string version; //!< Simulator current version.
	//std::random_device randomDevice; //!< Random device for random generators. // Cannot be done here... move it to cpp implementation.
	std::default_random_engine randomEngine; //!< The Random Engine to be used by traffic generators.
	unsigned int seed; //!< The actual seed to use (or being used) for the random number generator.
	unsigned int tokenInitialId; //!< Initial ID for tokens generated in this simulator. To assure unique IDs, use the function getTokenNextId().
	
	void initializeRandomGeneratorRandomSeed();

public:
	SimulatorGlobals();
	SimulatorGlobals(double currentAbsoluteTime, double simulationAbsoluteStartTime, bool printTraceFlag, std::string version);
	SimulatorGlobals(double currentAbsoluteTime, bool printTraceFlag, std::string version);

	double getCurrentAbsoluteTime() const;
	double getSimulationDuration() const;
	bool isTraceOn() const;
	std::string getVersion() const;
	unsigned int getRandomNumberGeneratorSeed() const;
	void seedRandomNumberGenerator(unsigned int seed);
	void setSimulationAbsoluteStartTime(double simulationAbsoluteStartTime);
	void setCurrentAbsoluteTime(double currentAbsoluteTime);
	unsigned int getTokenNextId();
	std::default_random_engine &getRandomNumberGeneratorEngineInstance();

	/// @todo Tracing is not yet implemented. Each class must implement its own trace routines.
};
