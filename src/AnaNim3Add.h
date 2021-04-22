#ifndef _ANA_NIM3_ADD__H_
#define _ANA_NIM3_ADD__H_
#include <map>
#include "RoadMap.h"
#include "RoadList.h"
#include "AnaNim3.h"
class TH1;
class AnaGmc;

class AnaNim3Add : public AnaNim3 {
 protected:
  RoadMap* m_road_map_pos_top;
  RoadMap* m_road_map_pos_bot;
  RoadMap* m_road_map_neg_top;
  RoadMap* m_road_map_neg_bot;

  ///
  /// Parameters
  ///

 public:
  AnaNim3Add(const std::string label="nim3_add");
  virtual ~AnaNim3Add();
  virtual void Init(AnaGmc* ana_gmc);
  virtual void End();
  virtual void Analyze();

 protected:
  virtual void ProcessOneEvent();
};

#endif // _ANA_NIM3_ADD__H_
