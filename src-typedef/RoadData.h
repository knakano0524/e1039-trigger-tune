#ifndef _ROAD_DATA__H_
#define _ROAD_DATA__H_
#include <vector>
#include <map>
//#include <phool/PHObject.h>
//#include <TLorentzVector.h>

class RoadInfo {
  int    m_road_id;
  int    m_count;
  double m_weight;

 public:
  RoadInfo();
  RoadInfo(const RoadInfo& obj);
  RoadInfo(const int road_id, const int count, const double weight);
  virtual ~RoadInfo();

  void   Add      (const double weight);
  void   SetRoadID(const int road_id  ) { m_road_id = road_id; }
  void   SetCount (const int count    ) { m_count   = count  ; }
  void   SetWeight(const double weight) { m_weight  = weight ; }
  int    GetRoadID() const { return m_road_id; }
  int    GetCount () const { return m_count  ; }
  double GetWeight() const { return m_weight ; }
  
  bool operator<(const RoadInfo& obj) const;
};

typedef std::vector<RoadInfo> RoadList;
typedef std::map<int, RoadInfo> RoadMap;

typedef std::vector<int> EleList;

namespace UtilRoad {
  void MakeOrderedList(RoadMap& road_map, RoadList& road_list);
}


#endif // _ROAD_DATA__H_
