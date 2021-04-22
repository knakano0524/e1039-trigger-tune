#ifndef _ANA_GMC__H_
#define _ANA_GMC__H_
#include <map>
#include <TVector3.h>
#include "RoadData.h"
class TFile;
class TTree;

class AnaGmc {
  //TFile* m_file;
  //TTree* m_tree;

  RoadMap m_road_map_pos_top;
  RoadMap m_road_map_pos_bot;
  RoadMap m_road_map_neg_top;
  RoadMap m_road_map_neg_bot;

  RoadList m_road_list_pos_top;
  RoadList m_road_list_pos_bot;
  RoadList m_road_list_neg_top;
  RoadList m_road_list_neg_bot;

 public:
  AnaGmc();
  virtual ~AnaGmc();
  void ReadOneFile(const char* fname="sim_tree.root", const char* tname="tree");
  void AnalyzeRoads();
  void End();

  RoadMap & GetRoadMapPosTop () { return m_road_map_pos_top ; }
  RoadMap & GetRoadMapPosBot () { return m_road_map_pos_bot ; }
  RoadMap & GetRoadMapNegTop () { return m_road_map_neg_top ; }
  RoadMap & GetRoadMapNegBot () { return m_road_map_neg_bot ; }

  RoadList& GetRoadListPosTop() { return m_road_list_pos_top; }
  RoadList& GetRoadListPosBot() { return m_road_list_pos_bot; }
  RoadList& GetRoadListNegTop() { return m_road_list_neg_top; }
  RoadList& GetRoadListNegBot() { return m_road_list_neg_bot; }

 protected:
  void DrawOneVar(TTree* tree, const char* name, const char* var, const int N, const double X0, const double X1);
};

#endif /* _ANA_GMC__H_ */
