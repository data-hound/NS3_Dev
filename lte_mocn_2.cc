/*
 *     "ASLTE" an extension of LENA it provides a MOCN
 *      implementation to test different kind of scenarios.
 *
 *     Copyright (C) 2015  University Of the West of Scotland
 *
 *     Authors
 *     -------
 *
 *     Jose Maria Alcaraz-Calero 	Jose.Alcaraz-Calero@uws.ac.uk
 *     Ricardo Marco Alaez		Ricardo.MarcoAlaez@uws.ac.uk
 *
 *
 *     "ASLTE" is free software: you can redistribute it
 *     and/or modify it under the terms of the GNU General Public License
 *     as published by the Free Software Foundation, either version 3 of the
 *     License, or (at your option) any later version.
 *
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *     Additional Terms of this License (availables in this distribution)
 *
 *     1. It is Required the preservation of specified reasonable legal notices
 *        and author attributions in that material and in the Appropriate Legal
 *        Notices displayed by works containing it.
 *
 *     2. It is limited the use for publicity purposes of names of licensors or
 *        authors of the material.
 *
 *     3. It is Required indemnification of licensors and authors of that material
 *        by anyone who conveys the material (or modified versions of it) with
 *        contractual assumptions of liability to the recipient, for any liability
 *        that these contractual assumptions directly impose on those licensors
 *        and authors.
 *
 *     4. It is Prohibed misrepresentation of the origin of that material, and it is
 *        required that modified versions of such material be marked in reasonable
 *        ways as different from the original version.
 *
 *     5. It is Declined to grant rights under trademark law for use of some trade
 *        names, trademarks, or service marks.
 *
 */

#include "ns3/as-lte-helper.h"
#include "ns3/as-p2p-epc-helper.h" 
#include "ns3/epc-helper.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/lte-module.h"
#include "ns3/applications-module.h"
#include <ns3/config-store-module.h>
#include "ns3/flow-monitor-module.h"
#include <unistd.h>
#include "ns3/netanim-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("AS");

// GLOBAL VARS
int counteripaddrss = 1;
int countermacaddrss = 1;
int countervethinf = 0;

void CreateVeth(int index, char *VethNames[]) {
  char *filename; // final file name

  for(uint16_t i = 0 ; i < index ; i++){  
        filename  = (char *)malloc(18);
	sprintf(filename,"veth%i",   countervethinf);
	VethNames[i]=filename;
	countervethinf++;
  }
}

void CreateMACAddress(int index, char *MacAddrss[]) {
  char *filename; // final file name

  for(uint16_t i = 0 ; i < index ; i++){  
        filename  = (char *)malloc(18);
        int round = countermacaddrss /99;
	if ( round <= 0)
	 sprintf(filename,"00:00:00:AA:AA:%i",   countermacaddrss);
	else 
  	 sprintf(filename,"00:00:00:AA:%i:%i",   round+1, countermacaddrss % 99);
	MacAddrss[i]=filename;
	countermacaddrss++;
  }
}

void CreateIpAddress(int index, char *IpAddrss[]) {
  char *filename; // final file name

  for(uint16_t i = 0 ; i < index ; i++){  
        filename  = (char *)malloc(16);
        int round = counteripaddrss /240;
   
        if ( round <= 0)
	 sprintf(filename,"1.%i.0.0",   counteripaddrss);
        else{
         sprintf(filename,"%i.%i.0.0",   round+1, counteripaddrss % 240);
	}
         
	IpAddrss[i]=filename;
	counteripaddrss++;
  }
}

void CreateMask(uint16_t NumberOfNodes, const char *Mask[], const char* MaskType){

  for(uint16_t i = 0 ; i < NumberOfNodes ; i++){  
	Mask[i]=MaskType;
  }
}

void CreatePlmns(uint16_t NumberOfNodes, uint16_t plmnids[]){

  for(uint16_t i = 0 ; i < NumberOfNodes ; i++){  
	plmnids[i]=i;
  }
}

int main (int argc, char *argv[]) {

 //LOCAL VARS
 double distanceenbs;

 // CONFIG PARAMS
  double interPacketInterval = 10;
  double simTime = 2.1;
  int startTime = 0.1;
  int incrementTime = 0.01;
  double srsPeriodicity = 320;
  uint64_t gbrDl = 20000000; //20Mbps 20-50
  uint64_t gbrUl = 20000000; //20Mbps
  uint64_t mbrDl = 50000000; //50Mbps
  uint64_t mbrUl = 50000000; //50Mbps
  std::string Scheduler = "ns3::PssFfMacScheduler";
  std::string S1uLinkDataRate = "1Gb/s";
  uint16_t S1uLinkDelay = 1; //ms
  uint16_t S1uLinkMtu = 2000;
  std::string X2LinkDataRate = "1Gb/s";
  uint16_t X2LinkDelay = 0;
  uint16_t X2LinkMtu = 2000;
  bool shareInternet = false;
  bool internaltraffic = false;
  bool shownetwork = true;  
  bool logenabled = false;  
  double txpowerue = 20; //20 db
  double txpowerenb = 50; //50 db
  uint64_t LTEDataRateDl = 25; //25 Resource Block
  uint64_t LTEDataRateUl = 25; //25 Resource Block
  
  double distanceues = 1;
  bool loadbalancing = true;
  uint16_t plmns =4; // NUMBER OF OPERATOR
  uint16_t remotehosts = 1; // NUMBER OF REMOTE HOSTS
  uint16_t NenbNode = 1;    // NUMBER OF ENBS (MIN = PLMNS)--> MIN=PLMNS????---->MIN = 1 if PLMNS<512 else PLMNS/512
  uint16_t NenbConnections = 1 ;     // (MIN = 1, MAX=PLMNS)
  uint16_t NueNode = 512	;     // NUMBER OF UES
  std::string FileName="output";

// CONST
  char const *sgwtoremotehostmask = "255.255.0.0"; 
  
 // Command line arguments
  CommandLine cmd;
  cmd.AddValue("operator", "Number of PLMNs", plmns);
  cmd.AddValue("remoteHosts", "Number of Remote Hosts", remotehosts);
  cmd.AddValue("enbs", "Number of eNodeBs", NenbNode);
  cmd.AddValue("connections", "Number of connections per ENB", NenbConnections);
  cmd.AddValue("ues", "Number of UEs", NueNode);
  cmd.AddValue("simTime", "Total duration of the simulation [s])", simTime);
  cmd.AddValue("distanceues", "Distance between UEs [m]", distanceues);
  cmd.AddValue("interpacketinterval", "Applicaiton - Inter packet interval [ms])", interPacketInterval);
  cmd.AddValue("startTime", "Time Where the First Applications Starts [s])", startTime);
  cmd.AddValue("incrementTime", "Interval between Starting Applications [s])", incrementTime);
  cmd.AddValue("Scheduler", "Scheduler to use", Scheduler);
  cmd.AddValue("gbrDl", "Minimum guaranteed bit rate per EPS bearer (download)", gbrDl);
  cmd.AddValue("gbrUl", "Minimum guaranteed bit rate per EPS bearer (upload)", gbrUl);
  cmd.AddValue("mbrDl", "Maximum guaranteed bit rate per EPS bearer (download)", mbrDl);
  cmd.AddValue("mbrUl", "Maximum guaranteed bit rate per EPS bearer (upload)", mbrUl);
  cmd.AddValue("S1uLinkDataRate", "S1U Data Rate  [10Gbs/s...] ", S1uLinkDataRate);
  cmd.AddValue("S1uLinkDelay", "S1U Delay [ms]", S1uLinkDelay);
  cmd.AddValue("S1uLinkMtu", "S1U MTU [bytes]", S1uLinkMtu);
  cmd.AddValue("X2LinkDataRate", "X2 Data Rate [10Gbs/s]", X2LinkDataRate);
  cmd.AddValue("X2LinkDelay", "X2 Delay [ms]", X2LinkDelay);
  cmd.AddValue("X2LinkMtu", "X2 MTU [bytes]", X2LinkMtu);
  cmd.AddValue("srsPeriodicity", "At least grather than the maximum number of UE per EnB. Max 320", srsPeriodicity); 
  cmd.AddValue("shareInternet", "Link Between SGWs)", shareInternet);
  cmd.AddValue("logenabled", "Logging Information Enabled", logenabled);
  cmd.AddValue("shownetwork", "Show Network Interfaces Logging Information", shownetwork);
  cmd.AddValue("internaltraffic", "Internal Traffic Between Operators Enabled", internaltraffic);
  cmd.AddValue("txpowerenb", "Tx Power ENBs [db]", txpowerenb);
  cmd.AddValue("txpowerue", "Tx Power UWs [db]", txpowerue);
  cmd.AddValue("LTEDataRateDl", "LTE Data Rate [RBs] (download)", LTEDataRateDl);
  cmd.AddValue("LTEDataRateUl", "LTE Data Rate [RBs] (upload)", LTEDataRateUl);
  cmd.AddValue("FileName", "Filename for the results", FileName);

  
// FIXED PARAM
  GlobalValue::Bind ("SimulatorImplementationType", StringValue ("ns3::RealtimeSimulatorImpl"));
  Config::SetDefault ("ns3::LteSpectrumPhy::CtrlErrorModelEnabled", BooleanValue (true));
  Config::SetDefault ("ns3::LteSpectrumPhy::DataErrorModelEnabled", BooleanValue (true));  
  Config::SetDefault ("ns3::LteEnbRrc::SrsPeriodicity", UintegerValue (srsPeriodicity));
  Config::SetDefault ("ns3::ASLteHelper::UseIdealRrc", BooleanValue (false));  //FALSE=RealRRC Activated - TRUE=NO-Activated
  GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));

  Config::SetDefault ("ns3::ASP2PEpcHelper::S1uLinkDataRate", DataRateValue (DataRate (S1uLinkDataRate)));
  Config::SetDefault ("ns3::ASP2PEpcHelper::S1uLinkDelay", TimeValue (Seconds (S1uLinkDelay)));
  Config::SetDefault ("ns3::ASP2PEpcHelper::S1uLinkMtu", UintegerValue (S1uLinkMtu));
  Config::SetDefault ("ns3::ASP2PEpcHelper::X2LinkDataRate", DataRateValue (DataRate (X2LinkDataRate)));
  Config::SetDefault ("ns3::ASP2PEpcHelper::X2LinkDelay", TimeValue (Seconds (X2LinkDelay)));
  Config::SetDefault ("ns3::ASP2PEpcHelper::X2LinkMtu", UintegerValue (X2LinkMtu));
  Config::SetDefault ("ns3::ASLteEnbNetDevice::UlBandwidth", UintegerValue (LTEDataRateUl)); 
  Config::SetDefault ("ns3::ASLteEnbNetDevice::DlBandwidth", UintegerValue (LTEDataRateDl)); 
  Config::SetDefault ("ns3::ASLteEnbPhy::TxPower", DoubleValue (txpowerenb)); 
  Config::SetDefault ("ns3::LteUePhy::TxPower", DoubleValue (txpowerue)); 

  ConfigStore inputConfig;
  inputConfig.ConfigureDefaults();


  cmd.Parse(argc, argv);

  if (logenabled){
   LogComponentEnable ("PacketSink", LOG_LEVEL_ALL);
   LogComponentEnable ("ASLteEnbRrc", LOG_LEVEL_ALL);
   LogComponentEnable ("LteUeRrc", LOG_LEVEL_INFO);
   LogComponentEnable ("ASEpcEnbApplication", LOG_LEVEL_ALL);
   LogComponentEnable ("ASEpcSgwPgwApplication", LOG_LEVEL_ALL);
  }

  int epo = NenbNode / plmns;//--Enbs Per Operator
  if ( epo > 0)
     NenbConnections = NenbConnections * NenbNode;
  else
     NenbConnections = NenbConnections * plmns;

  remotehosts = remotehosts * plmns;
  int cpe = NenbConnections / NenbNode;//--Connections Per Enb
  int cpo = NenbConnections / plmns;//--Connections Per Operator
  int upe = NueNode / NenbNode;//--Users Per Enb
  int rpo = remotehosts / plmns;//--Remote Hosts Per Operator
  int upo = NueNode / plmns;//--Users Per Operator
  int upr = NueNode / remotehosts;///--Users Per Remote Host
  
  std::cout << "CPE = " << cpe <<std::endl;
  std::cout << "CPO = " << cpo <<std::endl;
  std::cout << "UPE = " << upe <<std::endl;
  std::cout << "RPO = " << rpo <<std::endl;
  std::cout << "UPO = " << upo <<std::endl;
  std::cout << "UPR = " << upr <<std::endl;
  std::cout << "EPO = " << epo <<std::endl;

  // ERROR CHECKING 
  if (remotehosts < plmns){
    std::cout << "remotehosts < plmns - Currently not supported" << std::endl;
    return 0;
  }

if (remotehosts > NueNode){
    std::cout << "remotehosts > NueNode - Currently not supported. It does not make sense." << std::endl;
    return 0;
} 

  if (remotehosts > NueNode){
    std::cout << "remotehosts > ues - Currently not supported" << std::endl;
    return 0;
  }

  if (NenbNode > NueNode){
    std::cout << "enbs > ues - Currently not supported" << std::endl;
    return 0;
  }

  if (epo > 0){
   if (NenbConnections/NenbNode > NenbNode){
      std::cout << "EPO>0, NenbConnections > plmns - Currently not supported" << std::endl;
      return 0;
    }
  }else{
   if (NenbConnections/plmns > plmns){
      std::cout << "EPO<0, NenbConnections > NenbNode - Currently not supported" << std::endl;
      return 0;
   }
 }

 if (plmns > NueNode){
    std::cout << "plmns > ues - Currently not supported" << std::endl;
    return 0;
  }

 if (cpe > plmns){
    std::cout << "cpe > plmns - Currently not supported" << std::endl;
    return 0;
  }

  if (NenbNode <= 0 || NueNode <=0 || remotehosts <= 0 || plmns <= 0 || NenbConnections <= 0 ){
    std::cout << "enbs, ues, remote hsots, plmns and connects cannot be 0." << std::endl;
    return 0;
  }
  if (cpo <= 0){
    std::cout << "cpo < 0 - Currently not supported" << std::endl;
    return 0;
  }

  char *ueaddress[plmns];
  char *epcipaddress[NenbConnections];
  char *x2ipaddress[plmns];
  char *remotehostaddress[remotehosts];
  const char *remotehostmask[remotehosts];
  const char *uemasknetwork[plmns];
  char *sgwtosgwaddress[1]; // Used for Connecting Operators via Internet. 
  uint16_t plmnids[plmns];
  char *sgwMacAddress[plmns];
  char *sgweth[plmns];
  char *enbeth[NenbNode];
  char* enbMacAddress[NenbNode]; 

  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;  // define Qci type
  GbrQosInformation qos;
  qos.gbrDl = gbrDl; // Downlink GBR
  qos.gbrUl = gbrUl; // Uplink GBR
  qos.mbrDl = mbrDl; // Downlink MBR
  qos.mbrUl = mbrUl; // Uplink MBR
  EpsBearer bearer (q, qos);


  // INITIALIZATION

  CreateIpAddress(plmns, ueaddress);
  CreateIpAddress(NenbConnections, epcipaddress);
  CreateIpAddress(remotehosts, remotehostaddress);
  CreateIpAddress(1, sgwtosgwaddress);
  CreateIpAddress(plmns, x2ipaddress);

  CreateMask(remotehosts, remotehostmask, "255.255.0.0");
  CreateMask(plmns, uemasknetwork, "255.255.0.0");

  CreatePlmns(plmns, plmnids);

  CreateMACAddress(plmns, sgwMacAddress);
  CreateVeth(plmns,sgweth);
  
  CreateVeth(NenbNode,enbeth);
  CreateMACAddress(NenbNode, enbMacAddress);


  NodeContainer remoteHostContainer;  
  NodeContainer enbNodes; 
  NodeContainer ueNodes;
  Ipv4Address remoteHostAddr[remotehosts]; // ONE PER EACH PLMN
  Ipv4InterfaceContainer ueIpIface[NueNode]; // ONE PER EACH UE
  ASP2PEpcHelper::EnbInf enbInf[NenbNode]; // ONE PER EACH ENB
  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  InternetStackHelper internet;
  Ptr<Node> pgw[plmns];
  
  // INITIALIZATION--*****LTE HELPER CREATED HERE********
  Ptr<ASLteHelper> lteHelper = CreateObject<ASLteHelper> ();
  Ptr<ASP2PEpcHelper> epcHelper = CreateObject<ASP2PEpcHelper> ();
  lteHelper->SetEpcHelper (epcHelper);
  lteHelper->SetSchedulerType (Scheduler); 


    // CREATE OPERATORS (SGW + MME)
  std::cout << "CREATING OPERATORS. N="<< plmns << std::endl;
  for(uint16_t u = 0 ; u < plmns ; u++){ 
    plmnids[u] = epcHelper->AddNewOperator(ueaddress[u], uemasknetwork[u], sgweth[u], sgwMacAddress[u], x2ipaddress[u]);
    pgw[u] = epcHelper->GetPgwNode (plmnids[u]);
  }

  // CREATE REMOTE HOST
  remoteHostContainer.Create (remotehosts);
  internet.Install (remoteHostContainer);
  
  // CREATION OF ENBs
  enbNodes.Create(NenbNode);

  // CREATION OF UEs
  ueNodes.Create(NueNode);

  // Install Mobility Model for ENBs and UEs
  distanceenbs  = upe * distanceues; 
  std::cout << "INSTALLING MOBILITY ..." << std::endl;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  for (uint16_t i = 0; i < NenbNode; i++){
      positionAlloc->Add (Vector((distanceenbs * i) + (upe/2)*distanceues, 1, 0)); // ENB IN THE MIDDLE OF THE NODES ATTACHED START TOPOLOGY
      std::cout << "ENB= " << i << " LOCATED AT ("<< (distanceenbs * i) + (upe/2) << "," << 1 << "," << 0 << ")" << std::endl;
  }

 int enb=0; 
 int counter = 0;
 for ( uint16_t u = 0; u < NueNode;) {
    Ptr<Node> ueNode = ueNodes.Get (u);
     int interspace = 0;
     for(uint16_t i = 0 ; i < upe ; i++){  
if (plmns==1){
      if(i >= 160){
	    positionAlloc->Add (Vector(counter, 5, 0));
            std::cout << "UE= " << u << " LOCATED AT ("<< counter<< "," << 5 << "," << 0 << ")" <<std::endl;
	    counter++;
	}

      if(i < 160){
      positionAlloc->Add (Vector(i, -10, 0));
      std::cout << "UE= " << u << " LOCATED AT ("<< i<< "," << -5 << "," << 0 << ")" <<std::endl;
      }
}
else{
      positionAlloc->Add (Vector((distanceenbs * enb) + interspace, 0, 0));
      std::cout << "UE= " << u << " LOCATED AT ("<< distanceenbs * enb  + interspace<< "," << 0 << "," << 0 << ")" <<std::endl;
}	
      interspace = interspace + distanceues; 
      u++;
     }
     enb++;  
 }

  MobilityHelper mobility;
  mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator(positionAlloc);
  mobility.Install(enbNodes);
  mobility.Install(ueNodes);

  std::cout << "CONFIGURING ENBS. N="<< NenbNode << std::endl;
  //==========================================================================================
  //Code to enable CSG-Id-> The distinguishing factor to identify the node as a femtocell node
  //as given in Lena-Dual-Stripe Example (approx around line:600)
  //Added on 16/8
  //==========================================================================================
  lteHelper->SetEnbDeviceAttribute ("CsgId", UintegerValue (1));
  lteHelper->SetEnbDeviceAttribute ("CsgIndication", BooleanValue (true));
  //==========================================================================================
  NetDeviceContainer enbLteDevs = lteHelper->InstallEnbDevice (enbNodes);

  for(uint16_t u = 0 ; u < NenbNode ; u++){
    Ptr<Node> enbNode = enbNodes.Get (u);	
    enbInf[u] = epcHelper->AddNewEnodeB (enbNode, enbLteDevs.Get(u), enbLteDevs.Get(u)->GetObject<ASLteEnbNetDevice> ()->GetCellId ());
  }
  
 
    std::cout << "ASSIGNING ENBS TO OPERATORS. N CONECTIONS="<< NenbConnections << std::endl;
     int conn =0;
      if(epo > 0){
        for(uint16_t u = 0 ; u < NenbNode ; ){
         for(uint16_t c = 0 ; c < cpe ; c++){
          std::cout << "EPO>0, ENB = " << u << " CONN ID - " << conn  <<" TO OPERATOR = " << (u+c) % plmns << std::endl;
          Ptr<Node> enbNode = enbNodes.Get (u);
          epcHelper->AssignEnbToOperator(epcipaddress[conn], enbNode, enbLteDevs.Get(u), enbInf[u].lteSocket, enbLteDevs.Get(u)->GetObject<ASLteEnbNetDevice> ()->GetCellId (), enbeth[u], enbMacAddress[u], enbInf[u].enbApp, (u+c) % plmns);
          conn++;
         }
        u++;
        }
      }else{
       // epo < 0
        for(uint16_t p = 0 ; p < plmns ; ){
          for(uint16_t c = 0 ; c < cpo ; c++){
           int enb = p % NenbNode;
           std::cout << "EPO<0, ENB = " << enb << " CONN ID - " << conn  <<" TO OPERATOR = " << (p+c) % plmns << std::endl;
           Ptr<Node> enbNode = enbNodes.Get (enb);
           epcHelper->AssignEnbToOperator(epcipaddress[conn], enbNode, enbLteDevs.Get(enb), enbInf[enb].lteSocket, enbLteDevs.Get(enb)->GetObject<ASLteEnbNetDevice> ()->GetCellId (), enbeth[enb], enbMacAddress[enb], enbInf[enb].enbApp, (p+c) % plmns); 
           conn++;
          }
          p++;
        }
      }

   std::cout << "INSTALLING LTE IN UE. N="<< NueNode<< std::endl;
   
   /*=====CONFIGURING UEs TO BE ABLE TO BE CONNECTED TO hENBs========
   
   // set the home UE as a CSG member of the home eNodeBs
   
   */
   lteHelper->SetUeDeviceAttribute ("CsgId", UintegerValue (1));
   
   
   
   NetDeviceContainer ueLteDevs;
   for(uint16_t u = 0 ; u < NueNode ; u++){
    Ptr<Node> ueNode = ueNodes.Get (u);
    ueLteDevs.Add (lteHelper->InstallSingleUeDevice (ueNode));   
   }
   internet.Install (ueNodes);

  std::cout << "ASSING UE TO OPERATOR ..." << std::endl;
  if ( epo > 0 ){
   // EPO > 0
   for ( uint16_t u = 0; u < NueNode; ) {
    
    int iterator = 0; 
    if ( cpe < upe)
     iterator = cpe;
    else
     iterator = upe;
  
    for ( uint16_t c = 0; c < iterator; c++) { // Conns per ENB
     int enb = u / upe;        
     int assignedOperator = (enb+c) % plmns;
     Ptr<Node> ueNode = ueNodes.Get (u);
     epcHelper->AddUePlmn (ueLteDevs.Get(u), assignedOperator);
     Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ueNode->GetObject<Ipv4> ());
     ueIpIface[u] = epcHelper->AssignUeIpv4Address (ueLteDevs.Get(u), assignedOperator);
     ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (assignedOperator), 1);
     std::cout << "EPO >0 UE  = " << u << " TO OPERATOR  = " << assignedOperator << std::endl;                
     std::cout << "EPO >0 ASSIGN IP AND DEFAULT GW TO UE = " << u << " FOR PLMN  = " <<  assignedOperator << std::endl;
     u++;
    }
   }
  }else{
  // EPO < 0
   for ( uint16_t u = 0; u < NueNode; ) {
   int iterator = 0;
    if ( cpe < upe)
     iterator = cpe;
    else
     iterator = upe;
     
    for(uint16_t c = 0 ; c < iterator ; c++){
        Ptr<Node> ueNode = ueNodes.Get (u);
        int connused = c / cpo;
        int internal = c % cpo;
        int enbAttached = u / upe;
  
        int assignedOperator = (enbAttached + internal+ (connused * NenbNode) ) % plmns;
           epcHelper->AddUePlmn (ueLteDevs.Get(u), assignedOperator);
           Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ueNode->GetObject<Ipv4> ());
           ueIpIface[u] = epcHelper->AssignUeIpv4Address (ueLteDevs.Get(u), assignedOperator);
           ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (assignedOperator), 1);
           std::cout << "EPO <0 UE  = " << u << " TO OPERATOR  = " << assignedOperator << std::endl;                
           std::cout << "EPO <0  ASSIGN IP AND DEFAULT GW TO UE = " << u << " FOR PLMN  = " <<  assignedOperator << std::endl;
           u++;
          }
      
   }
  }
  
  std::cout << "CONNECTING REMOTE HOSTS TO OPERATOR ..." << std::endl;
  int plmn=0;
  for(uint16_t u = 0 ; u < remotehosts ; ){   
      for(uint16_t i = 0 ; i < rpo ; i++){   
       Ptr<Node> remoteHost = remoteHostContainer.Get (u);  
       ASP2PEpcHelper::SgwAddresses sgwAddresses;
       sgwAddresses = epcHelper->ConnectRemoteHostToSGW(pgw[plmn], remoteHost, remotehostaddress[u], remotehostmask[u]);
       remoteHostAddr[u] =  sgwAddresses.sgwDestinationAddr;
       // Route by default to reach REmote Host
       std::cout << " REMOTE HOST  = " << u << " TO OPERATOR  = " << plmn << " ADDRESS " << remoteHostAddr[u] << " DEFAULT GW IP " << sgwAddresses.sgwSourceAddr << std::endl;
       Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting (remoteHost->GetObject<Ipv4> ());
       remoteHostStaticRouting->SetDefaultRoute (sgwAddresses.sgwSourceAddr, 1);
       u++;
      }
      plmn++;
     
  }

 std::cout << "ATTACH UE TO ENBS ..." << std::endl;
 enb=0;  
 for ( uint16_t u = 0; u < NueNode;) {
    Ptr<Node> ueNode = ueNodes.Get (u);
     for(uint16_t i = 0 ; i < upe ; i++){  
      std::cout << " ATTACH UE NUMBER = " << u << " TO ENB = " <<  enb << std::endl;
      epcHelper->Attach (ueLteDevs.Get(u), enbLteDevs.Get (enb)); 
      u++;
     }
     enb++;
 }

if (shareInternet){
  Ipv4Address sgwToSgwAddresses[plmns][plmns];
  epcHelper->ConfigureSGWToSGWNetworkAddress(sgwtosgwaddress[0], sgwtoremotehostmask);
  std::cout << "CONNECTING SGWS TOGETHER ..." << std::endl;
  for(uint16_t u = 0 ; u < plmns-1 ; u++){   
     Ptr<Node> pgwSource = pgw[u];
     for(uint16_t i = u+1 ; i < plmns ; i++){   
       Ptr<Node> pgwDestination = pgw[i];
       ASP2PEpcHelper::SgwAddresses sgwAddresses;
       sgwAddresses = epcHelper->ConnectSGWToSGW(pgwSource, pgwDestination);
       sgwToSgwAddresses[u][i] = sgwAddresses.sgwDestinationAddr;
       sgwToSgwAddresses[i][u] = sgwAddresses.sgwSourceAddr;
     }
  }

  std::cout << "CONFIGURE STATIC ROUTING FOR SGWS ..." << std::endl;
  for(uint16_t u = 0 ; u < plmns ; u++){   
     Ptr<Node> pgwSource = pgw[u];
    for(uint16_t i = 0 ; i < plmns ; i++){   
     Ptr<Node> pgwDestination = pgw[i];
     if ( u != i ){
       Ptr<Ipv4StaticRouting> sgwSourceStaticRouting = ipv4RoutingHelper.GetStaticRouting (pgwSource->GetObject<Ipv4> ());
       int iface = epcHelper->GetInterfaceIndexByIPAddress(pgwSource,sgwToSgwAddresses[i][u]);
       if (iface == -1){
        iface = epcHelper->GetInterfaceIndexByIPAddress(pgwSource,sgwToSgwAddresses[u][i]);
        if (iface != -1){ 
         sgwSourceStaticRouting->AddNetworkRouteTo (Ipv4Address (ueaddress[i % plmns]), Ipv4Mask (uemasknetwork[i % plmns]),sgwToSgwAddresses[i][u],  iface);
        }else{
         std::cout << "SOMETHING IS WRONG IN THE SGW STATIC ROUTES ...." << std::endl;
        }
       }else{
         sgwSourceStaticRouting->AddNetworkRouteTo (Ipv4Address (ueaddress[i % plmns]), Ipv4Mask (uemasknetwork[i % plmns]),sgwToSgwAddresses[u][i],  iface);
       }
     }
    }
  }
}
lteHelper->ActivateDedicatedEpsBearer (ueLteDevs, bearer, EpcTft::Default ());
if (shownetwork){
  ///// DEBUG DE EVERY INTERFACES
   std::cout << " /////////////////// START IP INTERFACES AND ROUTES ///////////////////" << std::endl;
   for(uint16_t u = 0 ; u < plmns ; u++){      
    Ptr<Node> netdevice = epcHelper->GetPgwNode (u);
    
    std::cout << "SGW FOR PLMN " << u << " has " << netdevice->GetObject<Ipv4> ()->GetNInterfaces () << " INTERFACES" << " NODE ID= " << netdevice->GetId() << std::endl;
    for(uint16_t i = 0 ; i < netdevice->GetObject<Ipv4> ()->GetNInterfaces () ; i++){ 
      for(uint16_t j = 0 ; j < netdevice->GetObject<Ipv4> ()->GetNAddresses (i) ; j++){ 
          Ipv4Address address = netdevice->GetObject<Ipv4> ()->GetAddress (i, j).GetLocal();
          Ipv4Mask 	 mask = netdevice->GetObject<Ipv4> ()->GetAddress (i, j).GetMask();
          std::cout << "INTERFACE =" << i << " : ADDRESS=" << j << " : IP=" << address << " MASK=" << mask << std::endl;
      }
    }
    
    Ptr<Ipv4StaticRouting> netdeviceStaticRouting = ipv4RoutingHelper.GetStaticRouting (netdevice->GetObject<Ipv4> ()); 
     for(uint16_t i = 0 ; i < netdeviceStaticRouting->GetNRoutes () ; i++){ 
         std::cout << "GW N= " << u << " ROUTE N="<< i << " ROUTE=" << netdeviceStaticRouting->GetRoute (i) << std::endl;
     }
  }
  
    for(uint16_t u = 0 ; u < remotehosts ; u++){   
     Ptr<Node> netdevice = remoteHostContainer.Get (u);  
    
    std::cout << "REMOTE HOST N=" << u << " has " << netdevice->GetObject<Ipv4> ()->GetNInterfaces () << " INTERFACES" << " NODE ID= " << netdevice->GetId()  << std::endl;
    for(uint16_t i = 0 ; i < netdevice->GetObject<Ipv4> ()->GetNInterfaces () ; i++){ 
      for(uint16_t j = 0 ; j < netdevice->GetObject<Ipv4> ()->GetNAddresses (i) ; j++){ 
          Ipv4Address address = netdevice->GetObject<Ipv4> ()->GetAddress (i, j).GetLocal();
         Ipv4Mask 	 mask = netdevice->GetObject<Ipv4> ()->GetAddress (i, j).GetMask();
          std::cout << "INTERFACE =" << i << " : ADDRESS=" << j << " : IP=" << address << " MASK=" << mask << std::endl;
 
      }
    }
    Ptr<Ipv4StaticRouting> netdeviceStaticRouting = ipv4RoutingHelper.GetStaticRouting (netdevice->GetObject<Ipv4> ()); 
     for(uint16_t i = 0 ; i < netdeviceStaticRouting->GetNRoutes () ; i++){ 
         std::cout << "REMOTEHOST N= " << u << " ROUTE N="<< i << " ROUTE=" << netdeviceStaticRouting->GetRoute (i) << std::endl;
     }
  }
  
    for(uint16_t u = 0 ; u < NenbNode ; u++){
    Ptr<Node> netdevice = enbNodes.Get (u);
    
    std::cout << "ENB N= " << u << " has " << netdevice->GetObject<Ipv4> ()->GetNInterfaces () << " INTERFACES" << " NODE ID= " << netdevice->GetId() << std::endl;
    for(uint16_t i = 0 ; i < netdevice->GetObject<Ipv4> ()->GetNInterfaces () ; i++){ 
      for(uint16_t j = 0 ; j < netdevice->GetObject<Ipv4> ()->GetNAddresses (i) ; j++){ 
          Ipv4Address address = netdevice->GetObject<Ipv4> ()->GetAddress (i, j).GetLocal();
                Ipv4Mask 	 mask = netdevice->GetObject<Ipv4> ()->GetAddress (i, j).GetMask();
          std::cout << "INTERFACE =" << i << " : ADDRESS=" << j << " : IP=" << address << " MASK=" << mask << std::endl;
 
      }
    }
    
    Ptr<Ipv4StaticRouting> netdeviceStaticRouting = ipv4RoutingHelper.GetStaticRouting (netdevice->GetObject<Ipv4> ()); 
     for(uint16_t i = 0 ; i < netdeviceStaticRouting->GetNRoutes () ; i++){ 
         std::cout << "ENB N= " << u << " ROUTE N="<< i << " ROUTE=" << netdeviceStaticRouting->GetRoute (i) << std::endl;
     }
    
  }
  
  for ( uint16_t u = 0; u < NueNode; u++) {
    Ptr<Node> netdevice = ueNodes.Get (u);
    
    std::cout << "UE N= " << u << " has " << netdevice->GetObject<Ipv4> ()->GetNInterfaces () << " INTERFACES" << " NODE ID= " << netdevice->GetId() << std::endl;
    for(uint16_t i = 0 ; i < netdevice->GetObject<Ipv4> ()->GetNInterfaces () ; i++){ 
      for(uint16_t j = 0 ; j < netdevice->GetObject<Ipv4> ()->GetNAddresses (i) ; j++){ 
          Ipv4Address address = netdevice->GetObject<Ipv4> ()->GetAddress (i, j).GetLocal();
         Ipv4Mask 	 mask = netdevice->GetObject<Ipv4> ()->GetAddress (i, j).GetMask();
          std::cout << "INTERFACE =" << i << " : ADDRESS=" << j << " : IP=" << address << " MASK=" << mask << std::endl;
 
      }
    }
    
    Ptr<Ipv4StaticRouting> netdeviceStaticRouting = ipv4RoutingHelper.GetStaticRouting (netdevice->GetObject<Ipv4> ()); 
     for(uint16_t i = 0 ; i < netdeviceStaticRouting->GetNRoutes () ; i++){ 
         std::cout << "UE N= " << u << " ROUTE N="<< i << " ROUTE=" << netdeviceStaticRouting->GetRoute (i) << std::endl;
     }
  }
  
    std::cout << " /////////////////// END IP INTERFACES ///////////////////" << std::endl;
 }
 
   std::cout << "INSTALLING APPLICATIONS ..." << std::endl;
  // Install and start applications on UEs and remote host
  uint16_t dlPort = 1234;
  uint16_t ulPort = 2000;
  uint16_t otherPort = 3000;
  ApplicationContainer clientApps[NueNode];
  ApplicationContainer serverApps[NueNode];


 int balance[plmns];
 for ( int i = 0 ; i < plmns ; i++){
  balance[i] = 0;
 }

 for ( uint16_t u = 0; u < NueNode; u++) {
   int rhdestination = 0;
   int uedestination = 0;

   int iterator = 0; 
   if ( cpe < upe)
    iterator = cpe;
   else
    iterator = upe;

   if ( epo > 0 ){
    // EPO > 0
  
     int ceb = u % iterator;// what is ceb?
     int connused = ceb / cpe;// why is the connection used = ceb/cpe
     int internal = ceb % cpe;//why is the connection used = ceb%cpe
     int enb = u / upe;  //why       

     int assignedOperator = (enb + internal+ (connused * NenbNode) ) % plmns;
     Ptr<Node> ueNode = ueNodes.Get (u);
     rhdestination = assignedOperator*rpo;     
     if (loadbalancing){
      rhdestination = rhdestination + (balance[assignedOperator] % rpo); 
      balance[assignedOperator]=balance[assignedOperator]+1;
     }

   }else{
    // EPO < 0
     int ceb = u % iterator;
     int connused = ceb / cpo;
     int internal = ceb % cpo;
     int enb = u / upe;        

     int assignedOperator = (enb + internal+ (connused * NenbNode) ) % plmns;

     Ptr<Node> ueNode = ueNodes.Get (u);
     rhdestination =assignedOperator*rpo;     
     if (loadbalancing){
      rhdestination = rhdestination + (balance[assignedOperator] % rpo); 
      balance[assignedOperator]=balance[assignedOperator]+1;
     }

   }

   ++ulPort;
   ++otherPort;
   PacketSinkHelper dlPacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), dlPort));
   PacketSinkHelper ulPacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), ulPort));
      
   serverApps[u].Add (dlPacketSinkHelper.Install (ueNodes.Get(u)));
   serverApps[u].Add (ulPacketSinkHelper.Install (remoteHostContainer.Get(rhdestination)));

   std::cout << " APPS: UE <-> REMOTE HOST " << ueIpIface[u].GetAddress (0) << " <-> " << remoteHostAddr[rhdestination]<< " UE ID " << u << "<-> RH ID="<< rhdestination << std::endl;

   UdpClientHelper dlClient (ueIpIface[u].GetAddress (0), dlPort);
   dlClient.SetAttribute ("Interval", TimeValue (MilliSeconds(interPacketInterval)));
   dlClient.SetAttribute ("MaxPackets", UintegerValue(1000000));

   UdpClientHelper ulClient (remoteHostAddr[rhdestination], ulPort);
   ulClient.SetAttribute ("Interval", TimeValue (MilliSeconds(interPacketInterval)));
   ulClient.SetAttribute ("MaxPackets", UintegerValue(1000000));

   clientApps[u].Add (dlClient.Install (remoteHostContainer.Get(rhdestination)));
   clientApps[u].Add (ulClient.Install (ueNodes.Get(u)));

   serverApps[u].Start (Seconds (startTime));
   clientApps[u].Start (Seconds (startTime));

   if (internaltraffic){
     PacketSinkHelper packetSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), otherPort));            
     serverApps[u].Add (packetSinkHelper.Install (ueNodes.Get(u)));
     UdpClientHelper client (ueIpIface[u].GetAddress (0), otherPort);
     client.SetAttribute ("Interval", TimeValue (MilliSeconds(interPacketInterval)));
     client.SetAttribute ("MaxPackets", UintegerValue(1000000));
     std::cout << " APPS: UE <-> UE " << ueIpIface[u].GetAddress (0) << " <-> " << ueIpIface[uedestination].GetAddress (0) << std::endl;
     clientApps[u].Add (client.Install (ueNodes.Get(uedestination)));
    }          
    startTime=startTime+incrementTime;
  }
  
     
  // Install FlowMonitor on all nodes
  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll ();
  
//  lteHelper->EnableTraces ();

  AnimationInterface anim("lte_mocn_1.xml");
  


/*  for (uint16_t i = 0; i < NenbNode; i++){
      
      anim.SetConstantPosition(enbNodes.Get(i),(distanceenbs * i) + (upe/2),0);
  }

  enb=0; 
  counter = 0;
 for ( uint16_t u = 0; u < NueNode;) {
    Ptr<Node> ueNode = ueNodes.Get (u);
     int interspace = 0;
     for(uint16_t i = 0 ; i < upe ; i++){  
if (plmns==1){
      if(i >= 160){
	    //positionAlloc->Add (Vector(counter, 5, 0));
            //std::cout << "UE= " << u << " LOCATED AT ("<< counter<< "," << 5 << "," << 0 << ")" <<std::endl;
            anim.SetConstantPosition(enbNodes.Get(i),counter,5);
	    counter++;
	}

      if(i < 160){
      //positionAlloc->Add (Vector(i, -10, 0));
      //std::cout << "UE= " << u << " LOCATED AT ("<< i<< "," << -5 << "," << 0 << ")" <<std::endl;
      anim.SetConstantPosition(enbNodes.Get(i),counter,-5);
      }
}
else{
      //positionAlloc->Add (Vector((distanceenbs * enb) + interspace, 0, 0));
      //std::cout << "UE= " << u << " LOCATED AT ("<< distanceenbs * enb  + interspace<< "," << 0 << "," << 0 << ")" <<std::endl;
      anim.SetConstantPosition(enbNodes.Get(i),distanceenbs * enb  + interspace,0);
}	
      interspace = interspace + distanceues; 
      u++;
     }
     enb++;  
 }*/
 
 

  std::cout << "RUNNING EMULATION ..." << std::endl;  
  Simulator::Stop(Seconds(simTime));
  Simulator::Run();

   std::string xmlResult = FileName + ".xml";
   std::string txtResult = FileName + ".txt";

   //Print per flow statistics 
   monitor->CheckForLostPackets ();
   Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
   std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();

   AsciiTraceHelper asciiTHFlow;
   monitor->SerializeToXmlFile (xmlResult , true, true );
   Ptr<OutputStreamWrapper> flowStream = asciiTHFlow.CreateFileStream (txtResult);

  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
    {
          Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
            *flowStream->GetStream () << " " << " Flow " << i->first << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")" << std::endl;
            *flowStream->GetStream () <<  " " << " Tx Bytes: " << i->second.txBytes << " / Packet: " << i->second.txPackets << std::endl;
            *flowStream->GetStream () <<  " " << " Rx Bytes: " << i->second.rxBytes << " / Packet: " << i->second.rxPackets << std::endl;
            *flowStream->GetStream () <<  " " << " Throughput: " << i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds()-i->second.timeFirstTxPacket.GetSeconds()) / 1024  << " Kbps";
            *flowStream->GetStream () <<  " " << " Packet loss= " << ((i->second.txPackets-i->second.rxPackets)*1.0)/i->second.txPackets;
          uint32_t dropes = 0;
           for (uint32_t reasonCode = 0; reasonCode < i->second.packetsDropped.size (); reasonCode++)
           {
        	   dropes+= i->second.packetsDropped[reasonCode];
           }
           *flowStream->GetStream () << " Dropped packets: " << dropes << std::endl<< std::endl;
           //dropped are the same by lost,  dropped are categorized by reason
    }

 for (uint32_t u = 0; u < NueNode;u++){
  for ( uint16_t s = 0 ; s < serverApps[u].GetN() ; s++){
	  Ptr<PacketSink> sink1 = DynamicCast<PacketSink> (serverApps[u].Get (s));
	 std::cout << "Total Bytes Received by sink packet SERVER # "<<sink1->GetNode()->GetId()<<": " << sink1->GetTotalRx () << std::endl;
   }
  }

  Simulator::Destroy();
  std::cout << "END!" <<std::endl;
  
  return 0; 
}

