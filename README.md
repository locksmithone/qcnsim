qcnsim
======

QCNSim: A simulator for the Quake-Catcher Network

Summary
This project consists of building a new version of a simulator for the Quake-Catcher Network and the underlying BOINC instance of volunteer computing. The simulator aims at enabling investigation of different configurations of the BOINC network and network topologies, such as the existence of multiple BOINC servers and clustering of QCN-hosts. In addition, the new simulator will allow exploring robustness in face of network failures, and formulation of possible approaches to mitigate issues arising from these failures.

 The general characteristic of the simulator may also allow adding new earthquake models and other natural models, such as hurricanes.

The Quake-Catcher Network (QCN)
The QCN network (http://qcn.stanford.edu/) consists of volunteer computers (identified, in this document, as “hosts” or BOINC “clients”) connected to the BOINC network of volunteer computing projects. Each host has an “earthquake” sensor, which is, in essence, a low-cost device with three accelerometers and a compass, connected to the host computer via USB port. The hosts register to the QCN network with their current geographical position.

 The hosts maintain sporadic connection with the appropriate BOINC server. In the presence of relevant signals from the accelerometer sensor, the hosts sends frequent messages to the BOINC server with its measurements and other data. By receiving potentially several hundreds of measurements from sensors spread over an area, the BOINC server can analyze and identify the macroscopic earthquake phenomena. By its nature, when an earthquake on land is detected, there are not enough seconds to provide an effective alarm or take precautionary measures to safeguard populations. Still, there might be ways to safeguard equipment, turn off computers or divert networks, in which the QCN can become a key part.

The BOINC project
BOINC (http://boinc.berkeley.edu/) is a framework for volunteer computing projects. It offers the necessary software for connecting hosts to the project’s servers and the scheduling of grid applications.

 Typically, a project is created by allocating a computer server that runs the BOINC server software and the appropriate project’s grid software. Hosts will run the BOINC client. Upon registration in the project, a client host will download the project’s software, responsible for doing the scientific calculations, and work units. The software will process the work units (normally during idle times of the host computer) and send the results back to the project’s server.

Particularities for QCN
In QCN, the host computer performs no calculation. The host software essentially monitors the accelerometer sensor and sends messages (called “trickle messages”) back to the QCN server when relevant data is present. Dummy work units are downloaded from the server periodically to maintain status, but they require no calculation or results.

Previous work
GCLab has previously constructed a simulator/emulator for the QCN network, named EmBOINC for QCN. EmBOINC is a general-purpose emulator/simulator for BOINC projects, capable of simulating hosts and including an emulator of a BOINC server. This emulator is the actual BOINC server code embedded into the simulator, and it is accessible through network protocols from the simulated BOINC clients. EmBOINC for QCN is a customized version, in which the simulated hosts properly mimic BOINC clients equipped with accelerometer sensors, and include an earthquake simulation module to feed signals to the simulated hosts. The emulated BOINC server code is also modified such that it runs in simulated time, jumping from one event to another in time.

Comments
The presence of the BOINC server emulator allows for precise simulation of task scheduling and service time duration, and simulation of the communication between hosts and server. However, task scheduling, basically, is non-existent for the QCN project, since no scientific processing is done in the work units downloaded by the clients. Moreover, in EmBOINC for QCN, communication between the clients and the emulated server is done in a high level way, e.g., via direct calling of PHP commands. Therefore, network transmission protocols are not necessarily used, the channel is always reliable (although there are failure models targeted at the BOINC clients), and, hence, the impact of transmission protocols is not translated into the simulation results.

 Furthermore, the presence of the emulator requires presence of several additional software, such as a database manager and webserver, and limits the number of different architectures in which EmBOINC for QCN can run. The construction of EmBOINC with one server does not make it possible to investigate solutions in which more than one BOINC server is available, and clients could self-organize and cluster to diverse servers.

Installation and use of EmBOINC for QCN
EmBOINC for QCN operates in Linux environment, and requires MySQL, PHP, Apache, among other packages. Installation is not trivial, and compilation is dependent on setting specific compiler flags that propagate to the code, resulting in the code being modified at compile time according to the values of the flags.

Details on the new simulator, QCNSim

Purpose
The new simulator, named QCNSim, intends to address the aforementioned issues by stripping off the BOINC server code from the simulator and substituting it for a simulated model. This will offer more flexibility in creating more than one BOINC server and exploring diverse topologies. Additionally, the absence of the BOINC server code will facilitate installation and decrease requirements and dependencies, making it also possible to compile the simulator in architectures other than Linux.

Questions we want to answer using the simulator
How can we mitigate network failures, or BOINC server failures? 
Can we apply network partitioning and/or self-organization of clients for robustness? 
How can the network resist anomalies, such as botnet attacks using emulated (fake) sensors? 
Are there realistic alarm tactics we can deploy? 
How is the behavior of the network with other models, such as hurricanes?

Construction
The simulator, coded in C++, will be composed of three layers:
Discrete, event-based queuing system simulator; 
Computer networks components; 
QCN models.

 Layer 1 is a general-purpose, queuing system simulator, composed of facilities, tokens, and an event chain. Layer 2 builds on top of of layer 1 and provides computer network components, such as links, messages and traffic generators. It is based on my previous Masters work and my TARVOS computer networks simulator (Portnoi, M. & Martins, J. S. B. TARVOS - an Event-Based Simulator for Performance Analysis, Supporting MPLS, RSVP-TE, and Fast Recovery. XIII Brazilian Symposium on Multimedia and the Web - Webmedia 2007, XIII Brazilian Symposium on Multimedia and the Web - Webmedia 2007, 2007, 1, 222-229).

 Layer 3 will implement earthquake models and the BOINC topology.

 The code aims to allow compilation from any compiler without customizations or hand-written makefiles.

Related publication:
Portnoi, M.; Schlachter, S.; Taufer, M. Study of the Network Impact on Earthquake Early Warning in the Quake-Catcher Network Project. To appear in the International Conference on Computational Science (ICCS) 2014, 2014.
Abstract: The Quake-Catcher Network (QCN) project uses the low cost sensors in accelerometers attached to volunteers' computers to detect earthquakes. The master-worker topology currently used in QCN and other similar projects su.ers from major weaknesses. The centralized master can fail to collect data if the volunteers' computers are not connected to the network, or it can introduce signi.cant delays in the warning if the network is congested. We propose to solve these problems by using multiple servers in a more advanced network topology than the simple master-worker con.guration. We .rst consider several critical scenarios in which the current master-worker con.guration of QCN can hinder the early warning of an earthquake, and then integrate the advanced network topology around multiple servers and emulate these critical scenarios in a simulation environment to quantify the bene.ts and costs of our proposed solution. By using metrics of interest that have a clear scienti.c meaning for the scope of the QCN project, we show how our solution can reduce the time to detect an earthquake from 1.8 s to 173 ms in case of network congestion and the number of lost trickle messages from 2,013 to 391 messages in case of network failure.
 The code is available at https://qcnsim.codeplex.com (and a fork at https://github.com/TauferLab/QCN-sim).
