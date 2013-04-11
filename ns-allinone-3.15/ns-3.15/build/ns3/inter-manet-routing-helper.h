
#ifndef INTER_MANET_ROUTING_HELPER_H
#define INTER_MANET_ROUTING_HELPER_H

#include "ns3/ipv4-routing-helper.h"
#include <stdint.h>
#include <list>

namespace ns3 {

/**
 * \brief Helper class that adds ns3::Ipv4ListRouting objects
 *
 * This class is expected to be used in conjunction with 
 * ns3::InternetStackHelper::SetRoutingHelper
 */
class InterMANETRoutingHelper : public Ipv4RoutingHelper
{
public:
  /*
   * Construct an Ipv4ListRoutingHelper used to make installing routing
   * protocols easier.
   */
  InterMANETRoutingHelper ();

  /*
   * \internal
   * Destroy an Ipv4ListRoutingHelper.
   */
  virtual ~InterMANETRoutingHelper ();

  /**
   * \brief Construct an Ipv4ListRoutingHelper from another previously 
   * initialized instance (Copy Constructor).
   */
  InterMANETRoutingHelper (const InterMANETRoutingHelper &);

  /**
   * \internal
   * \returns pointer to clone of this Ipv4ListRoutingHelper 
   * 
   * This method is mainly for internal use by the other helpers;
   * clients are expected to free the dynamic memory allocated by this method
   */
  InterMANETRoutingHelper* Copy (void) const;

  /**
   * \param routing a routing helper
   * \param priority the priority of the associated helper
   *
   * Store in the internal list a reference to the input routing helper
   * and associated priority. These helpers will be used later by
   * the ns3::Ipv4ListRoutingHelper::Create method to create
   * an ns3::Ipv4ListRouting object and add in it routing protocols
   * created with the helpers.
   */
  void Add (const Ipv4RoutingHelper &routing, int16_t priority);
  /**
   * \param node the node on which the routing protocol will run
   * \returns a newly-created routing protocol
   *
   * This method will be called by ns3::InternetStackHelper::Install
   */
  virtual Ptr<Ipv4RoutingProtocol> Create (Ptr<Node> node) const;
private:
  /**
   * \internal
   * \brief Assignment operator declared private and not implemented to disallow
   * assignment and prevent the compiler from happily inserting its own.
   */
  InterMANETRoutingHelper &operator = (const InterMANETRoutingHelper &o);

  std::list<std::pair<const Ipv4RoutingHelper *,int16_t> > m_list;
};

} // namespace ns3

#endif /* IPV4_LIST_ROUTING_HELPER_H */
