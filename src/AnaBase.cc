#include <iostream>
#include <sstream>
#include <TSystem.h>
#include "AnaBase.h"
using namespace std;

AnaBase::AnaBase(const std::string label) : m_label(label)
{
  ostringstream oss;
  oss << "result/" << m_label;
  m_dir_out = oss.str();
  gSystem->mkdir(oss.str().c_str(), true);
  oss << "/log.txt";
  m_ofs.open(oss.str().c_str());
}

AnaBase::~AnaBase()
{
  m_ofs.close();

  ostringstream oss;
  oss << m_dir_out << "/log.txt";
  string fname = oss.str();
  ifstream ifs(fname.c_str());
  cout << "+----------------------------------------------------------------+\n"
       << "| Messages in '" << fname << "':\n"
       << "+----------------------------------------------------------------+\n"
       << string(istreambuf_iterator<char>(ifs), istreambuf_iterator<char>())
       << "+----------------------------------------------------------------+\n"
       << endl;
  ifs.close();
}

void AnaBase::Init()
{
  ;
}

void AnaBase::Analyze()
{
  ;
}

void AnaBase::End()
{
  ;
}

void AnaBase::ProcessOneEvent()
{
  ;
}
