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

#include "Facility.h"

/**
 * @brief Constructor with parameters.
 * 
 * @param name Name of Facility.
 * @param numberOfServers Number of servers in this facility.
 * @param simulatorGlobals SimulatorGlobals reference, typically used to get and update global values such as current clock time.
 * @param scheduler Scheduler object reference, typically to allow for scheduling service a dequeued token.
 */
Facility::Facility(const std::string &name, unsigned int numberOfServers, SimulatorGlobals &simulatorGlobals, Scheduler &scheduler): name(name), simulatorGlobals(simulatorGlobals), scheduler(scheduler) {
	this->initializeMembers();
	for (unsigned int i = 0; i < numberOfServers; ++i) {
		servers.push_back(FacilityServer());
	}
}

/**
 * Constructor with name parameter.
 *
 * @details 
 * Number of servers default to 1.
 * 
 * @param name Name of Facility.
 * @param simulatorGlobals SimulatorGlobals reference, typically used to get and update global values such as current clock time.
 * @param scheduler Scheduler object reference, typically to allow for scheduling service a dequeued token.
 */
Facility::Facility(const std::string &name, SimulatorGlobals &simulatorGlobals, Scheduler &scheduler): name(name), simulatorGlobals(simulatorGlobals), scheduler(scheduler) {
	this->initializeMembers();
	servers.push_back(FacilityServer());
}

/**
 * @brief Initialize all facilities' member variables.
 */
void Facility::initializeMembers() {
	maxRecordedQueueSize = 0;
	dequeuedTokensCount = 0;
	lastQueueChangeTime = 0.0;
	sumBusyTime = 0.0;
	preemptedTokensCount = 0;
	sumLengthTimeProduct = 0.0;
	releasedTokensCount = 0;
	isUp_ = true;
	droppedTokensCount = 0;
	requestsPreemptsCount = 0;
	queueSizeLimit = std::numeric_limits<unsigned int>::max();
}

/**
 * Put tokens in queue, in order of highest (numeric) priority.
 *
 * @details 
 * Tokens with same priority will go into the queue in FIFO manner (after other tokens of same priority).
 * See dequeue for explanation on the Event Type.
 *
 * There are two approaches that can be used when requesting service and enqueueing tokens:
 * (1) In TARVOS's (http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.122.4620) approach, a request for service for a token
 *     scheduled with the service time and next event to be scheduled once the service is finished. If the facility is busy, the token
 *     is enqueued with the service time and next event. Upon dequeuing, all the information needed to put the token into service is
 *     present in the queue:  the service time and the next event.  The token is put into service immediately, bypassing the scheduler, 
 *     and the next event is scheduled (typically a release event).
 *     A disadvantage of this approach is that dynamicity is lost for service times. Once the service time is recorded with the token when
 *     enqueueing, it will not change regardless of the time at which the token will be actually put into service. E.g., a facility might
 *     have dynamic performance:  when the service was requested, the facility was operating at a certain mean service time s. The token
 *     was enqueued (since the facility was busy), and when the token is effectively put into service, the facility might be operating at a 
 *     faster mean service time 2s. Since the service time was recorded and frozen the token was enqueued, the token will not benefit from
 *     a faster facility when it is actually serviced.
 * (2) In SMPL approach, the token is enqueued with the current event with which it was scheduled. Upon dequeuing, this event is rescheduled
 *     for the token and put at the front of the event chain (a 0.0 time scheduling), such that it is the first event to occur next.
 *     The advantage of this approach is that now service will be requested for the token with whatever current mean service time, and even
 *     the treatment of the event might have changed during the period the token was in the queue. This approach is thus more dynamic. However,
 *     attention must be taken when calculating certain statistics, such as arrivals in a facility. If an arrival is counted when service is
 *     requested, then an enqueued token will result in two arrivals counted (one when it is first requested, and a second when the event is
 *     re-scheduled upon dequeuing).
 *
 * @param token Token to be enqueued.
 * @param eventType Event type associated with this token.  Necessary for re-scheduling token upon dequeuing.
 * @param serviceTime Service time of this token.  It can be the remaining service time if the token
 *				      was in service and was preempted (thus sent back to queue). For blocked tokens
 *					  (i.e., tokens to which service did not yet begin), use serviceTime = 0.0.
 * @param isPreempted True if this token to be enqueued was preempted; false if it was blocked. Based on this flag,
 *					  this function will decide at which position to place the token in the queue:
 *					  _ if preempted (true), then the token will be in front of tokens of same priority.
 *					  _ if blocked (false), then the token will be behind tokens of same priority.
 */
void Facility::enqueue(std::shared_ptr<const Token> token, EventType eventType, double serviceTime, bool isPreempted) {
	// Update statistics *before* modifying queue.  See Jain for explanation.
	sumLengthTimeProduct += queue.size() * (simulatorGlobals.getCurrentAbsoluteTime() - lastQueueChangeTime);
	lastQueueChangeTime = simulatorGlobals.getCurrentAbsoluteTime();
	
	// Insert this token in the queue with the event type and remaining service time.
	// If isPreempted is false, then it signifies the token is blocked, i.e., service has not yet begun for this token;
	// when dequeuing, the event associated with the token must be re-scheduled.
	// If isPreempted is true, then the token was preempted, and it will be put ahead of tokens of same priority in the queue
	// (but not ahead of tokens of higher priority).
	std::list<FacilityQueueElement>::iterator queueIterator = queue.begin();
	if (! isPreempted) {
		// Procedure for blocked tokens.
		while (queueIterator != queue.end()) {
			if (queueIterator->token->priority >= token->priority) { // Looking for token with priority lower than token to be enqueued.
				++queueIterator;
			} else {
				// Found proper place to insert token, after tokens of same priority.
				queue.insert(queueIterator, FacilityQueueElement(token, eventType, serviceTime));
				break; // Exit the loop;
			}
		}
		// Test whether end of queue was reached; if true, just insert at the end of queue.
		if (queueIterator == queue.end()) {
			queue.push_back(FacilityQueueElement(token, eventType, serviceTime));
		}
	} else {
		// Procedure for preempted tokens.
		while (queueIterator != queue.end()) {
			if (queueIterator->token->priority < token->priority) { // Looking for token with priority higher than token to be enqueued.
				++queueIterator;
			} else {
				// Found proper place to insert token, after tokens of same priority.
				queue.insert(queueIterator, FacilityQueueElement(token, eventType, serviceTime));
				break; // Exit the loop.
			}
		}
		// Test whether end of queue was reached; if true, just insert at the end of queue.
		if (queueIterator == queue.end()) {
			queue.push_back(FacilityQueueElement(token, eventType, serviceTime));
		}
	}
	
	//// Update statistics.  See Jain for explanation.
	//sumLengthTimeProduct += queue.size() * (simulatorGlobals.getCurrentAbsoluteTime() - lastQueueChangeTime);
	//lastQueueChangeTime = simulatorGlobals.getCurrentAbsoluteTime();
	
	// Update max queue length after queue change.
	if (queue.size() > maxRecordedQueueSize) {
		maxRecordedQueueSize = queue.size();
	}
}

/**
 * Retrieves next token in queue and schedules service for it at the head of the event chain.
 *
 * @details  
 * This function dequeues the token at the head of the queue and schedules a service request event for it, putting this event
 * at the head of the event chain (such that this event is the very next to occur).
 * Since the service request is being scheduled, and the token is not being put directly into service in this function,
 * the function does not have to check whether there really is a free, available server (this is a sanity check, since there SHOULD be a free
 * server when the dequeue function is called). These checks are done by the request function.
 * The function also updates related statistics.
 *
 * @return True if a token was in fact successfully dequeued; false if no dequeue happened (typicall if the queue was empty).
 */
bool Facility::dequeue() {
	if (! queue.empty()) {
		// Queue not empty:  dequeue token, schedule event, and update queue statistics.
		sumLengthTimeProduct += queue.size() * (simulatorGlobals.getCurrentAbsoluteTime() - lastQueueChangeTime);
		lastQueueChangeTime = simulatorGlobals.getCurrentAbsoluteTime();
		++dequeuedTokensCount;
		// Schedule event for dequeued token, put in head of event chain (call special schedule function).
		scheduler.scheduleFront(Event(0.0, queue.front().eventType, queue.front().token));
		queue.pop_front(); // Now actually remove first token from queue vector.
		return true; // Done, token at queue will be put into service.
	} else {
		return false; // Nothing in queue.
	}
}

/**
 * Purge queue, update statistics, return number of dropped elements (tokens).
 *
 * @return Number of tokens dropped from queue.
 */
unsigned int Facility::purgeQueue() {
	// Drop each token in the queue and update statistics.
	// Technically, statistics only have to be updated for the first element, since all others will result in zero delta
	// added to the statistics.  Let's leave the whole iteration here, however, for future implementations.
	unsigned int droppedTokensHere = 0; // Tokens actually dropped from queue. (This is simply the current queue.size(), but let's count for debugging purposes.)
	while (queue.size() > 0) {
		sumLengthTimeProduct += queue.size() * (simulatorGlobals.getCurrentAbsoluteTime() - lastQueueChangeTime); // For subsequent tokens, this will be a zero delta!
		// Discarded token, not dequeued token.  Then, do not update dequeued token counter.
		//dequeuedTokensCount--;
		++droppedTokensCount; // Update dropped token counter.
		lastQueueChangeTime = simulatorGlobals.getCurrentAbsoluteTime();
		queue.pop_front(); // Remove token in front of queue.
		++droppedTokensHere;
	}
	return droppedTokensHere;
} 

/**
 * Request service for a token at this facility.
 *
 * @details 
 * Receives a token and requests service for it. Searches for a free server within this facility, and, if found, puts the token into service in this
 * (first found) free server.  There are three possible cases:
 *
 * (1) Facility is down: if the facility is down, the token is not put into service, and the dropped tokens counter is updated. Hence, the token is considered dropped
 *     (but it is not actually dropped in this function). Returns FACILITY_DOWN_TOKEN_DROPPED.
 *
 * (2) No free server was found: if no free server was found in the facility, then then token in enqueued with the current (received) event type and 0.0 as service time
 *     (that indicates a blocked (not preempted) token). Returns FACILITY_BUSY_TOKEN_ENQUEUED.
 *
 * (3) Token put into service: the list of servers is searched from beginning to end. Once a free server is found, the token is put into service there, and the corresponding
 *     start of service time and other server member variables are updated. Returns TOKEN_PUT_INTO_SERVICE.
 *
 * @param token Token object for which service is requested.
 * @param eventType Event type of the event that requested service for this token (necessary for enqueing the token, and later scheduling a new request for the dequeued token).
 * @return Return type or result of this token (whether the token was enqueued, put into service, etc.).
 */
FacilityReturnType Facility::request(std::shared_ptr<const Token> token, EventType eventType) {
	std::vector<FacilityServer>::iterator serversIterator;

	// Increment request count.
	++requestsPreemptsCount;

	// If facility is down, token should be discarded, and statistics, updated.
	if (!isUp_) {
		++droppedTokensCount; // Increment dropped tokens counter.
		return FacilityReturnType::FACILITY_DOWN_TOKEN_DROPPED;
	}
	// Get next free server, if any. The return might be an iterator to a free server, or servers.end() if no free server was found.
	serversIterator = nextFreeServer();
	// Check whether the iterator points to servers.end(), which, in this case, means there are not free servers and token  must be enqueued.
	if (serversIterator == servers.end()) {
		if (queue.size() < queueSizeLimit) { // Only enqueue if queue is below size limit.
			enqueue(token, eventType, 0.0, false); // Enqueue with 0.0 and false, indicating this is a blocked token (not preempted).
			return FacilityReturnType::FACILITY_BUSY_TOKEN_ENQUEUED;
		} else { // Facility was busy and queue has reached capacity; drop token.
			++droppedTokensCount;
			return FacilityReturnType::FACILITY_BUSY_QUEUE_FULL_TOKEN_DROPPED;
		}
	} else {
		// Put token into service in this server.
		serversIterator->token = token;
		serversIterator->serviceStartTime = simulatorGlobals.getCurrentAbsoluteTime(); // Update start of service.
		serversIterator->isBusy = true; // Mark this server as busy.
		return FacilityReturnType::TOKEN_PUT_IN_SERVICE;
	}
}

/**
 * Preempts a token already in service and attempts to put another token in service at this facility.
 *
 * @details 
 * Receives a token with the associated priority. Searches the list of servers for either a free server, or a busy server servicing a lower-priority token.
 * First, the function searches for a free server. If none is found, then it begins searching for busy servers servicing lower-priority tokens.
 *
 * There are three possible cases:
 *
 * (1) Facility is down: if the facility is down, the token is not put into service, and the dropped tokens counter is updated. Hence, the token is considered dropped
 *     (but it is not actually dropped in this function). Returns FACILITY_DOWN_TOKEN_DROPPED.
 *
 * (2) A free server was found: the token is put into service in this server. Here, the preempt function works exactly as the request function. Returns TOKEN_PUT_INTO_SERVICE.
 *
 * (3a) No free server was found: if no free server was found, then the list of servers is again searched, now considering the priority of the token passed as argument and
 *      the priority of tokens in service. Among all busy servers, the one servicing the token with the lowest priority is selected. If this priority is lower than the
 *      priority of the token passed as argument (i.e., the priority of the argument token is strictly less than the priority of the token in the busy server), then this
 *      token being serviced is removed from service, and the argument token is put into service in place. The preempted token is enqueued with the remaining service time
 *      and also according to its priority, but before tokens with the same priority (such that it is service before those tokens). A release event for the preempted token
 *      *must* be removed from the event chain!!
 *      Returns TOKEN_PUT_INTO_SERVICE.
 * @todo enqueuing here cannot be made with the enqueue function, since it will enqueue the token *after* tokens with same priority!
 *
 * (3b) No free server was found: no free server was found, and, among the busy servers, all tokens in service have priority equal or higher than the argument token.
 *      Then, enqueue the token as it is done in the request function. Returns FACILITY_BUSY_TOKEN_ENQUEUED.
 *
 * @param token Token object for which service is requested.
 * @param eventType Event type of the event that requested service for this token (necessary for enqueing the token, and later scheduling a new request for the dequeued token).
 * @return Return type or result of this token (whether the token was enqueued, put into service, etc.).
 */
/// @todo To be implemented.
/// @todo Verify behavior here in case there is a queue size limit!!
FacilityReturnType Facility::preempt(std::shared_ptr<const Token> token, EventType eventType) {
	// Increment preemptions count.
	++requestsPreemptsCount;
	
	return FacilityReturnType::NOT_IMPLEMENTED; /// @todo dummy

	// Comments from Tarvos regarding releasedTokensCount.
	
/* Note: this function, as it is, is *not* incrementing # of releases upon a successful preemption.
*       This is in contrast with SMPL approach. Although the current approach will yield a correct
*       Mean of Busy Time calculation, problems might arise if a preempted token never goes back to service.
*       If a preempted token never goes back to service, then the service time is has received so far did
*       go into Sum of Busy Time, but a # release count will never be accumulated for this token. Therefore,
*       the effect is adding time to the Sum of Busy Time for no specific token, causing an "error" in the
*       computation of the Mean of Busy Time.
*
*       (more)
*		This SMPL behavior has effects on the correct calculation of certain statistics,
*		namely Sum of Busy Time and Mean Busy Period. Mean Busy Period relies on # of releases to calculate the mean.
*		A preempted token moving back into service, and then released, can be technically considered as two tokens:
*		one is serviced up to time t and then "released"; the second is serviced for the remaining service time and then
*		released. Sum of Busy Time accumulates both parts of the service (before preemption, and after resuming service), 
*		and Number of Releases accumulates two releases for the same token. The final Mean Busy Period will be slightly different
*		than accumulating one whole service time (the sum of the two parts of the service for the preempted token) and dividing by
*		*one* single release for the same token.
*		The larger the value of Sum of Busy Time and # of Releases, the less the difference, but it exists.
*		SMPL treatment also causes problems if # of Releases is used as # of Tokens serviced. A preempted token will be counted twice.
*		A simple fix is to obtain the # of Serviced tokens as # of Releases - # of Preemptions.
*		And, perhaps, use, for calculating the Mean Busy Period, the # of Serviced Tokens, and not # of Releases.
*		(01.August.2013 Marcos Portnoi)	
*/
	
	// This is Tarvos.

	///*-------PREEMPT FACILITY WITH TOKEN POINTER - FC preemptp ----------------
//* A funcao preempt foi modificada com o acrescimo de um apontador associado
//* a tkn para que na area apontada tenha mais informacoes sobre a tkn. Desta
//* maneira pode-se acrescentar caracteristicas da token. Neste caso criou-se
//* uma variavel tkp - token pointer com o tipo da estrutura do pacote.
//* Este programa trabalha com um sistema de prioridade de processamento da
//* token onde o maior valor numerico de prioridade tem tambem a maior prioridade de
//* execucao.
//* Assim, uma token com prioridade 5 devera ser processada antes de tokens com prioridade
//* menor do que cinco.
//
//* Comentário:  na função originalmente implementada, caso as tokens em serviço tenham
//* prioridade igual ou menor que a token que chama a preempção, então não haverá preempção;
//* esta função chama pois a função enqueuep_preempt, que coloca a token em questão na fila
//* da facility em ordem de prioridade, mas à frente de outras tokens de mesma prioridade.
//* Não seria portanto uma ação idêntica à requestp, como prevê MacDougall para o SMPL.
//* Em havendo de fato a preempção, a token que foi retirada de serviço vai para a fila em
//* ordem de prioridade, e à frente de outras tokens de mesma prioridade, podendo assim
//* voltar a serviço imediatamente à frente das outras tokens enfileiradas de mesma prioridade.
//* O funcionamento original, portanto, não está conforme previsto por MacDougall no SMPL.  Se não
//* houver preempção e preemptp deve funcionar como um requestp neste caso, então a token
//* deve ser enfileirada por ordem de prioridade em ao fim das tokens de mesma prioridade
//* (deve obedecer ao esquema FIFO dentre a mesma prioridade).  Uma chamada à função enqueuep,
//* ao invés de enqueuep_preempt, para o caso de não haver preempção, deve resolver a questão.
//* (08.Jan.2006 Marcos Portnoi)
//*
//* A função retorna:
//*  0 se a facility tem servidor disponível e o token foi colocado em serviço;
//*  1 se a facility não tem servidor disponível; token colocada na fila
//*  2 se a facility está down (não-operacional); nada foi feito com a token
//* O contador de tokens descartados também é incrementado para o caso 2.
//* (estas ampliações de estado da facility 10.Jan.2006 Marcos Portnoi)
//*/
//int preemptp (int f, int tkn, int pri, int ev, double te, TOKEN *tkp) {
//	int r, tkn_srv, tkn_pri_srv, ev_srv, menor_pri;
//	struct facilit *fct;
//	struct fserv *srv, *srv_menor_pri;
//	struct evchain *evc_tkn_srv;
//	TOKEN *tkp_srv;
//	double te_srv;
//
//	if ( tkn == 0 )
//	{
//		printf ("\nError - preemptp - token = 0 ");
//		exit (1);
//	}
//
//	if ( f > sim[sn].fct_number)
//	{
//		printf ("\nError - preemptp - facility number does not exist");
//		exit (1);
//	}
//
//	fct = sim[sn].fct_begin;
//	while (fct->f_number != f)
//	{
//		fct=fct->fct_next;
//	}
//
//	//se a facility estiver down, retorne imediatamente com 2
//	//O programa do usuário deve decidir o que fazer com a token (descartar ou não)
//	if (getFacUpStatus(f)==0)
//	{
//		fct->f_tkn_dropped++; //incremente contador de tokens descartadas; se isso não for interessante para o usuário, então esta linha deve ser editada
//		return 2;
//	}
//
//	/*Situacoes possíveis na preempção:
//     1. Se tiver servidor livre aloca o servidor com a token e retorna 0 da funcao
//	 2. Se todos os servidores estiverem ocupados verifica-se as prioridades das tokens em servico
//			a. Se as tokens em servico tiverem prioridade maior ou igual a token que chama a preempção, entao a
//			   token que chama a preempção eh colocada em fila como qualquer outra token e 1 eh retornado ou seja
//				 o preempt funciona exatamente como o request.
//			b. Se a token em servico tiver prioridade menor que do que a token atual, retira a token
//			   de servico fazendo todas as atualizacoes necessarias -
//				 . retira da cadeia de eventos o termino do servico desta token (caso ela nao encontre
//				   esta token com termino de servico um erro e retornado
//				 . acrescenta a utilizacao na facility e no servidor do tempo que ela passou em servico
//				   este tempo eh o clock atual subtraido do tempo em que ela entrou em servico
//					 pois de toda forma ele ja usou os recursos de servico
//				 . coloca entao a token retirada em servico na fila de espera onde o tempo que estara
//				   associado ao termino de servico da token e o tempo final de servico do evento
//					 retirado da cadeia de eventos diminuido do clock atual
//				 . a insercao da token na fila de espera respeitara a seguinte condicao: ela sera
//				   colocada para execucao antes das tokens de mesma prioridade. Assim, dentro das
//					 tokens de mesma prioridade ela sera a primeira a ser servida.
//				 . coloca a token que chama a preempção em servico
//				 . retorna 0, pois a token foi colocada em serviço
//	*/
//
//	if ( fct->f_n_busy_serv < fct->f_n_serv )	/* Ainda tem servidor livre */
//	{
//			srv = fct->f_serv;
//			while (srv->fs_tkn != 0)
//			{
//				srv=srv->fs_next;
//			}
//			srv->fs_tkn = tkn;
//			srv->fs_p_tkn = pri;
//			srv->fs_start = clock;				/* Comeco de novo servico */
//			fct->f_n_busy_serv++;
//			r = 0;
//			return (r);
//	}
//
//	/* Se ele chegar aqui siginifica que a Facility esta busy - assim deve-se
//	   verificar os servidores que estao ocupados para verificar prioridade da
//	   token em servico e efetuar os processamentos 2.a ou 2.b */
//
//	srv = fct->f_serv;
//	srv_menor_pri = nullptr;
//	menor_pri = pri;
//	while (srv != nullptr)
//	{
//		if (menor_pri > srv->fs_p_tkn)
//		{
//			menor_pri = srv->fs_p_tkn;
//			srv_menor_pri = srv;
//		}
//		srv=srv->fs_next;
//	}
//
//	if (srv_menor_pri == nullptr)			/* Processamento 2.a inicio */
//
//	/* Significa que ele nao achou um servidor com tkn em servico com prioridade menor
//	   do que a token que chama a preempção. Ou seja, as tokens tem prioridade maior ou igual a token que chama a preempção.
//	   Não haverá, pois, suspensão de tokens em serviço.*/
//	{
//	    /*(Comentário:  Sergio Brito) Neste ponto discordo do MacDougall pois ele diz que, para a token que chama a preempção, se forem achados
//		  servidores com token em servico com prioridade maior ou igual à dessa, a token que chama a preempção deve ser
//		  enfileirada como é feito em um request tradicional (deve ficar atrás, na fila, de tokens com mesma prioridade).
//		  Eu acho que ela deve ser enfileirada com o mesmo esquema de prioridade que eh feito no caso 2.b, assim ao
//		  inves de usar a funcao - enqueuep(f, tkn, tkp, pri, te, ev); - deve usar a funcao: enqueuep_preempt */
//		/* O problema aqui é que, assim, a token que não causou preempção vai para fila à frente de outras tokens de mesma
//		   prioridade... isso é desejável?  O comportamento neste caso assemelha-se a elevação da prioridade da token que chamou a função
//		   (08.Jan.2006 Marcos Portnoi)*/
//		/* Façamos como o MacDougall:  colocar a token bloqueada após as tokens de mesma prioridade
//		enqueuep_preempt(f, tkn, tkp, pri, te, ev);*/
//		enqueuep(f, tkn, tkp, pri, te, ev); //coloca a token em fila em ordem de prioridade, após as tokens de mesma prioridade
//		r = 1;
//		return (r);
//	}       					/* Processamento 2.a final */
//
//	srv = srv_menor_pri;		/* Processamento 2.b inicio:  aqui, efetivamente, haverá preempção (suspensão de token em serviço) */
//
//	/* faz o servidor atual ser o com tkn de menor prioridade */
//	/* retira a token da cadeia de eventos com o tempo de termino de servico
//	   deve retornar apos o processamento o apontador para o elemento da cadeia
//	   de eventos que contem as informacoes de processamento da token */
//
//	evc_tkn_srv = cancelp_tkn(srv->fs_tkn);
//
//	if (evc_tkn_srv == nullptr) { //se for nullptr, indica que o token não foi encontrado na cadeia de eventos
//		printf("\nError - preemptp - token to be preempted not found in event chain (possible release event)");
//		exit(1);
//	}
//
//	/* obtem as informacoes da token em servico para que ela possa ser enfileirada */
//
//	if ( evc_tkn_srv->ev_tkn != srv->fs_tkn ) /* Testa se realmente a tkn obtida eh a em srv */
//	{
//		printf ("\nError - preemptp - token removed from event chain is different from token in service");
//		printf ("\nError - preemptp - token removed from event chain is different from token in service");
//		exit (1);
//	}
//
//	tkn_srv = evc_tkn_srv->ev_tkn;
//	tkp_srv = evc_tkn_srv->ev_tkn_p;
//	te_srv = (evc_tkn_srv->ev_time - clock);
//	tkn_pri_srv = srv->fs_p_tkn;
//	ev_srv = evc_tkn_srv->ev_type;
//
//	/* coloca a token em servico na fila de espera da facility como a primeira que sera
//	   servida em relacao as tokens de mesma prioridade */
//
//	enqueuep_preempt(f, tkn_srv, tkp_srv, tkn_pri_srv, te_srv, ev_srv);
//
//	/* atualiza estatisticas do servidor apos saida da tkn que estava em servico */
//	srv->fs_tkn = 0;
//	srv->fs_p_tkn = 0;
//	  /* srv->fs_release_count++;nao deve ser contabilizado mais um release pois senao
//		 um job criado na fonte seria representado por varios pedacos.
//		 Entao, so deve ocorrer a contabilizacao do release, quando ele
//		 for retirado de servico pela funcao release.  (Sergio Brito)
//		 Observar que o SMPL, do MacDougall, contabiliza o release na suspensão de serviço
//		 (preempção).  Isso de fato criaria jobs fragmentados.  hmmmmm... (08.Jan.2006 Marcos Portnoi)*/
//	srv->fs_busy_time = srv->fs_busy_time + ( clock - srv->fs_start ); /*Acumula tempo uso serv */
//
//	/* atualiza estatisticas da facility apos saida do servico */
//	fct->f_busy_time = fct->f_busy_time + ( clock - srv->fs_start );
//	/* fct->f_release_count++; idem srv->fs_release_cont++ */
//	fct->f_n_busy_serv--; //agora há um servidor livre
//	fct->f_preempt_count++; //incrementa contador de preempções, pois aqui efetivamente ocorreu uma
//
//	/* apos a utilizacao de todas as informacoes da token que foi tirada da cadeia e do
//	   servico e foi enfileirada na facility, podemos liberar a area de memoria que
//	   o elemento da cadeia estava usando */
//
//	free(evc_tkn_srv);
//
//	/* coloca a token que chama a preempção em servico */
//
//	srv->fs_tkn = tkn;
//	srv->fs_p_tkn = pri;
//	srv->fs_start = clock;		/* Comeco de novo servico */
//	fct->f_n_busy_serv++;
//	r = 0;						/* retorna 0 pois a solicitacao de servico foi aceita
//								   para que seja escalonado o termino de servico na
//								   rotina que a chamou */
//
//	return (r);					/* Processamento 2.b final */


	// This is from SMPL. Notice the comment and how the remaining service time is artificially set to 1.0e-99 to mark this token as not blocked (just about to be released).
	//else
 //               { /* preempt user of server k.  suspend event, save   */
 //                 /* event number & remaining event time, & enqueue   */
 //                 /* preempted token.  If remaining event time is 0   */
 //                 /* (preemption occurred at the instant release was  */
 //                 /* to occur, set 'te' > 0 for proper enqueueing     */
 //                 /* (see 'enlist').  Update facility & server stati- */
 //                 /* stics for the preempted token, and set r = 0 to  */
 //                 /* reserve the facility for the preempting token.   */
 //                 if (tr) then msg(8,tkn,fname(f),2,0);
 //                 j=l1[k]; i=suspend(j); ev=l3[i]; te=l5[i]-clock;
 //                 if (te==0.0) then te=1.0e-99; put_elm(i);
 //                 enqueue(f,j,l2[k],ev,te);
 //                 if (tr) then
 //                   {msg(10,-1,"",j,l3[f]); msg(12,-1,fname(f),tkn,0);}
 //                 l3[k]++; l4[k]+=clock-l5[k];
 //                 l2[f]--; l4[f+1]++; r=0;

// This is from enlist. Notice that the decision on where to put the enqueued token (in front of tokens of same priority, or after) is
// made based on the remaining service time; if 0.0 (blocked token), put at end; if > 0 (preemted token), put at head.
// I believe, in this simulator, we can make use of a flag (blocked/non-blocked)?
	
	//else
//                    { /* queue: if entry is for a preempted token-    */
//                      /* (l4, the remaining event time, >0), insert   */
//                      /* entry at beginning of its priority class;    */
//                      /* otherwise, insert it at the end              */
//                      if ((v<arg) || ((v==arg) && (l4[elm]>0.0)))
//                        then break;
}

/**
 * Releases a token from service at a facility.
 *
 * @details 
 * Searches the list of servers for the one servicing the token passed as argument. Once found, updates statistics and status for that server. Checks queue:
 * if not empty, schedules a request event for the token at the head of the queue (and the event is put at the head of the event chain).
 * If no server is found with the argument token, then we have reached an error condition: a release event should not exist for a token not in service. This could happen
 * if the token was preempted, and the release event was not properly removed from the event chain.
 *
 * Notice that the dequeue procedure for the token queue is done entirely here.
 *
 * @param token Token object to be released from service.
 */
FacilityReturnType Facility::release(std::shared_ptr<const Token> token) {
	std::vector<FacilityServer>::iterator serversIterator;
	
	// Iterate through the servers in this facility to find which one has the token.
	serversIterator = servers.begin();
	while (serversIterator != servers.end()) {
		if (serversIterator->isBusy == true && serversIterator->token == token) {
			// Found busy server with the token. Let's release and update statistics.
			serversIterator->isBusy = false;
			serversIterator->token.reset(); // One less reference to the token, i.e., server down not "own" token anymore.
			++serversIterator->releasedTokensCount;
			serversIterator->sumBusyTime += simulatorGlobals.getCurrentAbsoluteTime() - serversIterator->serviceStartTime; // Acumulates busy time for this server.
			// Update facility's statistics.
			this->sumBusyTime += simulatorGlobals.getCurrentAbsoluteTime() - serversIterator->serviceStartTime; // Acumulates busy time for facility.
			++releasedTokensCount;

			// Now check queue. If true, then there was a token in queue and service request will be scheduled for it.
			if (dequeue()) {
				return FacilityReturnType::TOKEN_RELEASED_DEQUEUED_SERVICE_EVENT_SCHEDULED; // Done, token at head of queue will be put into service at next event.
			}
			// If false, queue was empty. Facility is now free, nothing scheduled for it.
			return FacilityReturnType::TOKEN_RELEASED_QUEUE_EMPTY;
		} // Did not find server yet with token. Continue.
		++serversIterator;
	} // Did not find this token in service in any server. Error (?).
	std::cout << "\nError - release - token not found in service at this facility." << std::endl;
	//exit(1);
	return FacilityReturnType::TOKEN_NOT_FOUND; // This is an error condition (not consistent)! Treat appropriately.
}

/**
 * Returns the busy status of this facility.
 *
 * @details 
 * The busy status has no relation to the up/down status.
 *
 * @todo Perhaps should analyze the performance cost of this function, and use a bool isBusy member variable? The number of servers in the facility is really not dynamic...
 *
 * @return True if busy, false if non-busy (not necessarily available, however; facility might be down).
 */
bool Facility::isBusy() const {
	// If all servers in vector are busy, then facility is busy.
	// If the iterator returned by nextFreeServer() does not point to servers.end(), then a free server was found.
	if (nextFreeServer() != servers.end()) {
		return false;
	}
	return true; // If reached this line, then no free server was found (servers.end() was returned by nextFreeServer()).
}


/**
 * Returns iterator to a free server within this facility.
 *
 * @details 
 * The iterator might point servers.end(), indicating that there are no free servers. This
 * must be tested by the calling function.
 *
 * @note I found no way of using const_cast<>(static_cast<>()) to cast away constness and re-use the const version of this function here,
 * as described in Effective C++, by Scott Meyers.
 *
 * @return Iterator to a free server, or servers.end().
 */
std::vector<FacilityServer>::iterator Facility::nextFreeServer() {
	for (std::vector<FacilityServer>::iterator serversIterator = servers.begin(); serversIterator != servers.end(); ++serversIterator) {
		if (! serversIterator->isBusy) { // Return immediately with reference if one server is found free.
			return serversIterator;
		}
	}
	return servers.end(); // If reached this line, then no free server was found.
}

/**
 * Returns iterator to a free server within this facility, const version.
 *
 * @details 
 * The iterator might point servers.end(), indicating that there are no free servers. This
 * must be tested by the calling function.
 *
 * @return Const_iterator to a free server, or servers.end().
 */
std::vector<FacilityServer>::const_iterator Facility::nextFreeServer() const {
	for (std::vector<FacilityServer>::const_iterator serversIterator = servers.begin(); serversIterator != servers.end(); ++serversIterator) {
		if (! serversIterator->isBusy) { // Return immediately with reference if one server is found free.
			return serversIterator;
		}
	}
	return servers.end(); // If reached this line, then no free server was found.
}

/**
 * Returns up/down status of facility.
 *
 * @details 
 * Up/down status has no relation to busy status.
 *
 * @return True if up, false if down.
 */
bool Facility::isUp() const {
	return isUp_;
}

/**
 * Returns Mean Service Rate.
 *
 * @return Mean service rate.
 */
double Facility::getMeanServiceRate() const {
	return getFullyServicedTokensCount() / (simulatorGlobals.getCurrentAbsoluteTime() * getUtilization());
}

/**
 * Make facility UP.
 */
void Facility::setUp() {
	isUp_ = true;
}

/**
 * Make facility DOWN.
 *
 * @details 
 * Also discard all elements from queue and update counters. If this is unwanted, edit the proper function call within.
 *
 * @return Number of discarded tokens.
 */
unsigned int Facility::setDown() {
	// Purge current queue, update counters. Comment out if this is unwanted.
	isUp_ = false;
	return dropTokensInService() + purgeQueue();
	
}

/**
 * Return current queue size.
 *
 * @return Queue size.
 */
std::list<FacilityQueueElement>::size_type Facility::getQueueSize() const {
	return queue.size();
}

/**
 * Return facility utilization.
 *
 * @return Utilization.
 */
double Facility::getUtilization() const {
	// Get the duration of the simulation up to this moment.
	double duration = simulatorGlobals.getSimulationDuration();
	// If the duration of the simulation is zero, then utilization is zero (avoid /0 here).
	return (duration > 0.0) ? sumBusyTime / duration : 0.0;
}

/**
 * Return mean busy period of facility, all released tokens (including preemptions).
 *
 * @details 
 * See Jain, MacDougall.
 * This function counts all releases, including preemtions, in the calculation. As such, a token preempted once will count as two releases. This is the MacDougall approach.
 * For counting preempted, fully serviced tokens, use the polymorphic getMeanBusyPeriod(bool onlyFullyServiced) instead.
 *
 * @return Mean busy period.
 */
double Facility::getMeanBusyPeriod() const {
	// If there is no released token yet, then just return the current busy time.
	return (releasedTokensCount > 0) ? sumBusyTime / releasedTokensCount : sumBusyTime;
}

/**
 * Return mean busy period of facility (not including preemptions, only fully serviced tokens).
 *
 * @details 
 * See Jain.
 *
 * @return Mean busy period.
 */
double Facility::getMeanBusyPeriod(bool onlyFullyServiced) const {
	// If there is no released token yet, then just return the current busy time.
	return (getFullyServicedTokensCount() > 0) ? sumBusyTime / getFullyServicedTokensCount() : sumBusyTime;
}

/**
 * Return mean queue length.
 *
 * @details 
 * See Jain, MacDougall.
 *
 * @return Mean queue length.
 */
double Facility::getMeanQueueLength() const {
	double duration = simulatorGlobals.getSimulationDuration();
	// If duration equals zero, then the mean queue length is obviously zero.
	return (duration > 0.0) ? sumLengthTimeProduct / duration : 0.0;
}

/**
 * Get facility's name.
 *
 * @return Name.
 */
std::string Facility::getName() const {
	return name;
}

/**
 * Set facility's name.
 */
void Facility::setName(const std::string &name) {
	this->name = name;
}

/**
 * Get number of servers in this facility.
 *
 * @details 
 * Basically counts the size of servers vector.
 *
 * @return Number of servers.
 */
unsigned int Facility::getNumberOfServers() const {
	return servers.size();
}

/**
 * Get current maximum recorded queue size from this facility.
 *
 * @return Maximum recorded queue size.
 */
unsigned int Facility::getMaxRecordedQueueSize() const {
	return maxRecordedQueueSize;
}

/**
 * Get number of dequeues (elements removed from the queue for processing) from this facility's queue.
 *
 * @return Number of dequeues.
 */
unsigned int Facility::getDequeuedTokensCount() const {
	return dequeuedTokensCount;
}

/**
 * Get last absolute time a change in queue (dequeue or enqueue) has happened for this facility.
 *
 * @details 
 * Will be used to obtain average queue size.
 *
 * @return Absolute time of last queue change.
 */
double Facility::getLastQueueChangeTime() const {
	return lastQueueChangeTime;
}

/**
 * Get total (relative, elapsed) time this facility was busy.
 *
 * @details 
 * Sum (integral) of busy time of all this facility's servers.
 *
 * @return Relative, elapsed time this facility was busy.
 */
double Facility::getSumBusyTime() const {
	return sumBusyTime;
}

/**
 * Get count of preemptions occurred so far in this facility. 
 *
 * @details 
 * Sum of preemptions of all servers in this facility.
 *
 * @return Count of preemptions.
 */
unsigned int Facility::getPreemptedTokensCount() const {
	return preemptedTokensCount;
}

/**
 * Get sumLengthTimeProduct for this facility.
 *
 * @details 
 * += Current queue size * (currentAbsoluteTime - absolute time of last queue change); used to obtain average queue size.
 *
 * @return Sum of queue length multiplied by time delta in queue.
 */
double Facility::getSumLengthTimeProduct() const {
	return sumLengthTimeProduct;
}

/**
 * Get count of releases from all servers in this facility.
 *
 * @details 
 * The number of releases also counts each preemtion as a release (from the preempt function).
 *
 * @return Count of releases.
 */
unsigned int Facility::getReleasedTokensCount() const {
	return releasedTokensCount;
}

/**
 * Get count of fully serviced tokens (no including preemptions) from all servers in this facility.
 *
 * @details 
 * The number of releases also counts each preemtion as a release (from the preempt function). A token preempted once, and then fully serviced (and released)
 * will count as two releases and one preemption.
 * Obviously, if there are not preemptions, then releasedTokensCount = fully serviced tokens count.
 *
 * @return Count of fully serviced tokens.
 */
unsigned int Facility::getFullyServicedTokensCount() const {
	return releasedTokensCount - preemptedTokensCount;
}

/**
 * Get count of dropped or discarded tokens in this facility.
 *
 * @details 
 * A token will typically be discarded when the token arrives for service and the facility is currently down.
 *
 * @return Count of dropped or discarded tokens.
 */
unsigned int Facility::getDroppedTokensCount() const {
	return droppedTokensCount;
}

/**
 * Get count of service requests and preemptions made to this facility.
 *
 * @details 
 * Notice that a token might have more than one request, e.g., if the first request finds a busy facility, token is enqueued. Dequeuing will generated another service request.
 * Therefore, for this single token, two requests would be generated and counted. This statistic is *not*, therefore, the same as number of unique arrivals at the facility's queue.
 * To obtain this count of unique arrivals, perhaps it is best to count the number of tokens generated for this facility from specific traffic source.
 * Obviously this statistic can be calculated from others, but it is good to have it independent for debugging purposes.
 *
 * @return Count of service requests and preemptions made to this facility.
 */
unsigned int Facility::getRequestsPreemptsCount() const {
	return requestsPreemptsCount;
}

/**
 * Set queue size limit.
 *
 * @details 
 * When the queue reaches the limit, all subsequent service requests (tokens) will be dropped until the queue size decreases below the limit.
 * Verify behavior for preemption under preempt function description.
 * Default value is std::numeric_limits<unsigned int>::max().
 *
 * @param limit queue size limit as unsigned int.
 */
void Facility::setQueueSizeLimit(unsigned int limit) {
	queueSizeLimit = limit;
}

/**
 * Get current queue size limit.
 *
 * @details 
 * When the queue reaches the limit, all subsequent service requests (tokens) will be dropped until the queue size decreases below the limit.
 * Verify behavior for preemption under preempt function description.
 * Default value is std::numeric_limits<unsigned int>::max().
 *
 * @return Current queue size limit.
 */
unsigned int Facility::getQueueSizeLimit() const {
	return queueSizeLimit;
}

/**
 * Drop all tokens in service.
 *
 * @details 
 * This function is typically called when a facility is put down. Then the current queue is purged, and
 * all tokens currently in service must also be dropped.
 * This function iterates through the list of servers; for every busy server, get the token, cancel future events for the token (release events),
 * update statistics (e.g., busy time), and drop the token, making the server non-busy.\
 *
 * @return Number of tokens in service that were dropped.
 */
unsigned int Facility::dropTokensInService() {
	std::vector<FacilityServer>::iterator serversIterator;
	serversIterator = servers.begin();
	unsigned int droppedTokensHere = 0;

	// Iterate through the list of servers. For each busy server, cancel its token events (e.g., release events), update stats, drop token, make server non-busy.
	while (serversIterator != servers.end()) {
		if (serversIterator->isBusy == true) {
			// Found busy server. Let's cancel events for the token being serviced.
			scheduler.removeEvents(serversIterator->token);
			serversIterator->isBusy = false; // Not busy anymore.
			serversIterator->token.reset(); // Clean the reference.
			// Update server stats.
			serversIterator->sumBusyTime += simulatorGlobals.getCurrentAbsoluteTime() - serversIterator->serviceStartTime; // Acumulates busy time for this server.
			// Update facility stats.
			this->sumBusyTime += simulatorGlobals.getCurrentAbsoluteTime() - serversIterator->serviceStartTime; // Acumulates busy time for facility.
			++droppedTokensCount; // Token was dropped; update global count.
			++droppedTokensHere; // Update local count.
		}
		++serversIterator; // Continue to next server.
	}
	return droppedTokensHere;
}