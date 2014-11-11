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




/* Estrutura para acumular estat?sticas em cada nodo ou seu destino final. Ex.:  quantos pacotes chegaram ao
*  destino e quantos ficaram no caminho.  Se os links forem duplex, estas estat?sticas acumular?o para todos os sentidos adequados.
*/
struct Node {
	//este grupo de estat?sticas ? global, compreedendo pacotes de aplica??o e de controle
	int packetsReceived;  //n?mero de pacotes que chegaram ao nodo.  Pode-se verificar ent?o quantos ficaram no caminho (descartados).
	double bytesForwarded; //quantidade de bytes que foram encaminhados a partir deste nodo.  Similar ao packetsForwarded abaixo
	int packetsForwarded; //n?mero de pacotes que foram encaminhados a partir deste nodo.  Quando o nodo n?o for o sorvedouro, o pacote ser? encaminhado por outro link, e esta estat?stica dever? ser atualizada.
	int packetsDropped; //n?mero de pacotes perdidos ou descartados no nodo (partindo dele); ? o somat?rio dos pacotes perdidos pela facility (transmiss?o) e os perdidos durante propaga??o, para o caso de link down.
	double meanDelay; //atraso m?dio medido para os pacotes recebidos por este nodo
	double delaySum; //somat?rio de todos os atrasos medidos para os pacotes recebidos por este nodo
	double delay; //atraso do ?ltimo pacote recebido por este nodo
	double meanJitter; //jitter m?dio para os atrasos dos pacotes recebidos por este nodo
	double jitterSum; //somat?rio de todos os jitters calculados para os pacotes recebidos por este nodo
	double jitter; //?ltimo jitter calculado para o ?ltimo pacote recebido por este nodo
	double bytesReceived; //n?mero de bytes recebidos pelo nodo.  ? o tamanho de cada pacote recebido, acumulado (admite-se, aqui, pacotes de tamanho diferente).
	//as estat?sticas abaixo s?o exclusivamente para pacotes de aplica??o, ou seja, n?o-controle
	int packetsReceivedAppl;  //n?mero de pacotes que chegaram ao nodo.  Pode-se verificar ent?o quantos ficaram no caminho (descartados).
	double bytesForwardedAppl; //quantidade de bytes que foram encaminhados a partir deste nodo.  Similar ao packetsForwarded abaixo
	int packetsForwardedAppl; //n?mero de pacotes que foram encaminhados a partir deste nodo.  Quando o nodo n?o for o sorvedouro, o pacote ser? encaminhado por outro link, e esta estat?stica dever? ser atualizada.
	double meanDelayAppl; //atraso m?dio medido para os pacotes recebidos por este nodo
	double delaySumAppl; //somat?rio de todos os atrasos medidos para os pacotes recebidos por este nodo
	double delayAppl; //atraso do ?ltimo pacote recebido por este nodo
	double meanJitterAppl; //jitter m?dio para os atrasos dos pacotes recebidos por este nodo
	double jitterSumAppl; //somat?rio de todos os jitters calculados para os pacotes recebidos por este nodo
	double jitterAppl; //?ltimo jitter calculado para o ?ltimo pacote recebido por este nodo
	double bytesReceivedAppl; //n?mero de bytes recebidos pelo nodo.  ? o tamanho de cada pacote recebido, acumulado (admite-se, aqui, pacotes de tamanho diferente).
	struct nodeMsgQueue *nodeMsgQueue; //apontador para a lista (Queue) de mensagens de controle deste nodo; aponta para o Head Node
	int (*nextLabel)[]; /*pr?ximo r?tulo dispon?vel para uso em constru??o de uma LSP para MPLS; ao usar este r?tulo, este campo deve ser
				   incrementado a fim de assegurar r?tulos ?nicos por interface no nodo; o ?ndice indica o n?mero da interface, que tamb?m ? o n?mero do link do modelo*/
	double ctrlMsgTimeout; //valor de timeout (relativo) para mensagens de controle RSVP-TE em segundos
	int ctrlMsgHandlEv; //n?mero do evento para tratamento de mensagens de controle
	int helloMsgGenEv; //n?mero do evento para tratamento de mensagens espec?ficas HELLO
	double LSPtimeout; /*tempo relativo para a extin??o (timeout) de uma LSP que parte deste nodo; as LSPs devem receber REFRESH periodicamente (o RSVP-TE ? soft state).
					 A fun??o de cria??o de uma LSP deve recolher este valor de modo a calcular o tempo absoluto de timeout de uma LSP.  Uma fun??o espec?fica de timeout
					 deve ser chamada periodicamente a fim de extinguir as LSPs.*/
	double helloMsgTimeout; //tempo default para timeout do timer de recebimento de uma mensagem HELLO (o estouro indica falha na comunica??o com o nodo)
	double helloTimeout; //tempo default limite para recebimento de um HELLO_ACK (o estouro deste indica falha na comunica??o com o nodo)
	double (*helloTimeLimit)[]; //tempo limite para que o nodo na outra ponta do link ligado a este nodo reporte HELLO_ACK (em tempo absoluto simtime())
};