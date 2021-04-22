R__LOAD_LIBRARY(libTriggerTuning)
using namespace std;

void ApplyRoad2Nim3()
{
  /// 0 or 1200
  const string rs_id = "105";
  const int inte_cut = 0; // Default = 0, no cut.

  RoadMap map_pos_top;
  RoadMap map_pos_bot;
  RoadMap map_neg_top;
  RoadMap map_neg_bot;
  UtilRoad::ReadFromFile(&map_pos_top, &map_pos_bot, &map_neg_top, &map_neg_bot, rs_id);

  ostringstream oss;
  oss << "nim3_rs" << rs_id << "_inte" << setfill('0') << setw(4) << inte_cut;
  AnaNim3* ana_nim3 = new AnaNim3(oss.str().c_str());
  ana_nim3->SetInteCut(inte_cut);
  ana_nim3->SetEnabledRoads(&map_pos_top, &map_pos_bot, &map_neg_top, &map_neg_bot);
  ana_nim3->Init();
  ana_nim3->ReadEventsFromFileList("list_nim3.txt");
  ana_nim3->Analyze();
  ana_nim3->End();

  delete ana_nim3;
  cout << "E N D" << endl;
  exit(0);
}

// ----  Note  ------------------------------------------------------------
// QIE inhibit threshold = 1200 on average in the last E906 run.
