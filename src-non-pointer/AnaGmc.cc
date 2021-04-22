#include <iostream>
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
//#include <UtilAna/UtilTrack.h>
//#include <UtilAna/UtilDimuon.h>
//#include <UtilAna/UtilTrigger.h>
//#include <geom_svc/GeomSvc.h>
#include "AnaGmc.h"
using namespace std;

AnaGmc::AnaGmc()
{
  ;
}

AnaGmc::~AnaGmc()
{
  cout << "AnaGmc::~AnaGmc()..." << endl;
  //m_road_map_pos_top .clear();
  //m_road_map_pos_bot .clear();
  //m_road_map_neg_top .clear();
  //m_road_map_neg_bot .clear();
  //m_road_list_pos_top.clear();
  //m_road_list_pos_bot.clear();
  //m_road_list_neg_top.clear();
  //m_road_list_neg_bot.clear();
  cout << "...done." << endl;
}

void AnaGmc::ReadOneFile(const char* fname, const char* tname)
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

  const bool do_draw = false;
  if (do_draw) {
    DrawOneVar(tree, "mass", "mass", 20, 1, 6);
    DrawOneVar(tree, "x1"  , "x1"  , 20, 0, 1);
    DrawOneVar(tree, "x2"  , "x2"  , 20, 0, 1);
  }

  double weight;
  double mass;
  int road_pos;
  int road_neg;
  tree->SetBranchAddress("weight"  , &weight  );
  tree->SetBranchAddress("mass"    , &mass    );
  tree->SetBranchAddress("road_pos", &road_pos);
  tree->SetBranchAddress("road_neg", &road_neg);
  for (int i_ent = 0; i_ent < tree->GetEntries(); i_ent++) {
    tree->GetEntry(i_ent);
    if (mass < 4.0) continue;
    if (road_pos * road_neg >= 0) continue;

    RoadMap* map = &(road_pos > 0 ? m_road_map_pos_top : m_road_map_pos_bot);
    map->Add(road_pos, weight);
 
    map = &(road_neg > 0 ? m_road_map_neg_top : m_road_map_neg_bot);
    map->Add(road_neg, weight);
  }

  file->Close();
  delete file;
}

void AnaGmc::AnalyzeRoads()
{
  cout << "AnaGmc::AnalyzeRoads():" << endl;

  //m_road_list_pos_top.reserve(m_road_map_pos_top.size());
  //m_road_list_pos_bot.reserve(m_road_map_pos_bot.size());
  //m_road_list_neg_top.reserve(m_road_map_neg_top.size());
  //m_road_list_neg_bot.reserve(m_road_map_neg_bot.size());

  UtilRoad::MakeOrderedList(&m_road_map_pos_top, &m_road_list_pos_top);
  UtilRoad::MakeOrderedList(&m_road_map_pos_bot, &m_road_list_pos_bot);
  UtilRoad::MakeOrderedList(&m_road_map_neg_top, &m_road_list_neg_top);
  UtilRoad::MakeOrderedList(&m_road_map_neg_bot, &m_road_list_neg_bot);

  cout << "  pos top: " << m_road_list_pos_top.Size() << endl
       << "  pos bot: " << m_road_list_pos_bot.Size() << endl
       << "  neg top: " << m_road_list_neg_top.Size() << endl
       << "  neg bot: " << m_road_list_neg_bot.Size() << endl
       << endl;
}

void AnaGmc::DrawOneVar(TTree* tree, const char* name, const char* var, const int N, const double X0, const double X1)
{
  static TCanvas* c1 = 0;
  if (! c1) {
    gSystem->mkdir("result", true);
    c1 = new TCanvas("c1", "");
    c1->SetGrid();
    c1->SetLogy(true);
    gStyle->SetOptStat(0000);
  }

  TH1* h1_all = new TH1D("h1_all", "", N, X0, X1);
  TH1* h1_acc = new TH1D("h1_acc", "", N, X0, X1);
  TH1* h1_tb  = new TH1D("h1_tb" , "", N, X0, X1);
  h1_all->Sumw2();
  h1_acc->Sumw2();
  h1_tb ->Sumw2();
  tree->Project("h1_all", var, "weight");
  tree->Project("h1_acc", var, "weight * (road_pos*road_neg != 0)");
  tree->Project("h1_tb" , var, "weight * (road_pos*road_neg <  0)");

  h1_all->SetLineColor(kBlack);
  h1_acc->SetLineColor(kBlue);
  h1_tb ->SetLineColor(kRed);

  ostringstream oss;
  oss << "GMC;" << name << ";Weighted yield";
  THStack hs("hs", oss.str().c_str());
  hs.Add(h1_all, "E1");
  hs.Add(h1_acc, "E1");
  hs.Add(h1_tb , "E1");
  hs.Draw("nostack");

  TLegend leg (0.75, 0.75, 0.99, 0.99);
  leg.AddEntry(h1_all, "All generated", "l");
  leg.AddEntry(h1_acc, "In hodo acceptance", "l");
  leg.AddEntry(h1_tb , "T+B or B+T", "l");
  leg.SetTextFont(22);
  leg.SetBorderSize(1);
  leg.SetFillColor(0);
  leg.Draw();

  oss.str("");
  oss << "result/h1_" << name << ".png";
  c1->SaveAs(oss.str().c_str());

  delete h1_all;
  delete h1_acc;
  delete h1_tb ;
}

void AnaGmc::End()
{
  //file->cd();
  //file->Write();
  //file->Close();
}
