// $Id: break_node.h,v 1.1 2016/03/17 23:43:11 ist175219 Exp $ -*- c++ -*-
#ifndef __CDK_BREAKNODE_H__
#define __CDK_BREAKNODE_H__

namespace zu {

  class break_node: public cdk::basic_node {
  public:
    inline break_node(int lineno) :
        basic_node(lineno){
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_break_node(this, level);
    }

  };

} // zu

#endif //__CDK_BREAKNODE_H__
