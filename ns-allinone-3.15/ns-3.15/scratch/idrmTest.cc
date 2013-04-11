
#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/olsr-helper.h"
#include "ns3/aodv-module.h"
#include "ns3/dsdv-module.h"
#include "ns3/idrm-module.h"
#include "ns3/dsr-module.h"
#include "ns3/ipv4-list-routing-helper.h"
#include "ns3/netanim-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("youluExample");

int main(int argc, char* argv[])
{
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
  //LogComponentEnable ("AodvRoutingProtocol", LOG_LEVEL_LOGIC);

  NodeContainer GNodes;
  NodeContainer LNodes;
  NodeContainer RNodes;
  GNodes.Create(1);
  LNodes.Create(2);
  LNodes.Add(GNodes.Get(0));
  RNodes.Add(GNodes.Get(0));
  RNodes.Create(2);
  
  GNodes.Get(0)->SetNType(2);
  LNodes.Get(0)->SetNType(0);
  LNodes.Get(1)->SetNType(0);
  RNodes.Get(1)->SetNType(0);
  RNodes.Get(2)->SetNType(0);

  YansWifiChannelHelper Lchannel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper Lphy = YansWifiPhyHelper::Default ();
  Lphy.SetChannel (Lchannel.Create ());

  WifiHelper Lwifi = WifiHelper::Default ();
  Lwifi.SetRemoteStationManager ("ns3::AarfWifiManager");

  NqosWifiMacHelper Lmac = NqosWifiMacHelper::Default ();

  Lmac.SetType ("ns3::AdhocWifiMac");

  YansWifiChannelHelper Rchannel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper Rphy = YansWifiPhyHelper::Default ();
  Rphy.SetChannel (Rchannel.Create ());

  WifiHelper Rwifi = WifiHelper::Default ();
  Rwifi.SetRemoteStationManager ("ns3::AarfWifiManager");

  NqosWifiMacHelper Rmac = NqosWifiMacHelper::Default ();

  Rmac.SetType ("ns3::AdhocWifiMac");

  NetDeviceContainer LDevices;
  NetDeviceContainer RDevices;
  LDevices = Lwifi.Install (Lphy, Lmac, LNodes);
  RDevices = Rwifi.Install (Rphy, Rmac, RNodes);
    

  MobilityHelper Lmobility;
  MobilityHelper Rmobility;

  Lmobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (5.0),
                                 "DeltaY", DoubleValue (10.0),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));

  Lmobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                             "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));

  Rmobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (5.0),
                                 "DeltaY", DoubleValue (10.0),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));

  Rmobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                             "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));

  Lmobility.Install (LNodes);
  Rmobility.Install (RNodes);
  //mobility.Install (GNodes);

  // Enable OLSR
  OlsrHelper Lolsr;
  OlsrHelper Golsr;
  OlsrHelper Rolsr;
  // Enable AODV
  AodvHelper Laodv;
  AodvHelper Gaodv;
  AodvHelper Raodv;
  
  IdrmHelper dsdv;

  Ipv4ListRoutingHelper Llist;
  //Llist.Add (Laodv, 0);
  Llist.Add (Lolsr, 0);
  Ipv4ListRoutingHelper Glist;
  //Glist.Add (Gaodv, 0);
  Glist.Add (Golsr, 10);
  //Glist.Add (Gaodv, 10);
  Ipv4ListRoutingHelper Rlist;
  Rlist.Add (Rolsr, 0);
  //Rlist.Add (Raodv, 0);

  InternetStackHelper Lstack;
  InternetStackHelper Gstack;
  InternetStackHelper Rstack;
  Lstack.SetRoutingHelper (dsdv);
  Gstack.SetRoutingHelper (dsdv);
  Rstack.SetRoutingHelper (dsdv);
  Lstack.Install (LNodes.Get(0));
  Lstack.Install (LNodes.Get(1));
  Gstack.Install (GNodes);
  Rstack.Install (RNodes.Get(1));
  Rstack.Install (RNodes.Get(2));

  Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper>  ("testRoutingTable",std::ios::out);
  Laodv.PrintRoutingTableAllEvery(Seconds(10), routingStream);

  Ipv4AddressHelper Laddress;
  Ipv4AddressHelper Raddress;
  Laddress.SetBase ("10.1.1.0", "255.255.255.0");
  Raddress.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer LInterfaces;
  Ipv4InterfaceContainer RInterfaces;
  LInterfaces = Laddress.Assign (LDevices);
  RInterfaces = Raddress.Assign (RDevices);

  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (RNodes.Get (2));
  serverApps.Start (Seconds (29.0));
  serverApps.Stop (Seconds (300.0));

  UdpEchoClientHelper echoClient (RInterfaces.GetAddress (2), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1000));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = 
    echoClient.Install (LNodes.Get (0));
  clientApps.Start (Seconds (30.0));
  clientApps.Stop (Seconds (300.0));

  //Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  
  AnimationInterface anim ("animation.xml");
  anim.SetMobilityPollInterval (Seconds (1));
  //anim.SetConstantPosition (wifiNodesL.Get(1), 0, 0);
  //anim.SetConstantPosition (wifiNodesL.Get(0), 100, 0);
  //anim.SetConstantPosition (wifiNodesR.Get(1), 200, 0);
  anim.EnablePacketMetadata (true);

  Simulator::Stop (Seconds (300.0));

  Lphy.EnablePcap ("youlu", LDevices);
  Rphy.EnablePcap ("youlu", RDevices);

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
