// $Id: not_node.h,v 1.2 2016/03/18 02:52:02 ist175219 Exp $ -*- c++ -*-
#ifndef __ZU_AST_EXPRESSION_NOT_H__
#define __ZU_AST_EXPRESSION_NOT_H__

#include <cdk/ast/unary_expression_node.h>

namespace zu {

  class not_node: public cdk::unary_expression_node {
  public:
    /**
     * @param lineno source code line number for this node
     * @param arg operand
     */
    inline not_node(int lineno, cdk::expression_node *arg) :
        unary_expression_node(lineno, arg) {
    }

    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_not_node(this, level);
    }

  };

} // zu

#endif //__ZU_AST_EXPRESSION_NOT_H__
