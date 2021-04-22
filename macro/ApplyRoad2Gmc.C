R__LOAD_LIBRARY(libTriggerTuning)
using namespace std;

void ApplyRoad2Gmc()
{
  /// 101, 4.0, 5.0
  /// 102, 4.0, 5.0
  /// 103, 2.5, 3.5
  /// 104, 2.5, 3.5
  /// 105, 7.0, 8.0
  const string   rs_id = "101";
  const double mass_lo = 4;
  const double mass_hi = 5;

  AnaGmcApp* ana_gmc = new AnaGmcApp("gmc_app_rs" + rs_id);
  ana_gmc->SetMassRange(mass_lo, mass_hi);
  ana_gmc->Init();
  UtilRoad::ReadFromFile(
    ana_gmc->GetRoadMapPosTop(), 
    ana_gmc->GetRoadMapPosBot(), 
    ana_gmc->GetRoadMapNegTop(), 
    ana_gmc->GetRoadMapNegBot(), 
    rs_id
    );
  ana_gmc->ReadOneFile("sim_tree.root");
  ana_gmc->Analyze();
  ana_gmc->End();

  delete ana_gmc;
  cout << "E N D" << endl;
  exit(0);
}

/*

QIE inhibit threshold = 1200 in the last E906 run.

Expected counts per spill = [N of "fired" events] / [N of all events] * [N of filled RFs]

N of filled RFs = 186e6

 */
