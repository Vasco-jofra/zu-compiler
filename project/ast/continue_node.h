// $Id: continue_node.h,v 1.1 2016/03/17 23:43:11 ist175219 Exp $ -*- c++ -*-
#ifndef __CDK_CONTINUENODE_H__
#define __CDK_CONTINUENODE_H__

namespace zu {

  class continue_node: public cdk::basic_node {
  public:
    inline continue_node(int lineno) :
        basic_node(lineno){
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_continue_node(this, level);
    }

  };

} // zu

#endif //__CDK_CONTINUENODE_H__
