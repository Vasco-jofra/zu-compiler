// $Id: variable_declaration_node.h,v 1.7 2016/04/15 11:28:21 ist175219 Exp $ -*- c++ -*-
#ifndef __ZU_VARDECL_H__
#define __ZU_VARDECL_H__

#include "ast/lvalue_node.h"
#include <string>

namespace zu {
  class variable_declaration_node: public cdk::basic_node {
  	basic_type *_type;
    zu::id_node *_identifier;
    cdk::expression_node *_rvalue;
  	bool _isPublic;
    bool _isExtern;

  public:
    inline variable_declaration_node(int lineno, 
                                     basic_type *type, 
                                     zu::id_node* identifier, 
                                     cdk::expression_node *rvalue, 
                                     bool isPublic, bool isExtern) :
        cdk::basic_node(lineno), _type(type), _identifier(identifier), 
        _rvalue(rvalue), _isPublic(isPublic), _isExtern(isExtern){
    }

  public:
    inline basic_type *type() {
      return _type;
    }
    inline cdk::expression_node *rvalue() {
      return _rvalue;
    }
    inline zu::id_node *identifier() {
      return _identifier;
    }
    inline bool isPublic() {
      return _isPublic;
    }

    inline bool isExtern() {
      return _isExtern;
    }
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_variable_declaration_node(this, level);
    }

  };

} // zu

#endif //__ZU_VARDECL_H__
