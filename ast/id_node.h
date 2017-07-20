#ifndef __ZU_NODE_IDENTIFIER_NODE_H__
#define __ZU_NODE_IDENTIFIER_NODE_H__

#include "ast/lvalue_node.h"
#include <string>

namespace zu {
  class id_node: public zu::lvalue_node {
    std::string _id;

  public:
    inline id_node(int lineno, const std::string *s) :
        zu::lvalue_node(lineno), _id(*s) {
    }
    inline id_node(int lineno, const std::string &s) :
        zu::lvalue_node(lineno), _id(s) {
    }


    inline const std::string &id() {
      return _id;
    }

    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_id_node(this, level);
    }

  };

} // zu

#endif //__ZU_NODE_IDENTIFIER_NODE_H__
