#ifndef _ANA_NIM3__H_
#define _ANA_NIM3__H_
#include <map>
#include "RoadMap.h"
#include "RoadList.h"
#include "AnaBase.h"
class TH1;

class AnaNim3 : public AnaBase {
 protected:
  typedef std::vector<int> EleList;

  int  m_n_evt_used;
  int  m_n_evt_tb;
  int  m_n_evt_fired;
  TH1* m_h1_evt_cnt;

  RoadMap  m_road_map_top;
  RoadMap  m_road_map_bot;

  const RoadMap* m_road_map_pos_top_on;
  const RoadMap* m_road_map_pos_bot_on;
  const RoadMap* m_road_map_neg_top_on;
  const RoadMap* m_road_map_neg_bot_on;

  ///
  /// Inputs
  ///
  int      b_run;
  int      b_evt;
  int      b_inte_max;
  int      b_fpga1;
  EleList* b_h1t;
  EleList* b_h2t;
  EleList* b_h3t;
  EleList* b_h4t;
  EleList* b_h1b;
  EleList* b_h2b;
  EleList* b_h3b;
  EleList* b_h4b;

  ///
  /// Outputs
  ///
  TFile* m_file_out;
  TH1* m_h1_inte_max;

  ///
  /// Parameters
  ///
  int m_inte_cut;

 public:
  AnaNim3(const std::string label="nim3");
  virtual ~AnaNim3();
  virtual void Init();
  virtual void End();
  void ReadEventsFromFileList(const char* fn_list, const int n_max=0);
  void ReadEventsFromFile(const char* fname, const char* tname="tree");
  virtual void Analyze();

  void SetEnabledRoads(const RoadMap* pos_top, const RoadMap* pos_bot, const RoadMap* neg_top, const RoadMap* neg_bot);

  RoadMap * GetRoadMapTop () { return &m_road_map_top ; }
  RoadMap * GetRoadMapBot () { return &m_road_map_bot ; }

  void SetInteCut(const int inte_cut) { m_inte_cut = inte_cut; }
  int  GetInteCut()            { return m_inte_cut; }

 protected:
  virtual void ProcessOneEvent();
  void FindAllRoads(const EleList* h1, const EleList* h2, const EleList* h3, const EleList* h4, const int tb, RoadMap* road_map);
  void DrawInteMax();
};

#endif // _ANA_NIM3__H_
