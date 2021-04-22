#ifndef _ANA_GMC__H_
#define _ANA_GMC__H_
#include "RoadMap.h"
#include "RoadList.h"
#include "AnaBase.h"
class TTree;

class AnaGmc : public AnaBase {
 protected:
  double  m_mass_lo;
  double  m_mass_hi;
  RoadMap m_road_map_pos_top;
  RoadMap m_road_map_pos_bot;
  RoadMap m_road_map_neg_top;
  RoadMap m_road_map_neg_bot;

  double b_weight;
  double b_mass;
  double b_xF;
  double b_x1;
  double b_x2;
  double b_mom;
  double b_phi;
  double b_theta;
  int    b_road_pos;
  int    b_road_neg;

  //RoadList m_road_list_pos_top;
  //RoadList m_road_list_pos_bot;
  //RoadList m_road_list_neg_top;
  //RoadList m_road_list_neg_bot;

 public:
  AnaGmc(const std::string label="gmc_gen");
  virtual ~AnaGmc();

  void SetMassRange(const double mass_lo, const double mass_hi);

  virtual void Init();
  virtual void End();
  virtual void Analyze();

  void ReadOneFile(const char* fname="sim_tree.root", const char* tname="tree");
  //void GenerateRoadSet(const double frac_cut, const std::string rs_id);
  //void GenerateRoadSetByFoM(const double frac_cut, const std::string rs_id);

  RoadMap * GetRoadMapPosTop () { return &m_road_map_pos_top ; }
  RoadMap * GetRoadMapPosBot () { return &m_road_map_pos_bot ; }
  RoadMap * GetRoadMapNegTop () { return &m_road_map_neg_top ; }
  RoadMap * GetRoadMapNegBot () { return &m_road_map_neg_bot ; }

  //RoadList* GetRoadListPosTop() { return &m_road_list_pos_top; }
  //RoadList* GetRoadListPosBot() { return &m_road_list_pos_bot; }
  //RoadList* GetRoadListNegTop() { return &m_road_list_neg_top; }
  //RoadList* GetRoadListNegBot() { return &m_road_list_neg_bot; }

 protected:
  virtual void ProcessOneEvent();
  //void DisableRoadsByWeightFraction(RoadList* road_list, const double frac);
  //void DisableRoadsByFoM(RoadList* road_list, const double frac);
};

#endif /* _ANA_GMC__H_ */
