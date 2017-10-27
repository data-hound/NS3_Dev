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

using namespace ns3;

int main(int argc, char *argv[]){
	
	//Creating a LteHelper to instantiate some common objects like channel object and help add enbs and ues
	Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
	//Creating Node Containers to contain nodes
	NodeContainer HeNB;         //Container to hold FemtoCell
	NodeContainer UE;           //Container to hold cellphones or other user equipments
	NodeContainer HeNBGW;       //Container to hold Femtocell Gateways
	
	
	HeNB.Create(1);
	UE.Create(8);
	
	//Configure the mobility model for devices
	//*The MobilityHelper module is used
	//*The Constant Position Mobility model adds all the nodes at (0,0,0)
	MobilityHelper mobility;
	mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
	mobility.Install(HeNB);
	mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel", "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));
	mobility.Install(UE);
	
	
	//Now we will install LTE stacks on respective devices, i.e., eNBs and UEs 
	NetDeviceContainer enbs;
	enbs = lteHelper->InstallEnbDevice (HeNB);
	NetDeviceContainer ues;
	ues = lteHelper->InstallUeDevice(UE);
	
	
	//Now we attach UEs to HeNB
	lteHelper->Attach (ues, enbs.Get (0));
	
	//Now we activate a radio bearer b/w UE and NodeB. Here we use data Radio Bearer
	
	enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
	EpsBearer bearer (q);
	lteHelper->ActivateDataRadioBearer (ues, bearer);
	
	//Specify the simulation time. This part can be skipped. Refer NS3 Documentation for more information
	
	//Run Simulator
	Simulator::Run();
	Simulator::Destroy();
	
	
	
	
	return 0;
}
