// $Id: dereference_node.h,v 1.4 2016/05/17 18:51:47 ist175219 Exp $
#ifndef __ZU_DEREFERENCE_NODE_H__
#define __ZU_DEREFERENCE_NODE_H__

#include <cdk/ast/unary_expression_node.h>

namespace zu {

  class dereference_node: public cdk::unary_expression_node {

  public:
    inline dereference_node(int lineno, cdk::expression_node *arg) :
        unary_expression_node(lineno, arg) {
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_dereference_node(this, level);
    }

  };

} // zu

#endif //__ZU_DEREFERENCE_NODE_H__
