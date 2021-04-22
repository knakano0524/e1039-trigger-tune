R__LOAD_LIBRARY(libTriggerTuning)
using namespace std;

/// Macro to generate a roadset based on the S/N FoM.
void GenerateRoad()
{
  /// 102: 4.0, 5.0, 1200, 0.010
  /// 103: 2.5, 3.5, 1200, 0.040
  /// 104: 2.5, 3.5, 1200, 0.033
  /// 105: 7.0, 8.0, 1200, 0.010
  const string    rs_id = "105";
  const double  mass_lo = 7;
  const double  mass_hi = 8;
  const int    inte_cut = 1200;
  const double frac_cut = 0.005;

  AnaGmc* ana_gmc = new AnaGmc();
  ana_gmc->SetMassRange(mass_lo, mass_hi);
  ana_gmc->Init();
  ana_gmc->ReadOneFile("sim_tree.root");
  ana_gmc->Analyze();
  ana_gmc->End(); // Do nothing (for now).

  AnaNim3Add* ana_nim3 = new AnaNim3Add();
  ana_nim3->SetInteCut(inte_cut);
  ana_nim3->Init(ana_gmc);
  ana_nim3->ReadEventsFromFileList("list_nim3.txt");
  ana_nim3->Analyze();
  ana_nim3->End(); // Do nothing (for now).

  GenRoadset* gen_rs = new GenRoadset("gen_rs" + rs_id);
  gen_rs->SetMap(ana_gmc);
  gen_rs->GenerateRoadSetByFoM(frac_cut, rs_id);

  delete gen_rs;
  delete ana_nim3;
  delete ana_gmc;
  cout << "E N D" << endl;
  exit(0);
}

/// Macro to generate a roadset based on the GMC signal yield.
void GenerateRoadBySignal()
{
  const string    rs_id = "901"; // "103"; // "101";
  const double  mass_lo = 4.0;
  const double  mass_hi = 5.0;
  const double frac_cut = 0.05;

  AnaGmc* ana_gmc = new AnaGmc();
  ana_gmc->SetMassRange(mass_lo, mass_hi);
  ana_gmc->Init();
  ana_gmc->ReadOneFile("sim_tree.root");
  ana_gmc->Analyze();

  GenRoadset* gen_rs = new GenRoadset("gen_rs" + rs_id);
  gen_rs->SetMap(ana_gmc);
  gen_rs->GenerateRoadSetBySignal(frac_cut, rs_id);

  ana_gmc->End();

  delete gen_rs;
  delete ana_gmc;
  cout << "E N D" << endl;
  exit(0);
}
