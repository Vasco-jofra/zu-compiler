#ifndef __ZU__IDENTITY__NODE_H__
#define __ZU__IDENTITY__NODE_H__

#include <cdk/ast/unary_expression_node.h>

namespace zu {


  class identity_node: public cdk::unary_expression_node {
  public:
    inline identity_node(int lineno, expression_node *arg) :
        unary_expression_node(lineno, arg) {
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_identity_node(this, level);
    }

  };

} // cdk

#endif //__ZU__IDENTITY__NODE_H__