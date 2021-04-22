#include <iostream>
#include <iomanip>
#include <TFile.h>
#include <TTree.h>
#include "RoadInfo.h"
#include "RoadMap.h"
#include "RoadList.h"
#include "UtilRoad.h"
#include "AnaGmc.h"
using namespace std;

AnaGmc::AnaGmc(const std::string label)
  : AnaBase(label)
  , m_mass_lo(0.0)
  , m_mass_hi(100.0)
{
  m_road_map_pos_top.SetPNTB(+1, +1);
  m_road_map_pos_bot.SetPNTB(+1, -1);
  m_road_map_neg_top.SetPNTB(-1, +1);
  m_road_map_neg_bot.SetPNTB(-1, -1);

  //m_road_list_pos_top.SetPNTB(+1, +1);
  //m_road_list_pos_bot.SetPNTB(+1, -1);
  //m_road_list_neg_top.SetPNTB(-1, +1);
  //m_road_list_neg_bot.SetPNTB(-1, -1);
}

AnaGmc::~AnaGmc()
{
  ;
}

void AnaGmc::SetMassRange(const double mass_lo, const double mass_hi)
{
  m_mass_lo = mass_lo;
  m_mass_hi = mass_hi;
}

void AnaGmc::Init()
{
  ;
}

void AnaGmc::End()
{
  ;
}

void AnaGmc::ReadOneFile(const char* fname, const char* tname)
{
  //gROOT->cd();
  TFile* file = new TFile(fname);
  if (! file->IsOpen()) {
    cout << "Cannot open the file, '" << fname << "'.  Abort." << endl;
    exit(1);
  }
  TTree* tree = (TTree*)file->Get(tname);
  if (! tree) {
    cout << "Cannot get the tree, '" << tname << "'.  Abort." << endl;
    exit(1);
  }

  tree->SetBranchAddress("weight"  , &b_weight  );
  tree->SetBranchAddress("mass"    , &b_mass    );
  tree->SetBranchAddress("xF"      , &b_xF      );
  tree->SetBranchAddress("x1"      , &b_x1      );
  tree->SetBranchAddress("x2"      , &b_x2      );
  tree->SetBranchAddress("mom"     , &b_mom     );
  tree->SetBranchAddress("phi"     , &b_phi     );
  tree->SetBranchAddress("theta"   , &b_theta   );
  tree->SetBranchAddress("road_pos", &b_road_pos);
  tree->SetBranchAddress("road_neg", &b_road_neg);
  for (int i_ent = 0; i_ent < tree->GetEntries(); i_ent++) {
    tree->GetEntry(i_ent);
    ProcessOneEvent();
  }

  file->Close();
  delete file;
}

void AnaGmc::ProcessOneEvent()
{
  if (b_mass < m_mass_lo || b_mass > m_mass_hi) return;
  if (b_road_pos * b_road_neg >= 0) return;

  RoadMap* map = &(b_road_pos > 0 ? m_road_map_pos_top : m_road_map_pos_bot);
  map->Add(b_road_pos, b_weight);
  
  map = &(b_road_neg > 0 ? m_road_map_neg_top : m_road_map_neg_bot);
  map->Add(b_road_neg, b_weight);
}

void AnaGmc::Analyze()
{
  cout << "AnaGmc::Analyze():" << endl;
  cout << "  Do nothing for now.  Probably it will scale the signal weight by the simulated integrated luminosity." << endl;

  //m_road_list_pos_top.reserve(m_road_map_pos_top.size());
  //m_road_list_pos_bot.reserve(m_road_map_pos_bot.size());
  //m_road_list_neg_top.reserve(m_road_map_neg_top.size());
  //m_road_list_neg_bot.reserve(m_road_map_neg_bot.size());

  //UtilRoad::MakeOrderedList(&m_road_map_pos_top, &m_road_list_pos_top);
  //UtilRoad::MakeOrderedList(&m_road_map_pos_bot, &m_road_list_pos_bot);
  //UtilRoad::MakeOrderedList(&m_road_map_neg_top, &m_road_list_neg_top);
  //UtilRoad::MakeOrderedList(&m_road_map_neg_bot, &m_road_list_neg_bot);

//  m_road_list_pos_top.Add(&m_road_map_pos_top);
//  m_road_list_pos_bot.Add(&m_road_map_pos_bot);
//  m_road_list_neg_top.Add(&m_road_map_neg_top);
//  m_road_list_neg_bot.Add(&m_road_map_neg_bot);
  //m_road_list_pos_top.SortBySignal();
  //m_road_list_pos_bot.SortBySignal();
  //m_road_list_neg_top.SortBySignal();
  //m_road_list_neg_bot.SortBySignal();

  //cout << "  pos top: " << m_road_list_pos_top.Size() << endl
  //     << "  pos bot: " << m_road_list_pos_bot.Size() << endl
  //     << "  neg top: " << m_road_list_neg_top.Size() << endl
  //     << "  neg bot: " << m_road_list_neg_bot.Size() << endl
  //     << endl;

  //m_ofs << "Pos Top: First Five\n";
  //UtilRoad::PrintList(&m_road_list_pos_top, +1, +5, m_ofs);
  //m_ofs << "          Last Five\n";
  //UtilRoad::PrintList(&m_road_list_pos_top, -5, -1, m_ofs);
  //m_ofs << "Pos Bot: First Five\n";
  //UtilRoad::PrintList(&m_road_list_pos_bot, +1, +5, m_ofs);
  //m_ofs << "          Last Five\n";
  //UtilRoad::PrintList(&m_road_list_pos_bot, -5, -1, m_ofs);
  //m_ofs << "Neg Top: First Five\n";
  //UtilRoad::PrintList(&m_road_list_neg_top, +1, +5, m_ofs);
  //m_ofs << "          Last Five\n";
  //UtilRoad::PrintList(&m_road_list_neg_top, -5, -1, m_ofs);
  //m_ofs << "Neg Bot: First Five\n";
  //UtilRoad::PrintList(&m_road_list_neg_bot, +1, +5, m_ofs);
  //m_ofs << "          Last Five\n";
  //UtilRoad::PrintList(&m_road_list_neg_bot, -5, -1, m_ofs);
}

//void AnaGmc::GenerateRoadSet(const double frac_cut, const std::string rs_id)
//{
//  DisableRoadsByWeightFraction(&m_road_list_pos_top, frac_cut);
//  DisableRoadsByWeightFraction(&m_road_list_pos_bot, frac_cut);
//  DisableRoadsByWeightFraction(&m_road_list_neg_top, frac_cut);
//  DisableRoadsByWeightFraction(&m_road_list_neg_bot, frac_cut);
//  UtilRoad::WriteToFile(&m_road_map_pos_top, &m_road_map_pos_bot, &m_road_map_neg_top, &m_road_map_neg_bot, rs_id);
//}
//
//void AnaGmc::GenerateRoadSetByFoM(const double frac_cut, const std::string rs_id)
//{
//  DisableRoadsByFoM(&m_road_list_pos_top, frac_cut);
//  DisableRoadsByFoM(&m_road_list_pos_bot, frac_cut);
//  DisableRoadsByFoM(&m_road_list_neg_top, frac_cut);
//  DisableRoadsByFoM(&m_road_list_neg_bot, frac_cut);
//  UtilRoad::WriteToFile(&m_road_map_pos_top, &m_road_map_pos_bot, &m_road_map_neg_top, &m_road_map_neg_bot, rs_id);
//}
//
//void AnaGmc::DisableRoadsByWeightFraction(RoadList* road_list, const double frac)
//{
//  if (frac < 0 || frac > 1) return;
//  road_list->SortBySignal();
//
//  m_ofs << "RoadList: " << road_list->GetPosNegStr() << " " << road_list->GetTopBotStr() << "\n"
//        << "  First Five\n";
//  UtilRoad::PrintList(road_list, +1, +5, m_ofs);
//  m_ofs << "  Last Five\n";
//  UtilRoad::PrintList(road_list, -5, -1, m_ofs);
//
//  int n_ent = road_list->Size();
//  double w_tot = 0;
//  for (int ii = 0; ii < n_ent; ii++) w_tot += road_list->Get(ii)->GetWeight();
//  double w_dis = 0;
//  int i_ent = n_ent - 1;
//  while (i_ent >= 0) {
//    RoadInfo* info = road_list->Get(i_ent);
//    info->SetOnOff(false);
//    w_dis += info->GetWeight();
//    if (w_dis / w_tot >= frac) break;
//    i_ent--;
//  }
//  int n_dis = n_ent - i_ent;
//  m_ofs << "  Total   : weight = " << w_tot << ", N = " << n_ent << "\n"
//        << "  Disabled: weight = " << w_dis << ", N = " << n_dis << "\n"
//        << "                     " << w_dis/w_tot << ", " << n_dis/n_ent << endl;
//}
//
//void AnaGmc::DisableRoadsByFoM(RoadList* road_list, const double frac)
//{
//  if (frac < 0 || frac > 1) return;
//  road_list->SortByFoM();
//
//  m_ofs << "RoadList: " << road_list->GetPosNegStr() << " " << road_list->GetTopBotStr() << "\n"
//        << "  First Five\n";
//  UtilRoad::PrintList(road_list, +1, +5, m_ofs);
//  m_ofs << "  Last Five\n";
//  UtilRoad::PrintList(road_list, -5, -1, m_ofs);
//
//  int n_ent = road_list->Size();
//  double w_tot = 0;
//  for (int ii = 0; ii < n_ent; ii++) w_tot += road_list->Get(ii)->GetFoM();
//  double w_dis = 0;
//  int i_ent = n_ent - 1;
//  while (i_ent >= 0) {
//    RoadInfo* info = road_list->Get(i_ent);
//    info->SetOnOff(false);
//    w_dis += info->GetFoM();
//    if (w_dis / w_tot >= frac) break;
//    i_ent--;
//  }
//  int n_dis = n_ent - i_ent;
//  m_ofs << "  Total   : weight = " << w_tot << ", N = " << n_ent << "\n"
//        << "  Disabled: weight = " << w_dis << ", N = " << n_dis << "\n"
//        << "                     " << w_dis/w_tot << ", " << n_dis/n_ent << endl;
//}
