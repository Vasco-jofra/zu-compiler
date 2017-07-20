// $Id: index_node.h,v 1.3 2016/04/13 21:08:59 ist175219 Exp $
#ifndef __ZU_INDEX_NODE_H__
#define __ZU_INDEX_NODE_H__

#include <cdk/ast/expression_node.h>
#include "ast/lvalue_node.h"

namespace zu {
  class index_node: public zu::lvalue_node {
    cdk::expression_node *_pointer;
    cdk::expression_node *_position;

  public:
    inline index_node(int lineno, cdk::expression_node *pointer, cdk::expression_node *position) :
        zu::lvalue_node(lineno), _pointer(pointer), _position(position) {
    }

  public:
    inline cdk::expression_node *pointer() {
      return _pointer;
    }

	inline cdk::expression_node *position() {
		return _position;
	}

  void accept(basic_ast_visitor *sp, int level) {
    sp->do_index_node(this, level);
  }

  };

} // zu

#endif //__ZU_INDEX_NODE_H__
