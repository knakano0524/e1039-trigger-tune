#ifndef _ANA_NIM3__H_
#define _ANA_NIM3__H_
#include <map>
#include "RoadData.h"
class TFile;
class TTree;
class TH1;

class AnaNim3 {
  int  m_n_evt_used;
  int  m_n_evt_tb;
  TH1* m_h1_evt_cnt;

  RoadMap  m_road_map_top;
  RoadMap  m_road_map_bot;
  RoadList m_road_list_top;
  RoadList m_road_list_bot;

 public:
  AnaNim3();
  virtual ~AnaNim3();
  void ReadFilesInList(const char* fn_list, const int n_max=0);
  void ReadOneFile(const char* fname, const char* tname="tree");
  void AnalyzeRoads();
  void End();

  RoadMap & GetRoadMapTop () { return m_road_map_top ; }
  RoadMap & GetRoadMapBot () { return m_road_map_bot ; }
  RoadList& GetRoadListTop() { return m_road_list_top; }
  RoadList& GetRoadListBot() { return m_road_list_bot; }

 protected:
  void FindAllRoads(const EleList* h1, const EleList* h2, const EleList* h3, const EleList* h4, const int tb, RoadMap* road_map);
};

#endif // _ANA_NIM3__H_
