#ifndef _ANA_BASE__H_
#define _ANA_BASE__H_
#include <string>
#include <fstream>

class AnaBase {
 protected:
  std::string m_label;
  std::string m_dir_out;
  std::ofstream m_ofs;

  AnaBase(const std::string label);

 public:
  virtual ~AnaBase();

  virtual void Init();
  virtual void Analyze();
  virtual void End();

 protected:
  virtual void ProcessOneEvent();
};

#endif // _ANA_BASE__H_ 
