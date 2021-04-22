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
//#include <geom_svc/GeomSvc.h>
#include "AnaNim3.h"
using namespace std;

AnaNim3::AnaNim3() 
  : m_n_evt_used(0)
  , m_n_evt_tb  (0)
  , m_h1_evt_cnt(0)
{
  ;
}

AnaNim3::~AnaNim3() 
{
  if (m_h1_evt_cnt) delete m_h1_evt_cnt;
  //gROOT->cd();
}

void AnaNim3::ReadFilesInList(const char* fn_list, const int n_max)
{
  vector<string> list_fname;
  ifstream ifs(fn_list);
  string fname;
  while (ifs >> fname) list_fname.push_back(fname);
  ifs.close();
  int n_fname = list_fname.size();
  cout << "N of NIM3 files = " << n_fname << endl;
  for (int ii = 0; ii < n_fname; ii++) {
    if (n_max > 0 && ii == n_max) break;
    cout << "." << flush;
    if ((ii+1) % 50 == 0) cout << " " << (ii+1) << endl;
    //cout << "File " << setw(3) << ii << " / " << n_fname << endl;
    ReadOneFile(list_fname[ii].c_str());
  }
  cout << endl;
}

/**
 * Read one ROOT file, which is usually "data_nim3.root" created by e1039-analysis/AnaRealDst/src/AnaNim3Event.
 */
void AnaNim3::ReadOneFile(const char* fname, const char* tname)
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

  int run;
  int evt;
  int inte_max;
  int fpga1;
  EleList* h1t = 0;
  EleList* h2t = 0;
  EleList* h3t = 0;
  EleList* h4t = 0;
  EleList* h1b = 0;
  EleList* h2b = 0;
  EleList* h3b = 0;
  EleList* h4b = 0;
  tree->SetBranchAddress("run"     , &run     );
  tree->SetBranchAddress("evt"     , &evt     );
  tree->SetBranchAddress("inte_max", &inte_max);
  tree->SetBranchAddress("fpga1"   , &fpga1   );
  tree->SetBranchAddress("h1t"     , &h1t     );
  tree->SetBranchAddress("h2t"     , &h2t     );
  tree->SetBranchAddress("h3t"     , &h3t     );
  tree->SetBranchAddress("h4t"     , &h4t     );
  tree->SetBranchAddress("h1b"     , &h1b     );
  tree->SetBranchAddress("h2b"     , &h2b     );
  tree->SetBranchAddress("h3b"     , &h3b     );
  tree->SetBranchAddress("h4b"     , &h4b     );
  for (int i_ent = 0; i_ent < tree->GetEntries(); i_ent++) {
    tree->GetEntry(i_ent);
    //if (inte_max > 1300) continue;
    m_n_evt_used++;

    int n_top = FindAllRoads(h1t, h2t, h3t, h4t, +1, m_road_map_top);
    int n_bot = FindAllRoads(h1b, h2b, h3b, h4b, -1, m_road_map_bot);
    if (n_top > 0 && n_bot > 0) m_n_evt_tb++;
  }
  file->Close();
  delete file;
}

int AnaNim3::FindAllRoads(const EleList* h1, const EleList* h2, const EleList* h3, const EleList* h4, const int tb, RoadMap& road_map)
{
  int n_road = 0;
  if (abs(tb) != 1) return n_road;
  for (EleList::const_iterator it1 = h1->begin(); it1 != h1->end(); it1++) {
  for (EleList::const_iterator it2 = h2->begin(); it2 != h2->end(); it2++) {
  for (EleList::const_iterator it3 = h3->begin(); it3 != h3->end(); it3++) {
  for (EleList::const_iterator it4 = h4->begin(); it4 != h4->end(); it4++) {
    int road = UtilTrigger::Hodo2Road(*it1, *it2, *it3, *it4, tb);
    road_map[road].Add(1);
    n_road++;
  }
  }
  }
  }
  return n_road;
}

void AnaNim3::AnalyzeRoads()
{
  cout << "AnaNim3::AnalyzeRoads():\n"
       << "Event Counts:\n";
  for (int ib = 1; ib < m_h1_evt_cnt->GetNbinsX(); ib++) {
    double cont = m_h1_evt_cnt->GetBinContent(ib);
    if (cont == 0) break;
    cout << setw(4) << ib << " " << cont << "\n";
  }
  cout << "\n"
       << "N of analyzed events:\n"
       << "  Used  " << m_n_evt_used << "\n"
       << "  T+B   " << m_n_evt_tb << "\n"
       << endl;

  m_road_list_top.reserve(m_road_map_top.size());
  m_road_list_bot.reserve(m_road_map_bot.size());
  UtilRoad::MakeOrderedList(m_road_map_top, m_road_list_top);
  UtilRoad::MakeOrderedList(m_road_map_bot, m_road_list_bot);
  cout << "  N of top fired roads: " << m_road_list_top.size() << endl
       << "  N of bot fired roads: " << m_road_list_bot.size() << endl
       << endl;
}

void AnaNim3::End()
{
  //file->cd();
  //file->Write();
  //file->Close();
}
