#include <iostream>
#include <fstream>
#include <iomanip>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <THStack.h>
#include <TLegend.h>
#include <TH1D.h>
#include <UtilAna/UtilTrigger.h>
#include "RoadInfo.h"
#include "RoadMap.h"
#include "RoadList.h"
#include "UtilRoad.h"
#include "AnaGmc.h"
#include "AnaNim3Add.h"
using namespace std;

AnaNim3Add::AnaNim3Add(const std::string label)
  : AnaNim3(label)
  , m_road_map_pos_top(0)
  , m_road_map_pos_bot(0)
  , m_road_map_neg_top(0)
  , m_road_map_neg_bot(0)
{
  ;
}

AnaNim3Add::~AnaNim3Add() 
{
  ;
}

void AnaNim3Add::Init(AnaGmc* ana_gmc)
{
  m_road_map_pos_top = ana_gmc->GetRoadMapPosTop();
  m_road_map_pos_bot = ana_gmc->GetRoadMapPosBot();
  m_road_map_neg_top = ana_gmc->GetRoadMapNegTop();
  m_road_map_neg_bot = ana_gmc->GetRoadMapNegBot();
  m_road_map_pos_top->Frozen();
  m_road_map_pos_bot->Frozen();
  m_road_map_neg_top->Frozen();
  m_road_map_neg_bot->Frozen();
}

void AnaNim3Add::End()
{
  ;
}

void AnaNim3Add::ProcessOneEvent()
{
  if (b_inte_max == 0) return; // Some readout error
  m_n_evt_used++;

  if (m_inte_cut > 0 && b_inte_max >= m_inte_cut) return;
  if (b_h1t->size() == 0 || b_h1b->size() == 0 || 
      b_h2t->size() == 0 || b_h2b->size() == 0 || 
      b_h3t->size() == 0 || b_h3b->size() == 0 || 
      b_h4t->size() == 0 || b_h4b->size() == 0   ) return;
  m_n_evt_tb++;
  
  RoadMap map_top;
  RoadMap map_bot;
  FindAllRoads(b_h1t, b_h2t, b_h3t, b_h4t, +1, &map_top);
  FindAllRoads(b_h1b, b_h2b, b_h3b, b_h4b, -1, &map_bot);
  
  bool fired = false;
  if (map_top.Contain(m_road_map_pos_top) &&
      map_bot.Contain(m_road_map_neg_bot)   ) { // T+B
    fired = true;
    for (RoadMap::Iter it = map_top.Begin(); it != map_top.End(); it++) {
      if (m_road_map_pos_top->Find(it->first)) m_road_map_pos_top->AddBG(it->second);
    }
    for (RoadMap::Iter it = map_bot.Begin(); it != map_bot.End(); it++) {
      if (m_road_map_neg_bot->Find(it->first)) m_road_map_neg_bot->AddBG(it->second);
    }
  }
  if (map_top.Contain(m_road_map_neg_top) &&
      map_bot.Contain(m_road_map_pos_bot)   ) { // B+T
    fired = true;
    for (RoadMap::Iter it = map_top.Begin(); it != map_top.End(); it++) {
      if (m_road_map_neg_top->Find(it->first)) m_road_map_neg_top->AddBG(it->second);
    }
    for (RoadMap::Iter it = map_bot.Begin(); it != map_bot.End(); it++) {
      if (m_road_map_pos_bot->Find(it->first)) m_road_map_pos_bot->AddBG(it->second);
    }
  }

  if (fired) m_n_evt_fired++;
}

void AnaNim3Add::Analyze()
{
  cout << "AnaNim3Add::Analyze():" << endl;

  m_ofs << "Event Counts:\n";
  for (int ib = 1; ib < m_h1_evt_cnt->GetNbinsX(); ib++) {
    double cont = m_h1_evt_cnt->GetBinContent(ib);
    if (cont == 0) break;
    m_ofs << setw(4) << ib << " " << cont << "\n";
  }
  m_ofs << "\n"
        << "N of analyzed events:\n"
        << "  Used  " << m_n_evt_used << "\n"
        << "  T+B   " << m_n_evt_tb << "\n"
        << "  Fired " << m_n_evt_fired << "\n \n"
        << "Expected counts per spill:\n"
        << "  T+B   " << 186e6 * m_n_evt_tb    / m_n_evt_used << "\n"
        << "  Fired " << 186e6 * m_n_evt_fired / m_n_evt_used << "\n"
        << endl;
  // Expected counts per spill = [N of "fired" events] / [N of all events] * [N of filled RFs]
  // N of filled RFs = 186e6 = 588 * 369000 * 6 / 7

  m_road_map_pos_top->ScaleBG(186e6 / m_n_evt_used);
  m_road_map_pos_bot->ScaleBG(186e6 / m_n_evt_used);
  m_road_map_neg_top->ScaleBG(186e6 / m_n_evt_used);
  m_road_map_neg_bot->ScaleBG(186e6 / m_n_evt_used);

  ///
  /// Regularize BG, namely set its weight to 1 (per spill), so that FoM is always finite.
  ///
  for (RoadMap::Iter it = m_road_map_pos_top->Begin(); it != m_road_map_pos_top->End(); it++) {
    if (it->second->GetWeightBG() == 0) it->second->AddBG(1);
  }
  for (RoadMap::Iter it = m_road_map_pos_bot->Begin(); it != m_road_map_pos_bot->End(); it++) {
    if (it->second->GetWeightBG() == 0) it->second->AddBG(1);
  }
  for (RoadMap::Iter it = m_road_map_neg_top->Begin(); it != m_road_map_neg_top->End(); it++) {
    if (it->second->GetWeightBG() == 0) it->second->AddBG(1);
  }
  for (RoadMap::Iter it = m_road_map_neg_bot->Begin(); it != m_road_map_neg_bot->End(); it++) {
    if (it->second->GetWeightBG() == 0) it->second->AddBG(1);
  }
}
