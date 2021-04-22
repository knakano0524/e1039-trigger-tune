R__LOAD_LIBRARY(libTriggerTuning)
using namespace std;

void DrawRS(const string rs_id="105")
{
  DrawRoadset* draw = new DrawRoadset(rs_id);
  //draw->SetMaxCount(20); // default = 20
  UtilRoad::ReadFromFile(
    draw->GetRoadMapPosTop(), 
    draw->GetRoadMapPosBot(), 
    draw->GetRoadMapNegTop(), 
    draw->GetRoadMapNegBot(), 
    rs_id
    );
  draw->Draw();

  delete draw;
  cout << "E N D" << endl;
  exit(0);
}
