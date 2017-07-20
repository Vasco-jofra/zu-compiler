// $Id: postfix_writer.h,v 1.15 2016/05/20 12:47:13 ist175219 Exp $ -*- c++ -*-
#ifndef __ZU_SEMANTICS_PF_WRITER_H__
#define __ZU_SEMANTICS_PF_WRITER_H__

#include <string>
#include <iostream>
#include <cdk/symbol_table.h>
#include <cdk/emitters/basic_postfix_emitter.h>
#include "targets/basic_ast_visitor.h"
#include "targets/symbol.h"
#include "cdk/ast/expression_node.h"

namespace zu {

  //!
  //! Traverse syntax tree and generate the corresponding assembly code.
  //!
  class postfix_writer: public basic_ast_visitor {
    cdk::symbol_table<zu::symbol> &_symtab;
    cdk::basic_postfix_emitter &_pf;
    int _lbl;

    std::vector<int> _break_lbls;
    std::vector<int> _continue_lbls;

    zu::function_definition_node *_currentFunctionNode = NULL;
    int _localVariableOffset = 0;
    int _localArgumentOffset = 8;
    bool _inArgumentContext = false;

  public:
    postfix_writer(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<zu::symbol> &symtab,
                   cdk::basic_postfix_emitter &pf) :
        basic_ast_visitor(compiler), _symtab(symtab), _pf(pf), _lbl(0){
    }

  public:
    ~postfix_writer() {
      os().flush();
    }
  
  private:
    void debugPrint(const cdk::basic_node *node);
    void debugPrint(const std::string &str);

  private:
    /** Method used to generate sequential labels. */
    inline std::string mklbl(int lbl) {
      std::ostringstream oss;
      if (lbl < 0)
        oss << ".L" << -lbl;
      else
        oss << "_L" << lbl;
      return oss.str();
    }

    void handleError(const cdk::basic_node *node, const std::string &s) {
      std::cerr << "[ERROR][PSTFIX_WRITER]: " << "@line " << node->lineno() << " node " << node->name() << " failed to process" \
                << "due to: \n\t" << s << std::endl;
      exit(2);
    }

    inline std::string checkFunctionName(std::string id) {
      if(id == "zu")    return "_main";
      if(id == "_main") return "zu";
      else return id;
    }

    inline bool inGlobalContext() {
      return _currentFunctionNode == NULL;
    }
    inline bool inArgumentContext() {
      return _inArgumentContext;
    }
    inline bool isLVAL(const cdk::basic_node *node) const {
      return ((node->name() == "id_node") || (node->name() == "index_node"));
    }

  private:
    void helpUnary(cdk::unary_expression_node * const node, int lvl);
    void helpBinary(cdk::binary_expression_node * const node, int lvl);

  public:
    void do_nil_node(cdk::nil_node * const node, int lvl){} //@FIXME
  	void do_data_node(cdk::data_node * const node, int lvl){} //@FIXME
  	void do_composite_node(cdk::composite_node * const node, int lvl){} //@FIXME
    void do_sequence_node(cdk::sequence_node * const node, int lvl);
    void do_identifier_node(cdk::identifier_node * const node, int lvl){}


  public:
    void do_integer_node(cdk::integer_node * const node, int lvl);
    void do_string_node(cdk::string_node * const node, int lvl);
    void do_double_node(cdk::double_node * const node, int lvl);

  public:
    void do_neg_node(cdk::neg_node * const node, int lvl);
    void do_identity_node(zu::identity_node * const node, int lvl);

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
    void do_block_node(zu::block_node * const node, int lvl);

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

#endif
