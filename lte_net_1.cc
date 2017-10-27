/*
*----------THE SCENARIO--------------
*A single FemtoCell Connected with multiple UEs and a HeNB-GW
*
*/

#include <bits/stdc++.h>
#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/mobility-module.h>
#include <ns3/lte-module.h>
#include "ns3/netanim-module.h"
#include <ns3/internet-module.h>
#include <ns3/config-store-module.h>
#include <ns3/buildings-module.h>
#include <ns3/point-to-point-helper.h>
#include <ns3/applications-module.h>
#include <ns3/log.h>
#include <iomanip>
#include <ios>
#include <string>
#include <vector>

using namespace ns3;

/*-----------------------VARIABLE/PARAMETER DECLARATION-------------------------*/
/*static ns3::GlobalValue g_nBlocks ("nBlocks",
                                   "Number of femtocell blocks",
                                   ns3::UintegerValue (1),
                                   ns3::MakeUintegerChecker<uint32_t> ());
*/
/*static ns3::GlobalValue g_nApartmentsX ("nApartmentsX",
                                        "Number of apartments along the X axis in a femtocell block",
                                        ns3::UintegerValue (10),
                                        ns3::MakeUintegerChecker<uint32_t> ());
*/
/*static ns3::GlobalValue g_nFloors ("nFloors",
                                   "Number of floors",
                                   ns3::UintegerValue (1),
                                   ns3::MakeUintegerChecker<uint32_t> ());
*/
/*static ns3::GlobalValue g_nMacroEnbSites ("nMacroEnbSites",
                                          "How many macro sites there are",
                                          ns3::UintegerValue (3),
                                          ns3::MakeUintegerChecker<uint32_t> ());
*/
/*static ns3::GlobalValue g_nMacroEnbSitesX ("nMacroEnbSitesX",
                                           "(minimum) number of sites along the X-axis of the hex grid",
                                           ns3::UintegerValue (1),
                                           ns3::MakeUintegerChecker<uint32_t> ());
*/
/*static ns3::GlobalValue g_interSiteDistance ("interSiteDistance",
                                             "min distance between two nearby macro cell sites",
                                             ns3::DoubleValue (500),
                                             ns3::MakeDoubleChecker<double> ());
*/
/*static ns3::GlobalValue g_areaMarginFactor ("areaMarginFactor",
                                            "how much the UE area extends outside the macrocell grid, "
                                            "expressed as fraction of the interSiteDistance",
                                            ns3::DoubleValue (0.5),
                                            ns3::MakeDoubleChecker<double> ());
*/
/*static ns3::GlobalValue g_macroUeDensity ("macroUeDensity",
                                          "How many macrocell UEs there are per square meter",
                                          ns3::DoubleValue (0.00002),
                                          ns3::MakeDoubleChecker<double> ());
*/
/*static ns3::GlobalValue g_homeEnbDeploymentRatio ("homeEnbDeploymentRatio",
                                                  "The HeNB deployment ratio as per 3GPP R4-092042",
                                                  ns3::DoubleValue (0.2),
                                                  ns3::MakeDoubleChecker<double> ());
*/
/*static ns3::GlobalValue g_homeEnbActivationRatio ("homeEnbActivationRatio",
                                                  "The HeNB activation ratio as per 3GPP R4-092042",
                                                  ns3::DoubleValue (0.5),
                                                  ns3::MakeDoubleChecker<double> ());
*/
/*static ns3::GlobalValue g_homeUesHomeEnbRatio ("homeUesHomeEnbRatio",
                                               "How many (on average) home UEs per HeNB there are in the simulation",
                                               ns3::DoubleValue (1.0),
                                               ns3::MakeDoubleChecker<double> ());
*/
/*static ns3::GlobalValue g_macroEnbTxPowerDbm ("macroEnbTxPowerDbm",
                                              "TX power [dBm] used by macro eNBs",
                                              ns3::DoubleValue (46.0),
                                              ns3::MakeDoubleChecker<double> ());
*/
static ns3::GlobalValue g_homeEnbTxPowerDbm ("homeEnbTxPowerDbm",
                                             "TX power [dBm] used by HeNBs",
                                             ns3::DoubleValue (20.0),
                                             ns3::MakeDoubleChecker<double> ());
/*static ns3::GlobalValue g_macroEnbDlEarfcn ("macroEnbDlEarfcn",
                                            "DL EARFCN used by macro eNBs",
                                            ns3::UintegerValue (100),
                                            ns3::MakeUintegerChecker<uint16_t> ());
*/
static ns3::GlobalValue g_homeEnbDlEarfcn ("homeEnbDlEarfcn",
                                           "DL EARFCN used by HeNBs",
                                           ns3::UintegerValue (100),
                                           ns3::MakeUintegerChecker<uint16_t> ());

/*static ns3::GlobalValue g_macroEnbBandwidth ("macroEnbBandwidth",
                                             "bandwidth [num RBs] used by macro eNBs",
                                             ns3::UintegerValue (25),
                                             ns3::MakeUintegerChecker<uint16_t> ());
*/
static ns3::GlobalValue g_homeEnbBandwidth ("homeEnbBandwidth",
                                            "bandwidth [num RBs] used by HeNBs",
                                            ns3::UintegerValue (25),
                                            ns3::MakeUintegerChecker<uint16_t> ());

/*static ns3::GlobalValue g_simTime ("simTime",
                                   "Total duration of the simulation [s]",
                                   ns3::DoubleValue (0.25),
                                   ns3::MakeDoubleChecker<double> ());
*/
/*static ns3::GlobalValue g_generateRem ("generateRem",
                                       "if true, will generate a REM and then abort the simulation;"
                                       "if false, will run the simulation normally (without generating any REM)",
                                       ns3::BooleanValue (false),
                                       ns3::MakeBooleanChecker ());
*/
/*static ns3::GlobalValue g_remRbId ("remRbId",
                                   "Resource Block Id of Data Channel, for which REM will be generated;"
                                   "default value is -1, what means REM will be averaged from all RBs of "
                                   "Control Channel",
                                   ns3::IntegerValue (-1),
                                   MakeIntegerChecker<int32_t> ());
*/
/*static ns3::GlobalValue g_epc ("epc",
                               "If true, will setup the EPC to simulate an end-to-end topology, "
                               "with real IP applications over PDCP and RLC UM (or RLC AM by changing "
                               "the default value of EpsBearerToRlcMapping e.g. to RLC_AM_ALWAYS). "
                               "If false, only the LTE radio access will be simulated with RLC SM. ",
                               ns3::BooleanValue (false),
                               ns3::MakeBooleanChecker ());
*/
/*static ns3::GlobalValue g_epcDl ("epcDl",
                                 "if true, will activate data flows in the downlink when EPC is being used. "
                                 "If false, downlink flows won't be activated. "
                                 "If EPC is not used, this parameter will be ignored.",
                                 ns3::BooleanValue (true),
                                 ns3::MakeBooleanChecker ());
*/
/*static ns3::GlobalValue g_epcUl ("epcUl",
                                 "if true, will activate data flows in the uplink when EPC is being used. "
                                 "If false, uplink flows won't be activated. "
                                 "If EPC is not used, this parameter will be ignored.",
                                 ns3::BooleanValue (true),
                                 ns3::MakeBooleanChecker ());
*/
/*static ns3::GlobalValue g_useUdp ("useUdp",
                                  "if true, the UdpClient application will be used. "
                                  "Otherwise, the BulkSend application will be used over a TCP connection. "
                                  "If EPC is not used, this parameter will be ignored.",
                                  ns3::BooleanValue (true),
                                  ns3::MakeBooleanChecker ());
*/
/*static ns3::GlobalValue g_fadingTrace ("fadingTrace",
                                       "The path of the fading trace (by default no fading trace "
                                       "is loaded, i.e., fading is not considered)",
                                       ns3::StringValue (""),
                                       ns3::MakeStringChecker ());
*/

static ns3::GlobalValue g_numBearersPerUe ("numBearersPerUe",
                                           "How many bearers per UE there are in the simulation",
                                           ns3::UintegerValue (1),
                                           ns3::MakeUintegerChecker<uint16_t> ());
/*static ns3::GlobalValue g_srsPeriodicity ("srsPeriodicity",
                                          "SRS Periodicity (has to be at least "
                                          "greater than the number of UEs per eNB)",
                                          ns3::UintegerValue (80),
                                          ns3::MakeUintegerChecker<uint16_t> ());
*/

/*static ns3::GlobalValue g_outdoorUeMinSpeed ("outdoorUeMinSpeed",
                                             "Minimum speed value of macor UE with random waypoint model [m/s].",
                                             ns3::DoubleValue (0.0),
                                             ns3::MakeDoubleChecker<double> ());
*/
/*static ns3::GlobalValue g_outdoorUeMaxSpeed ("outdoorUeMaxSpeed",
                                             "Maximum speed value of macor UE with random waypoint model [m/s].",
                                             ns3::DoubleValue (0.0),
                                             ns3::MakeDoubleChecker<double> ());
*/



int main(int argc, char *argv[]){
    /*--------------------------PARAMETER INITIALIZATION-------------------------------*/
  UintegerValue uintegerValue;
  IntegerValue integerValue;
  DoubleValue doubleValue;
  BooleanValue booleanValue;
  StringValue stringValue;
  //GlobalValue::GetValueByName ("nBlocks", uintegerValue);
  //uint32_t nBlocks = uintegerValue.Get ();
  //GlobalValue::GetValueByName ("nApartmentsX", uintegerValue);
  //uint32_t nApartmentsX = uintegerValue.Get ();
  //GlobalValue::GetValueByName ("nFloors", uintegerValue);
  //uint32_t nFloors = uintegerValue.Get ();
  //GlobalValue::GetValueByName ("nMacroEnbSites", uintegerValue);
  //uint32_t nMacroEnbSites = uintegerValue.Get ();
  //GlobalValue::GetValueByName ("nMacroEnbSitesX", uintegerValue);
  //uint32_t nMacroEnbSitesX = uintegerValue.Get ();
  //GlobalValue::GetValueByName ("interSiteDistance", doubleValue);
  //double interSiteDistance = doubleValue.Get ();
  //GlobalValue::GetValueByName ("areaMarginFactor", doubleValue);
  //double areaMarginFactor = doubleValue.Get ();
  //GlobalValue::GetValueByName ("macroUeDensity", doubleValue);
  //double macroUeDensity = doubleValue.Get ();
  //GlobalValue::GetValueByName ("homeEnbDeploymentRatio", doubleValue);
  //double homeEnbDeploymentRatio = doubleValue.Get ();
  //GlobalValue::GetValueByName ("homeEnbActivationRatio", doubleValue);
  //double homeEnbActivationRatio = doubleValue.Get ();
  //GlobalValue::GetValueByName ("homeUesHomeEnbRatio", doubleValue);
  //double homeUesHomeEnbRatio = doubleValue.Get ();
  //GlobalValue::GetValueByName ("macroEnbTxPowerDbm", doubleValue);
  //double macroEnbTxPowerDbm = doubleValue.Get ();
  GlobalValue::GetValueByName ("homeEnbTxPowerDbm", doubleValue);
  double homeEnbTxPowerDbm = doubleValue.Get ();
  //GlobalValue::GetValueByName ("macroEnbDlEarfcn", uintegerValue);
  //uint16_t macroEnbDlEarfcn = uintegerValue.Get ();
  GlobalValue::GetValueByName ("homeEnbDlEarfcn", uintegerValue);
  uint16_t homeEnbDlEarfcn = uintegerValue.Get ();
  //GlobalValue::GetValueByName ("macroEnbBandwidth", uintegerValue);
  //uint16_t macroEnbBandwidth = uintegerValue.Get ();
  GlobalValue::GetValueByName ("homeEnbBandwidth", uintegerValue);
  uint16_t homeEnbBandwidth = uintegerValue.Get ();
  //GlobalValue::GetValueByName ("simTime", doubleValue);
  //double simTime = doubleValue.Get ();
  //GlobalValue::GetValueByName ("epc", booleanValue);
  //bool epc = booleanValue.Get ();
  //GlobalValue::GetValueByName ("epcDl", booleanValue);
  //bool epcDl = booleanValue.Get ();
  //GlobalValue::GetValueByName ("epcUl", booleanValue);
  //bool epcUl = booleanValue.Get ();
  //GlobalValue::GetValueByName ("useUdp", booleanValue);
  //bool useUdp = booleanValue.Get ();
  //GlobalValue::GetValueByName ("generateRem", booleanValue);
  //bool generateRem = booleanValue.Get ();
  //GlobalValue::GetValueByName ("remRbId", integerValue);
  //int32_t remRbId = integerValue.Get ();
  //GlobalValue::GetValueByName ("fadingTrace", stringValue);
  //std::string fadingTrace = stringValue.Get ();
  GlobalValue::GetValueByName ("numBearersPerUe", uintegerValue);
  uint16_t numBearersPerUe = uintegerValue.Get ();
  //GlobalValue::GetValueByName ("srsPeriodicity", uintegerValue);
  //uint16_t srsPeriodicity = uintegerValue.Get ();
  //GlobalValue::GetValueByName ("outdoorUeMinSpeed", doubleValue);
  //uint16_t outdoorUeMinSpeed = doubleValue.Get ();
  //GlobalValue::GetValueByName ("outdoorUeMaxSpeed", doubleValue);
  //uint16_t outdoorUeMaxSpeed = doubleValue.Get ();
  
  //Config::SetDefault ("ns3::LteEnbRrc::SrsPeriodicity", UintegerValue (srsPeriodicity));
	
	//Creating a LteHelper to instantiate some common objects like channel object and help add enbs and ues
	Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
	
	//Creating Node Containers to contain nodes
	NodeContainer HeNB;         //Container to hold FemtoCell
	NodeContainer UE;           //Container to hold cellphones or other user equipments
	NodeContainer HeNBGW;       //Container to hold Femtocell Gateways
	
	
	HeNB.Create(2);
	UE.Create(16);
	
	//Configure the mobility model for devices
	//*The MobilityHelper module is used
	//*The Constant Position Mobility model adds all the nodes at (0,0,0)
	MobilityHelper mobility;
	mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
	mobility.Install(HeNB);
	mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
	//mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel", "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));
	mobility.Install(UE);
	
	
	//Now we will install LTE stacks on respective devices, i.e., eNBs and UEs 
	NetDeviceContainer enbs;
	enbs = lteHelper->InstallEnbDevice (HeNB);
	NetDeviceContainer ues;
	ues = lteHelper->InstallUeDevice(UE);
	
	
	//Now we attach UEs to HeNB------(Replaced from Lena dual stripe example)
	//lteHelper->Attach (ues, enbs.Get (0));
	
	
	// ********************Getting parameters for the HeNB from Lna-DualStripe******************************
	
	/*
	double homeEnbTxPowerDbm;
	uint16_t homeEnbDlEarfcn, homeEnbBandwidth, numBearersPerUe;
	std::cout<<"Enter HeNB Transmission Power in dbm(double):";  std::cin >> homeEnbTxPowerDbm;
	std::cout<<"Enter HeNB DownLink EARFCN(Non zero int):";  std::cin >> homeEnbDlEarfcn;
	std::cout<<"Enter HeNB Bandwidth(non zero int):";  std::cin >> homeEnbBandwidth;
	std::cout<<"Enter number of bearers per ue :";  std::cin >> numBearersPerUe;
	*/
	
	
	// ***********************************FROM LENA-DUALSTRIPE, Line 400 on***********************************************
	// 
	
	// HomeEnbs randomly indoor
    Ptr<PositionAllocator> positionAlloc = CreateObject<RandomRoomPositionAllocator> ();
    mobility.SetPositionAllocator (positionAlloc);
    mobility.Install (HeNB);
    BuildingsHelper::Install (HeNB);
    Config::SetDefault ("ns3::LteEnbPhy::TxPower", DoubleValue (homeEnbTxPowerDbm));
    lteHelper->SetEnbAntennaModelType ("ns3::IsotropicAntennaModel");
    lteHelper->SetEnbDeviceAttribute ("DlEarfcn", UintegerValue (homeEnbDlEarfcn));
    lteHelper->SetEnbDeviceAttribute ("UlEarfcn", UintegerValue (homeEnbDlEarfcn + 18000));
    lteHelper->SetEnbDeviceAttribute ("DlBandwidth", UintegerValue (homeEnbBandwidth));
    lteHelper->SetEnbDeviceAttribute ("UlBandwidth", UintegerValue (homeEnbBandwidth));
    lteHelper->SetEnbDeviceAttribute ("CsgId", UintegerValue (1));
    lteHelper->SetEnbDeviceAttribute ("CsgIndication", BooleanValue (true));
    NetDeviceContainer homeEnbDevs  = lteHelper->InstallEnbDevice (HeNB);
    
    // home UEs located in the same apartment in which there are the Home eNBs
    positionAlloc = CreateObject<SameRoomPositionAllocator> (HeNB);
    mobility.SetPositionAllocator (positionAlloc);
    mobility.Install (UE);
    BuildingsHelper::Install (UE);
  
    // set the home UE as a CSG member of the home eNodeBs
    lteHelper->SetUeDeviceAttribute ("CsgId", UintegerValue (1));
    NetDeviceContainer homeUeDevs = lteHelper->InstallUeDevice (UE);
    
    // each home UE is attached explicitly to its home eNB
      NetDeviceContainer::Iterator ueDevIt;
      NetDeviceContainer::Iterator enbDevIt;
      for (ueDevIt = ues.Begin (), enbDevIt = enbs.Begin();
           ueDevIt != ues.End (); ++ueDevIt, ++enbDevIt)
        {
          // this because of the order in which SameRoomPositionAllocator
          // will place the UEs
          if (enbDevIt == enbs.End ())
            {
              enbDevIt = enbs.Begin ();
            }
          lteHelper->Attach (*ueDevIt, *enbDevIt);
        }
        
        
        
       // for radio bearer activation purposes, consider together home UEs and macro UEs
      NetDeviceContainer ueDevs;
      ueDevs.Add (ues);
      //ueDevs.Add (macroUeDevs);
      for (uint32_t u = 0; u < ueDevs.GetN (); ++u)
        {
          Ptr<NetDevice> ueDev = ueDevs.Get (u);
          for (uint32_t b = 0; b < numBearersPerUe; ++b)
            {
              enum EpsBearer::Qci q = EpsBearer::NGBR_VIDEO_TCP_DEFAULT;
              EpsBearer bearer (q);
              lteHelper->ActivateDataRadioBearer (ueDev, bearer);
            }
        }
    
    // *****************************************************************************************************
	
	
	//Now we activate a radio bearer b/w UE and NodeB. Here we use data Radio Bearer
	
	/* -------------this has also been included from Lena Model-----------------------------
	enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
	EpsBearer bearer (q);
	lteHelper->ActivateDataRadioBearer (ues, bearer);
	*/
	
	//Specify the simulation time. This part can be skipped. Refer NS3 Documentation for more information
	
	AnimationInterface anim("lte_net1_anim.xml");
	anim.SetConstantPosition(HeNB.Get(0),2.0,2.0);
	anim.SetConstantPosition(HeNB.Get(1),4.0,4.0);
	anim.SetConstantPosition(UE.Get(1),0.0,0.0);
	anim.SetConstantPosition(UE.Get(2),0.0,1.0);
	anim.SetConstantPosition(UE.Get(3),3.0,1.0);
	anim.SetConstantPosition(UE.Get(4),1.0,3.0);
	anim.SetConstantPosition(UE.Get(5),2.0,1.0);
	anim.SetConstantPosition(UE.Get(6),1.0,2.0);
	anim.SetConstantPosition(UE.Get(7),4.0,4.0);
	anim.SetConstantPosition(UE.Get(8),1.0,1.0);
	anim.SetConstantPosition(UE.Get(9),2.0,3.0);
	anim.SetConstantPosition(UE.Get(10),4.0,2.0);
	anim.SetConstantPosition(UE.Get(11),2.0,4.0);
	anim.SetConstantPosition(UE.Get(12),3.0,2.0);
	anim.SetConstantPosition(UE.Get(13),2.0,3.0);
	anim.SetConstantPosition(UE.Get(14),5.0,5.0);
	
	
	
	
	//Run Simulator
	Simulator::Run();
	Simulator::Destroy();
	
	
	
	
	return 0;
}
