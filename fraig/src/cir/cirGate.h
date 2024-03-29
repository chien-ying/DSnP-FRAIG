/****************************************************************************
  FileName     [ cirGate.h ]
  PackageName  [ cir ]
  Synopsis     [ Define basic gate data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_GATE_H
#define CIR_GATE_H

#include <string>
#include <vector>
#include <iostream>
#include "cirDef.h"
#include "sat.h"

using namespace std;

// TODO: Feel free to define your own classes, variables, or functions.

class CirGate;

//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------
class CirGate
{
public:
   friend class CirMgr; // I added

   CirGate(GateType type, unsigned id, unsigned lineNo): _type(type), _id(id), _lineNo(lineNo) , _fanin(0), _fanout(0), _name("") {}
   virtual ~CirGate() {}

   // Basic access methods
   string getTypeStr() const { switch (_type) {
      case UNDEF_GATE: return "UNDEF";
      case PI_GATE:    return "PI";
      case PO_GATE:    return "PO";
      case AIG_GATE:   return "AIG";
      case CONST_GATE: return "CONST";
      default:         return "";
    } 
   }
   unsigned getLineNo() const { return _lineNo; }
   unsigned getID() const {return _id;}
   virtual bool isAig() const { return false; } // TODO

   // Printing functions
   virtual void printGate() const {}
   void reportGate() const;
   void reportFanin(int level) const;
   void reportFanout(int level) const;

   // for DFS in Mgr
   mutable unsigned _ref;

   // for DFS -fanin -fanout
   static unsigned _gmark;
   mutable unsigned _mark;
   void dfs_fanin(int level, int cur) const;
   void dfs_fanout(int level, int cur) const;


   void eraseFanIn(int del) 
   {
    for(int j = 0;j<_fanin.size();j++)
    {
      if(_fanin[j]->_id == del) _fanin.erase(_fanin.begin()+j);
      if(_fanin[j]->_id == del) _invert.erase(_invert.begin()+j);
    }  
   }
   void eraseFanOut(int del) 
   {
    for(int j = 0;j<_fanout.size();j++)
    {
      if(_fanout[j]->_id == del) _fanout.erase(_fanout.begin()+j);
      if(_fanout[j]->_id == del) _outinvert.erase(_outinvert.begin()+j);
    }
   }

   unsigned getkey()
   {
    size_t a = 0, b=0, c=0, d=0, k=0;
    bool hello = 0;
    (_fanin[1] < _fanin[0]) ? hello = 1 : hello = 0 ;
    a = _fanin[hello]->_id << 32;
    b = _fanin[!hello]->_id << 2; 
    c = _invert[!hello] << 1;
    d = _invert[hello];
    k = a + b + c + d;
    // return (_fanin[hello]->_id << 32) + (_fanin[!hello]->_id << 2) + (_invert[!hello] << 1) + _invert[hello];
    return k;
   }
   
private:

protected:
  GateType _type;
  unsigned _id;
  unsigned _lineNo;
  GateList _fanin;
  GateList _fanout;
  vector<bool> _invert;
  vector<bool> _outinvert;
  string _name;

  bool sweep = true;
  bool deleted = false;

};

class CirConstGate: public CirGate  {
public:
  CirConstGate(): CirGate(CONST_GATE, 0, 0) {}
  void printGate() const { cout << "CONST0"; } 
};

class CirPiGate: public CirGate  {
public:
  CirPiGate(unsigned id, unsigned lineNo): CirGate(PI_GATE, id, lineNo) {}
  void printGate() const { 
    cout << "PI  " << _id;
    if(_name!="") cout << " (" << _name << ")";
  }
};

class CirPoGate: public CirGate  {
public:
  CirPoGate(unsigned id, unsigned lineNo): CirGate(PO_GATE, id, lineNo) {}
  void printGate() const { cout << "PO  " << _id << " "; }
};

class CirAigGate: public CirGate {
public:
  CirAigGate(unsigned id, unsigned lineNo): CirGate(AIG_GATE, id, lineNo) {}
  void printGate() const { cout << "AIG " << _id << " "; }
};

class CirUndefGate: public CirGate  {
public:
  CirUndefGate(unsigned id): CirGate(UNDEF_GATE, id, 0) {}
  void printGate() const { cout << "UNDEF " << _id; }
};

#endif // CIR_GATE_H
