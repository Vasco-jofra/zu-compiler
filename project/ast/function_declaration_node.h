// $Id: function_declaration_node.h,v 1.5 2016/05/18 15:46:29 ist175219 Exp $ -*- c++ -*-
#ifndef __ZU_FUNCIONDECL_H__
#define __ZU_FUNCIONDECL_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>
#include <cdk/basic_type.h>
#include <string>

namespace zu {
  class function_declaration_node: public cdk::basic_node {
  	zu::id_node *_name;
  	basic_type *_returnType;
  	cdk::sequence_node *_arguments;
  	bool _isPublic;
    bool _isExtern;

  public:
    inline function_declaration_node(int lineno, 
                                     zu::id_node *name, 
                                     basic_type *returnType, 
                                     cdk::sequence_node* arguments, 
                                     bool isPublic, bool isExtern) :
        cdk::basic_node(lineno), _name(name), _returnType(returnType), 
        _arguments(arguments), _isPublic(isPublic), _isExtern(isExtern) {
	
    }

	inline zu::id_node *name() {
		return _name;
	}
	inline basic_type *type() {
		return _returnType;
	}
	inline cdk::sequence_node* arguments() {
		return _arguments;
	}
    inline bool isPublic() {
      return _isPublic;
    }

    inline bool isExtern() {
      return _isExtern;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_declaration_node(this, level);
    }

  };

} // zu

#endif
