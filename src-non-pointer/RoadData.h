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

//typedef std::vector<RoadInfo> RoadList;
//typedef std::map<int, RoadInfo> RoadMap;

class RoadList {
  std::vector<RoadInfo> m_vec;

 public:
  RoadList();
  virtual ~RoadList();

  int  Size() const { return m_vec.size(); }
  void Add(const RoadInfo& info);
  void Sort();
};

class RoadMap {
  typedef std::map<int, RoadInfo> InfoMap;
  InfoMap m_map;

 public:
  typedef InfoMap::iterator Iter;
  typedef InfoMap::const_iterator ConstIter;

  RoadMap();
  virtual ~RoadMap();

  int       Size () const { return m_map.size (); }
  ConstIter Begin() const { return m_map.begin(); }
  ConstIter End  () const { return m_map.end  (); }
  Iter      Begin()       { return m_map.begin(); }
  Iter      End  ()       { return m_map.end  (); }
  RoadInfo* Find(const int road);
  void      Add (const int road, const double weight);
};


typedef std::vector<int> EleList;

namespace UtilRoad {
  void MakeOrderedList(const RoadMap* road_map, RoadList* road_list);
}


#endif // _ROAD_DATA__H_
