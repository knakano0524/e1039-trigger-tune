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
#include "RoadMap.h"
#include "RoadList.h"
#include "UtilRoad.h"
#include "AnaNim3.h"
using namespace std;

AnaNim3::AnaNim3(const std::string label)
  : AnaBase(label)
  , m_n_evt_used (0)
  , m_n_evt_tb   (0)
  , m_n_evt_fired(0)
  , m_h1_evt_cnt (0)
  , m_road_map_pos_top_on(0)
  , m_road_map_pos_bot_on(0)
  , m_road_map_neg_top_on(0)
  , m_road_map_neg_bot_on(0)
  , m_inte_cut(0)
{
  ;
}

AnaNim3::~AnaNim3() 
{
  if (m_h1_evt_cnt) delete m_h1_evt_cnt;
}

void AnaNim3::Init()
{
  ostringstream oss;
  oss << m_dir_out << "/output.root";
  m_file_out = new TFile(oss.str().c_str(), "RECREATE");
  m_h1_inte_max = new TH1D("h1_inte_max"  , "", 200, 0, 20e3);

  m_ofs << "Intensity cut = " << m_inte_cut << endl;
}

void AnaNim3::End()
{
  m_file_out->cd();
  m_file_out->Write();
  m_file_out->Close();
}

void AnaNim3::ReadEventsFromFileList(const char* fn_list, const int n_max)
{
  vector<string> list_fname;
  ifstream ifs(fn_list);
  string fname;
  while (ifs >> fname) list_fname.push_back(fname);
  ifs.close();
  int n_fname = list_fname.size();

  m_ofs << "N of NIM3 files = " << n_fname << endl;
  cout  << "N of NIM3 files = " << n_fname << endl;
  for (int ii = 0; ii < n_fname; ii++) {
    if (n_max > 0 && ii == n_max) break;
    cout << "." << flush;
    if ((ii+1) % 50 == 0) cout << " " << (ii+1) << endl;
    //cout << "File " << setw(3) << ii << " / " << n_fname << endl;
    ReadEventsFromFile(list_fname[ii].c_str());
  }
  cout << endl;
}

/**
 * Read one ROOT file, which is usually "data_nim3.root" created by e1039-analysis/AnaRealDst/src/AnaNim3Event.
 */
void AnaNim3::ReadEventsFromFile(const char* fname, const char* tname)
{
  gROOT->cd();
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

  TH1* h1_evt_cnt = (TH1*)file->Get("h1_evt_cnt");
  assert(h1_evt_cnt);
  if (! m_h1_evt_cnt) {
    gROOT->cd();
    m_h1_evt_cnt = (TH1*)h1_evt_cnt->Clone("h1_evt_cnt_tot");
  } else {
    m_h1_evt_cnt->Add(h1_evt_cnt);
  }

  b_h1t = b_h2t = b_h3t = b_h4t = b_h1b = b_h2b = b_h3b = b_h4b = 0;
  tree->SetBranchAddress("run"     , &b_run     );
  tree->SetBranchAddress("evt"     , &b_evt     );
  tree->SetBranchAddress("inte_max", &b_inte_max);
  tree->SetBranchAddress("fpga1"   , &b_fpga1   );
  tree->SetBranchAddress("h1t"     , &b_h1t     );
  tree->SetBranchAddress("h2t"     , &b_h2t     );
  tree->SetBranchAddress("h3t"     , &b_h3t     );
  tree->SetBranchAddress("h4t"     , &b_h4t     );
  tree->SetBranchAddress("h1b"     , &b_h1b     );
  tree->SetBranchAddress("h2b"     , &b_h2b     );
  tree->SetBranchAddress("h3b"     , &b_h3b     );
  tree->SetBranchAddress("h4b"     , &b_h4b     );
  for (int i_ent = 0; i_ent < tree->GetEntries(); i_ent++) {
    tree->GetEntry(i_ent);
    ProcessOneEvent();
  }
  file->Close();
  delete file;
}

void AnaNim3::ProcessOneEvent()
{
  if (b_inte_max == 0) return; // Some readout error
  m_n_evt_used++;

  m_h1_inte_max->Fill(b_inte_max);

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
  
  if (m_road_map_pos_top_on) {
    bool fired = false;
    if (map_top.ContainEnabled(m_road_map_pos_top_on) &&
        map_bot.ContainEnabled(m_road_map_neg_bot_on)   ) fired = true;
    if (map_top.ContainEnabled(m_road_map_neg_top_on) &&
        map_bot.ContainEnabled(m_road_map_pos_bot_on)   ) fired = true;
    if (! fired) return;
  }
  m_n_evt_fired++;
  
  m_road_map_top.AddBG(&map_top);
  m_road_map_bot.AddBG(&map_bot);
}

void AnaNim3::FindAllRoads(const EleList* h1, const EleList* h2, const EleList* h3, const EleList* h4, const int tb, RoadMap* road_map)
{
  if (abs(tb) != 1) return;
  for (EleList::const_iterator it1 = h1->begin(); it1 != h1->end(); it1++) {
  for (EleList::const_iterator it2 = h2->begin(); it2 != h2->end(); it2++) {
  for (EleList::const_iterator it3 = h3->begin(); it3 != h3->end(); it3++) {
  for (EleList::const_iterator it4 = h4->begin(); it4 != h4->end(); it4++) {
    int road = UtilTrigger::Hodo2Road(*it1, *it2, *it3, *it4, tb);
    road_map->AddBG(road, 1.0);
  }
  }
  }
  }
}

void AnaNim3::Analyze()
{
  cout << "AnaNim3::Analyze():" << endl;

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

  DrawInteMax();

  RoadList road_list_top;
  RoadList road_list_bot;
  road_list_top.Add(&m_road_map_top);
  road_list_bot.Add(&m_road_map_bot);
  road_list_top.SortByBG();
  road_list_bot.SortByBG();
  m_ofs << "  N of fired roads @ Top: " << road_list_top.Size() << endl
        << "  N of fired roads @ Bot: " << road_list_bot.Size() << endl
        << endl;

  m_ofs << "Top: First Five\n";
  UtilRoad::PrintList(&road_list_top, +1, +5, m_ofs);
  m_ofs << "      Last Five\n";
  UtilRoad::PrintList(&road_list_top, -5, -1, m_ofs);
  m_ofs << "Bot: First Five\n";
  UtilRoad::PrintList(&road_list_bot, +1, +5, m_ofs);
  m_ofs << "      Last Five\n";
  UtilRoad::PrintList(&road_list_bot, -5, -1, m_ofs);
}

void AnaNim3::SetEnabledRoads(const RoadMap* pos_top, const RoadMap* pos_bot, const RoadMap* neg_top, const RoadMap* neg_bot)
{
  m_road_map_pos_top_on = pos_top;
  m_road_map_pos_bot_on = pos_bot;
  m_road_map_neg_top_on = neg_top;
  m_road_map_neg_bot_on = neg_bot;
}

void AnaNim3::DrawInteMax()
{
  int bin_cut = m_h1_inte_max->FindBin(m_inte_cut) - 1;
  double frac_acc = m_h1_inte_max->Integral(0, bin_cut) / m_h1_inte_max->Integral();
  m_ofs << "NIM3 event fraction = " << frac_acc << " @ inte_max < " << m_inte_cut << endl;

  TCanvas* c1 = new TCanvas("c1", "");
  c1->SetGrid();
  c1->SetLogy(true);
  gStyle->SetOptStat(0000);

  m_h1_inte_max->SetTitle("NIM3;Max intensity;Yield");

  m_h1_inte_max->GetXaxis()->SetRangeUser(0, 5000);
  m_h1_inte_max->Draw();
  ostringstream oss;
  oss << m_dir_out << "/h1_inte_max.png";
  c1->SaveAs(oss.str().c_str());

  m_h1_inte_max->GetXaxis()->SetRange();
  m_h1_inte_max->Rebin(10);
  m_h1_inte_max->Draw();
  oss.str("");
  oss << m_dir_out << "/h1_inte_max_all.png";
  c1->SaveAs(oss.str().c_str());

  delete c1;
}
