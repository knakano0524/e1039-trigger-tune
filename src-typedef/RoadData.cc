#include <iostream>
#include <algorithm>
#include "RoadData.h"
using namespace std;

RoadInfo::RoadInfo()
  : m_road_id(0)
  , m_count  (0)
  , m_weight (0)
{
  ;
}

RoadInfo::RoadInfo(const RoadInfo& obj)
  : m_road_id(obj.GetRoadID())
  , m_count  (obj.GetCount ())
  , m_weight (obj.GetWeight())
{
  ;
}

RoadInfo::RoadInfo(const int road_id, const int count, const double weight)
  : m_road_id(road_id)
  , m_count  (count  )
  , m_weight (weight )
{
  ;
}

RoadInfo::~RoadInfo()
{
  ;
}


void RoadInfo::Add(const double weight)
{
  m_count  += 1;
  m_weight += weight;
}

bool RoadInfo::operator<(const RoadInfo& obj) const
{
  return m_weight < obj.GetWeight() || m_road_id < obj.GetRoadID();
  //return m_count < obj.GetCount() || m_road_id < obj.GetRoadID();
}

void UtilRoad::MakeOrderedList(RoadMap& road_map, RoadList& road_list)
{
  //road_list.clear();
  //road_list.reserve(road_map.size());
  cout << "N " << road_map.size() << endl;
  for (RoadMap::iterator it = road_map.begin(); it != road_map.end(); it++) {
    int road = it->first;
    it->second.SetRoadID(road);
    road_list.push_back(it->second);
  }
  sort(road_list.begin(), road_list.end());
}
