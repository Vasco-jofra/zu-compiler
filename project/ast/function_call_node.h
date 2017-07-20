// $Id: function_call_node.h,v 1.3 2016/04/15 01:28:36 ist175219 Exp $ -*- c++ -*-
#ifndef __ZU_FUNCTIONCALL_NODE_H__
#define __ZU_FUNCTIONCALL_NODE_H__

#include <cdk/ast/basic_node.h>
#include <string>

namespace zu {
  class function_call_node: public cdk::expression_node {
	zu::id_node *_name;
	cdk::sequence_node* _arguments;

  public:
    inline function_call_node(int lineno, zu::id_node *name, cdk::sequence_node* arguments) :
        cdk::expression_node(lineno), _name(name), _arguments(arguments) {
    }
  public:
	inline zu::id_node *name() {
		return _name;
	}
	inline cdk::sequence_node* arguments() {
		return _arguments;
	}

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_call_node(this, level);
    }

  };

} // zu

#endif //__ZU_FUNCTIONCALL_NODE_H__
