#include <string>
#include "targets/mem_counter.h"
#include <sstream>
#include "ast/all.h"  // automatically generated

void zu::mem_counter::do_sequence_node(cdk::sequence_node * const node, int lvl){
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

void zu::mem_counter::do_block_node(zu::block_node * const node, int lvl){
  if(node->decls() != NULL) {
    node->decls()->accept(this, lvl+1);
  }

  if(node->instr() != NULL) {
    node->instr()->accept(this, lvl+1);
  }
}

//---------------------------------------------------------------------------
void zu::mem_counter::do_integer_node(cdk::integer_node * const node, int lvl) {
}
void zu::mem_counter::do_double_node(cdk::double_node * const node, int lvl) { 
}
void zu::mem_counter::do_string_node(cdk::string_node * const node, int lvl) {
}
void zu::mem_counter::do_neg_node(cdk::neg_node * const node, int lvl) {
}
void zu::mem_counter::do_identity_node(zu::identity_node * const node, int lvl) {
}
void zu::mem_counter::do_dereference_node(zu::dereference_node * const node, int lvl) { 
}
void zu::mem_counter::do_add_node(cdk::add_node * const node, int lvl) {
}
void zu::mem_counter::do_sub_node(cdk::sub_node * const node, int lvl) { 
}
void zu::mem_counter::do_mul_node(cdk::mul_node * const node, int lvl) {
}
void zu::mem_counter::do_div_node(cdk::div_node * const node, int lvl) {
}
void zu::mem_counter::do_mod_node(cdk::mod_node * const node, int lvl) {
}
void zu::mem_counter::do_lt_node(cdk::lt_node * const node, int lvl) {
}
void zu::mem_counter::do_le_node(cdk::le_node * const node, int lvl) {
}
void zu::mem_counter::do_ge_node(cdk::ge_node * const node, int lvl) {
}
void zu::mem_counter::do_gt_node(cdk::gt_node * const node, int lvl) {
}
void zu::mem_counter::do_ne_node(cdk::ne_node * const node, int lvl) {
}
void zu::mem_counter::do_eq_node(cdk::eq_node * const node, int lvl) {
}
void zu::mem_counter::do_and_node(zu::and_node * const node, int lvl) {
}
void zu::mem_counter::do_or_node(zu::or_node * const node, int lvl) {
}
void zu::mem_counter::do_not_node(zu::not_node * const node, int lvl) {
}
void zu::mem_counter::do_rvalue_node(zu::rvalue_node * const node, int lvl) {
}
void zu::mem_counter::do_id_node(zu::id_node * const node, int lvl) {
}


void zu::mem_counter::do_variable_declaration_node(zu::variable_declaration_node * const node, int lvl) {
  _totalSize += node->type()->size();
}

void zu::mem_counter::do_function_definition_node(zu::function_definition_node * const node, int lvl) {
  node->statements()->accept(this, lvl+2);
}

void zu::mem_counter::do_function_call_node(zu::function_call_node * const node, int lvl) { 
  if(node->arguments()) {
    node->arguments()->accept(this, lvl+2);
  }
}

void zu::mem_counter::do_for_node(zu::for_node * const node, int lvl) {
  if(node->initialization()) {
    node->initialization()->accept(this, lvl+2);
  }
  if(node->condition()) {
    node->condition()->accept(this, lvl+2);
  }
  if(node->increment()) {
    node->increment()->accept(this, lvl+2);
  }
  if(node->block()) {
    node->block()->accept(this, lvl+2);
  }
}

void zu::mem_counter::do_if_node(zu::if_node * const node, int lvl) {
  if(node->block() != NULL) {
    node->block()->accept(this, lvl+2);
  }
}

void zu::mem_counter::do_if_else_node(zu::if_else_node * const node, int lvl) {
  if(node->thenblock() != NULL) {
    node->thenblock()->accept(this, lvl+2);
  }
  if(node->elseblock() != NULL) {
    node->elseblock()->accept(this, lvl+2);
  }
}


void zu::mem_counter::do_assignment_node(zu::assignment_node * const node, int lvl) {
}
void zu::mem_counter::do_function_declaration_node(zu::function_declaration_node * const node, int lvl) { 
}
void zu::mem_counter::do_function_return_node(zu::function_return_node * const node, int lvl) { 
}
void zu::mem_counter::do_evaluation_node(zu::evaluation_node * const node, int lvl) {
}
void zu::mem_counter::do_print_node(zu::print_node * const node, int lvl) {
}
void zu::mem_counter::do_read_node(zu::read_node * const node, int lvl) {
}
void zu::mem_counter::do_continue_node(zu::continue_node * const node, int lvl) {
}
void zu::mem_counter::do_break_node(zu::break_node * const node, int lvl) {
}
void zu::mem_counter::do_index_node(zu::index_node * const node, int lvl) { 
}
void zu::mem_counter::do_reserve_memory_node(zu::reserve_memory_node * const node, int lvl) {
}