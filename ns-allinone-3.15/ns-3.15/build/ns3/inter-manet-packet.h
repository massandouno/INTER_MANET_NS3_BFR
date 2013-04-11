
#ifndef INTER_MANET_PACKET_H
#define INTER_MANET_PACKET_H

#include <iostream>
#include "ns3/header.h"
#include "ns3/ipv4-address.h"
#include "ns3/nstime.h"

namespace ns3 {
namespace inter_manet {
/**
 * \ingroup dsdv
 * \brief DSDV Update Packet Format
 * \verbatim
 |      0        |      1        |      2        |       3       |
  0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                      Destination Address                      |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                            HopCount                           |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                       Sequence Number                         |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * \endverbatim
 */

class InterMANETHeader : public Header
{
public:
  InterMANETHeader (Ipv4Address dst = Ipv4Address (), uint32_t hopcount = 0, uint32_t dstSeqNo = 0);
  virtual ~InterMANETHeader ();
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize () const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;

  void
  SetDst (Ipv4Address destination)
  {
    m_dst = destination;
  }
  Ipv4Address
  GetDst () const
  {
    return m_dst;
  }
  void
  SetHopCount (uint32_t hopCount)
  {
    m_hopCount = hopCount;
  }
  uint32_t
  GetHopCount () const
  {
    return m_hopCount;
  }
  void
  SetDstSeqno (uint32_t sequenceNumber)
  {
    m_dstSeqNo = sequenceNumber;
  }
  uint32_t
  GetDstSeqno () const
  {
    return m_dstSeqNo;
  }
private:
  Ipv4Address m_dst; ///< Destination IP Address
  uint32_t m_hopCount; ///< Number of Hops
  uint32_t m_dstSeqNo; ///< Destination Sequence Number
};
static inline std::ostream & operator<< (std::ostream& os, const InterMANETHeader & packet)
{
  packet.Print (os);
  return os;
}
}
}

#endif /* DSDV_PACKET_H */
