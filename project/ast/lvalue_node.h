// $Id: lvalue_node.h,v 1.2 2016/04/13 21:08:59 ist175219 Exp $
#ifndef __ZU_NODE_EXPRESSION_LEFTVALUE_H__
#define __ZU_NODE_EXPRESSION_LEFTVALUE_H__

#include <cdk/ast/expression_node.h>
#include <string>

namespace zu {

  /**
   * Class for describing syntactic tree leaves for holding lvalues.
   */
  class lvalue_node: public cdk::expression_node {
  public:
    virtual ~lvalue_node() {}

    inline lvalue_node(int lineno) : 
      cdk::expression_node(lineno){
    }
  };

} // zu

#endif
