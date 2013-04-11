
#ifndef INTER_MANET_HELPER_H
#define INTER_MANET_HELPER_H

#include "ns3/object-factory.h"
#include "ns3/node.h"
#include "ns3/node-container.h"
#include "ns3/ipv4-routing-helper.h"

namespace ns3 {
/**
 * \ingroup dsdv
 * \brief Helper class that adds DSDV routing to nodes.
 */
class InterMANETHelper : public Ipv4RoutingHelper
{
public:
  InterMANETHelper ();
  ~InterMANETHelper ();
  /**
   * \internal
   * \returns pointer to clone of this InterMANETHelper
   *
   * This method is mainly for internal use by the other helpers;
   * clients are expected to free the dynamic memory allocated by this method
   */
  InterMANETHelper* Copy (void) const;

  /**
   * \param node the node on which the routing protocol will run
   * \returns a newly-created routing protocol
   *
   * This method will be called by ns3::InternetStackHelper::Install
   *
   */
  virtual Ptr<Ipv4RoutingProtocol> Create (Ptr<Node> node) const;
  /**
   * \param name the name of the attribute to set
   * \param value the value of the attribute to set.
   *
   * This method controls the attributes of ns3::dsdv::RoutingProtocol
   */
  void Set (std::string name, const AttributeValue &value);

private:
  ObjectFactory m_agentFactory;
};

}

#endif /* DSDV_HELPER_H */
