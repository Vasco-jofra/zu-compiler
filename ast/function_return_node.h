// $Id: function_return_node.h,v 1.1 2016/03/17 23:43:12 ist175219 Exp $ -*- c++ -*-
#ifndef __ZU_FUNCTIONRETURN_NODE_H__
#define __ZU_FUNCTIONRETURN_NODE_H__

#include <cdk/ast/basic_node.h>

namespace zu {
  class function_return_node: public cdk::basic_node {
  public:
    inline function_return_node(int lineno) :
        cdk::basic_node(lineno) {
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_return_node(this, level);
    }

  };

} // zu

#endif //__ZU_FUNCTIONRETURN_NODE_H__
