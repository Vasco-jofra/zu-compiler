// $Id: mem_counter.h,v 1.2 2016/05/20 09:40:44 ist175219 Exp $ -*- c++ -*-
#ifndef __ZU_MEM_COUNTER_H__
#define __ZU_MEM_COUNTER_H__

#include <string>
#include <iostream>
#include <cdk/symbol_table.h>
#include <cdk/ast/basic_node.h>
#include "targets/symbol.h"
#include "targets/basic_ast_visitor.h"

namespace zu {

  /**
   * Print nodes as XML elements to the output stream.
   */
  class mem_counter: public basic_ast_visitor {
    cdk::symbol_table<zu::symbol> &_symtab;

    int _totalSize = 0;

  public:
    mem_counter(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<zu::symbol> &symtab) :
        basic_ast_visitor(compiler), _symtab(symtab) {
    }

  public:
    ~mem_counter() {

    }

  public:
    int getSize() const {
      return _totalSize;
    }

  private:
    void debugPrint(const cdk::basic_node *node);
    void debugPrint(const std::string &str);
    
  public:
  	void do_nil_node(cdk::nil_node * const node, int lvl){}
    void do_data_node(cdk::data_node * const node, int lvl){}
    void do_composite_node(cdk::composite_node * const node, int lvl) {}
    void do_sequence_node(cdk::sequence_node * const node, int lvl);
    void do_identifier_node(cdk::identifier_node * const node, int lvl) {}

  public:
    void do_integer_node(cdk::integer_node * const node, int lvl);
    void do_string_node(cdk::string_node * const node, int lvl);
    void do_double_node(cdk::double_node * const node, int lvl);

  public:
    void do_neg_node(cdk::neg_node * const node, int lvl);
    void do_identity_node(zu::identity_node * const node, int lvl);

  public:
    void do_block_node(zu::block_node * const node, int lvl);


  public:
    void do_add_node(cdk::add_node * const node, int lvl);
    void do_sub_node(cdk::sub_node * const node, int lvl);
    void do_mul_node(cdk::mul_node * const node, int lvl);
    void do_div_node(cdk::div_node * const node, int lvl);
    void do_mod_node(cdk::mod_node * const node, int lvl);
    void do_lt_node(cdk::lt_node * const node, int lvl);
    void do_le_node(cdk::le_node * const node, int lvl);
    void do_ge_node(cdk::ge_node * const node, int lvl);
    void do_gt_node(cdk::gt_node * const node, int lvl);
    void do_ne_node(cdk::ne_node * const node, int lvl);
    void do_eq_node(cdk::eq_node * const node, int lvl);

  public:
  	void do_and_node(zu::and_node * const node, int lvl);
  	void do_or_node(zu::or_node * const node, int lvl);
  	void do_not_node(zu::not_node * const node, int lvl);

  public:
    void do_id_node(zu::id_node * const node, int lvl);
    void do_rvalue_node(zu::rvalue_node * const node, int lvl);

  public:
  	void do_function_definition_node(zu::function_definition_node * const node, int lvl);
  	void do_function_declaration_node(zu::function_declaration_node * const node, int lvl);
  	void do_function_call_node(zu::function_call_node * const node, int lvl);
  	void do_function_return_node(zu::function_return_node * const node, int lvl);

  public:
    void do_evaluation_node(zu::evaluation_node * const node, int lvl);
    void do_print_node(zu::print_node * const node, int lvl);
    void do_read_node(zu::read_node * const node, int lvl);
    void do_assignment_node(zu::assignment_node * const node, int lvl);
    void do_variable_declaration_node(zu::variable_declaration_node * const node, int lvl);

  public:
  	void do_for_node(zu::for_node * const node, int lvl);
  	void do_continue_node(zu::continue_node * const node, int lvl);
  	void do_break_node(zu::break_node * const node, int lvl);
    void do_if_node(zu::if_node * const node, int lvl);
    void do_if_else_node(zu::if_else_node * const node, int lvl);

  public:
  	void do_index_node(zu::index_node * const node, int lvl);
  	void do_dereference_node(zu::dereference_node * const node, int lvl);
  	void do_reserve_memory_node(zu::reserve_memory_node * const node, int lvl);
  };

} // zu

#define GET_SIZE(compiler, symtab, node, value) { \
  zu::mem_counter counter(compiler, symtab); \
  (node)->accept(&counter, 0); \
  value = counter.getSize(); \
}

#endif
