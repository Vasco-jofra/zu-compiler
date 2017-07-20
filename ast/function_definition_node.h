// $Id: function_definition_node.h,v 1.5 2016/05/18 15:46:29 ist175219 Exp $ -*- c++ -*-
#ifndef __ZU_FUNCTIONDEFINITION_NODE_H__
#define __ZU_FUNCTIONDEFINITION_NODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/basic_type.h>
#include <string>

namespace zu {
  class function_definition_node: public cdk::basic_node {
  	zu::id_node *_name;
  	basic_type *_returnType;
    cdk::expression_node *_defaultReturnValue;
  	cdk::sequence_node* _arguments;
    cdk::basic_node *_statements;
  	bool _isPublic;
    bool _isExtern;

  public:
    inline function_definition_node(int lineno, 
                                    zu::id_node *name, 
                                    basic_type *returnType, 
                                    cdk::expression_node *defaultReturnValue, 
                                    cdk::sequence_node* arguments, 
                                    cdk::basic_node *statements,
                                    bool isPublic, bool isExtern) :
        cdk::basic_node(lineno), _name(name), _returnType(returnType), 
        _defaultReturnValue(defaultReturnValue), _arguments(arguments), 
        _statements(statements), _isPublic(isPublic), _isExtern(isExtern) {
    }

  public:
	inline zu::id_node *name() {
		return _name;
	}
	inline basic_type *type() {
		return _returnType;
	}
	inline cdk::sequence_node* arguments() {
		return _arguments;
	}
  inline cdk::basic_node *statements() {
    return _statements;
  }
  inline cdk::expression_node *defaultReturnValue() {
    return _defaultReturnValue;
  }
  inline bool isPublic() {
    return _isPublic;
  }

  inline bool isExtern() {
    return _isExtern;
  }
  void accept(basic_ast_visitor *sp, int level) {
    sp->do_function_definition_node(this, level);
  }

  };

} // zu

#endif
