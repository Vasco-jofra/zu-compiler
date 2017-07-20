// $Id: reserve_memory_node.h,v 1.5 2016/04/15 03:02:55 ist175219 Exp $
#ifndef __ZU_MEMRES_NODE_H__
#define __ZU_MEMRES_NODE_H__

#include "ast/lvalue_node.h"

namespace zu {
  class reserve_memory_node: public cdk::expression_node {
  	cdk::expression_node *_size;

  public:
    inline reserve_memory_node(int lineno, cdk::expression_node *size) :
        cdk::expression_node(lineno), _size(size) {
    }

  public:
	inline cdk::expression_node* size() {
		return _size;
	}

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_reserve_memory_node(this, level);
    }

  };

} // zu

#endif //__ZU_MEMRES_NODE_H__
