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

////////////////////////////////////////////////////////////////
RoadList::RoadList()
{
  ;
}

RoadList::~RoadList()
{
  ;
}

void RoadList::Add(const RoadInfo& info)
{
  m_vec.push_back(info);
}

void RoadList::Sort()
{
  sort(m_vec.begin(), m_vec.end());
}

////////////////////////////////////////////////////////////////
RoadMap::RoadMap()
{
  ;
}

RoadMap::~RoadMap()
{
  ;
}

RoadInfo* RoadMap::Find(const int road)
{
  return m_map.find(road) != m_map.end()  ?  &m_map[road]  :  0;
}

void RoadMap::Add(const int road, const double weight)
{
  RoadInfo* info = Find(road);
  if (! info) {
    info = &m_map[road];
    info->SetRoadID(road);
  }
  info->Add(weight);
}

////////////////////////////////////////////////////////////////

void UtilRoad::MakeOrderedList(const RoadMap* road_map, RoadList* road_list)
{
  //road_list.clear();
  //road_list.reserve(road_map.size());
  cout << "N " << road_map->Size() << endl;
  for (RoadMap::ConstIter it = road_map->Begin(); it != road_map->End(); it++) {
    cout << "  I " << it->first << " " << &it->second << endl;
    //int road = it->first;
    //it->second.SetRoadID(road);
    road_list->Add(it->second);
  }
  cout << "  S" << endl;
  road_list->Sort();
  cout << "  T" << endl;
}
