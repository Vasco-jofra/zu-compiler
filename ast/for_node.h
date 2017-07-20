// $Id: for_node.h,v 1.6 2016/04/15 01:28:36 ist175219 Exp $ -*- c++ -*-
#ifndef __ZU_FOR_NODE_H__
#define __ZU_FOR_NODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>

namespace zu {

  class for_node: public cdk::basic_node {
    cdk::sequence_node *_initialization;
    cdk::sequence_node *_condition;
    cdk::sequence_node *_increment;
    cdk::basic_node *_block;

  public:
    inline for_node(int lineno, 
                    cdk::sequence_node *initialization, 
                    cdk::sequence_node *condition, 
                    cdk::sequence_node *increment, 
                    cdk::basic_node *block) :
        basic_node(lineno), _initialization(initialization), _condition(condition), _increment(increment), _block(block){
    }

  public:
    inline cdk::sequence_node *initialization() {
      return _initialization;
    }
    inline cdk::sequence_node *condition() {
      return _condition;
    }
    inline cdk::sequence_node *increment() {
      return _increment;
    }

    inline cdk::basic_node *block() {
      return _block;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_for_node(this, level);
    }

  };

} // zu

#endif //__ZU_FOR_NODE_H__
