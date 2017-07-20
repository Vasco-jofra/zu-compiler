// $Id: xml_writer.cpp,v 1.15 2016/05/18 15:46:29 ist175219 Exp $ -*- c++ -*-
#include <string>
#include "targets/xml_writer.h"
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

//---------------------------------------------------------------------------

void zu::xml_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<sequence_node size='" << node->size() << "'>" << std::endl;
  for (size_t i = 0; i < node->size(); i++) {
    if(node == NULL) continue;
    node->node(i)->accept(this, lvl + 2);
  }
  closeTag(node, lvl); 
}

void zu::xml_writer::do_block_node(zu::block_node * const node, int lvl){
  openTag(node, lvl);
  if(node->decls() != NULL) {
    openTag("declarations", lvl + 2);
    node->decls()->accept(this, lvl + 4);
    closeTag("declarations", lvl + 2);
  }
  if(node->instr() != NULL) {
    openTag("instructions", lvl + 2);
    node->instr()->accept(this, lvl + 4);
    closeTag("instructions", lvl + 2);
  }
  closeTag(node, lvl);
}

void do_nil_node(cdk::nil_node * const node, int lvl);


//---------------------------------------------------------------------------

void zu::xml_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  processSimple(node, lvl);
}

void zu::xml_writer::do_double_node(cdk::double_node * const node, int lvl) {
  processSimple(node, lvl);
}

void zu::xml_writer::do_string_node(cdk::string_node * const node, int lvl) {
  processSimple(node, lvl);
}

//---------------------------------------------------------------------------

inline void zu::xml_writer::processUnaryExpression(cdk::unary_expression_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void zu::xml_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}

void zu::xml_writer::do_identity_node(zu::identity_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}

void zu::xml_writer::do_not_node(zu::not_node * const node, int lvl) { 
  processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

inline void zu::xml_writer::processBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void zu::xml_writer::do_add_node(cdk::add_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_div_node(cdk::div_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_le_node(cdk::le_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_and_node(zu::and_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_or_node(zu::or_node * const node, int lvl) { 
  processBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_rvalue_node(zu::rvalue_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_id_node(zu::id_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  os() << std::string(lvl, ' ') << "<" << node->name() << ">" << node->id() << "</" << node->name() << ">" << std::endl;
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_assignment_node(zu::assignment_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  
  openTag("lvalue", lvl + 2);
  node->lvalue()->accept(this, lvl + 4);
  openTag("lvalue", lvl + 2);

  openTag("rvalue", lvl + 2);
  node->rvalue()->accept(this, lvl + 4);
  closeTag("rvalue", lvl + 2);
 
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_variable_declaration_node(zu::variable_declaration_node * const node, int lvl) { 
  CHECK_TYPES(_compiler, _symtab, node);
  os() << std::string(lvl, ' ') << "<variable_declaration_node isPublic='" << node->isPublic() 
                                << "' isExtern='" << node->isExtern() << "'>" << std::endl;

  processBasicType(node->type(), lvl + 2);

  openTag("identifier", lvl + 2);
  node->identifier()->accept(this, lvl + 4);
  closeTag("identifier", lvl + 2);
 
  if(node->rvalue() != NULL) {
    openTag("rvalue", lvl + 2);
    node->rvalue()->accept(this, lvl + 4);
    closeTag("rvalue", lvl + 2);
  }
  
  closeTag(node, lvl);    
}

void zu::xml_writer::do_function_definition_node(zu::function_definition_node * const node, int lvl) { 
  CHECK_TYPES(_compiler, _symtab, node);
  os() << std::string(lvl, ' ') << "<function_definition_node isPublic='" << node->isPublic() 
                                << "' isExtern='" << node->isExtern() << "'>" << std::endl;

  node->name()->accept(this, lvl + 2);

  processBasicType(node->type(), lvl + 2);

  if(node->defaultReturnValue() != NULL) {
    openTag("defaultReturnValue", lvl + 2);
    node->defaultReturnValue()->accept(this, lvl + 4);
    closeTag("defaultReturnValue", lvl + 2);
  }

  if(node->arguments() != NULL) {
    openTag("arguments", lvl + 2);
    node->arguments()->accept(this, lvl + 4);
    closeTag("arguments", lvl + 2);
  }

  if(node->statements() != NULL) {
    openTag("statements", lvl + 2);
    node->statements()->accept(this, lvl + 4);
    closeTag("statements", lvl + 2);
  }

  closeTag(node, lvl); 
}

void zu::xml_writer::do_function_declaration_node(zu::function_declaration_node * const node, int lvl) { 
  CHECK_TYPES(_compiler, _symtab, node);
  os() << std::string(lvl, ' ') << "<function_declaration_node isPublic='" << node->isPublic() 
                                << "' isExtern='" << node->isExtern() << "'>" << std::endl;

  node->name()->accept(this, lvl + 2);

  processBasicType(node->type(), lvl + 2);

  if(node->arguments() != NULL) {
    openTag("arguments", lvl + 2);
    node->arguments()->accept(this, lvl + 4);
    closeTag("arguments", lvl + 2);
  }

  closeTag(node, lvl);
}
void zu::xml_writer::do_function_call_node(zu::function_call_node * const node, int lvl) { 
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);

  node->name()->accept(this, lvl + 2);

  if(node->arguments() != NULL) {
    openTag("arguments", lvl + 2);
    node->arguments()->accept(this, lvl + 4);
    closeTag("arguments", lvl + 2);
  }
  
  closeTag(node, lvl); 
}
void zu::xml_writer::do_function_return_node(zu::function_return_node * const node, int lvl) { 
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);

  closeTag(node, lvl); 
}
//---------------------------------------------------------------------------

void zu::xml_writer::do_evaluation_node(zu::evaluation_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void zu::xml_writer::do_print_node(zu::print_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_read_node(zu::read_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_for_node(zu::for_node * const node, int lvl) { 
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  
  if(node->initialization() != NULL) {
    openTag("initialization", lvl + 2);
    node->initialization()->accept(this, lvl + 4);
    closeTag("initialization", lvl + 2);
  }
  
  if(node->condition() != NULL) {
    openTag("condition", lvl + 2);
    node->condition()->accept(this, lvl + 4);
    closeTag("condition", lvl + 2);
  }
  if(node->increment() != NULL) {
    openTag("increment", lvl + 2);
    node->increment()->accept(this, lvl + 4);
    closeTag("increment", lvl + 2);
  }

  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);
  
  closeTag(node, lvl);
}

void zu::xml_writer::do_continue_node(zu::continue_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  openTag(node, lvl);
  closeTag(node, lvl);
}

void zu::xml_writer::do_break_node(zu::break_node * const node, int lvl) { 
  CHECK_TYPES(_compiler, _symtab, node);

  openTag(node, lvl);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_if_node(zu::if_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  closeTag(node, lvl);
}

void zu::xml_writer::do_if_else_node(zu::if_else_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->thenblock()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  openTag("else", lvl + 2);
  node->elseblock()->accept(this, lvl + 4);
  closeTag("else", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_index_node(zu::index_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  openTag(node, lvl);

  openTag("pointer", lvl + 2);
  node->pointer()->accept(this, lvl + 4);
  closeTag("pointer", lvl + 2);

  openTag("position", lvl + 2);
  node->position()->accept(this, lvl + 4);
  closeTag("position", lvl + 2);

  closeTag(node, lvl);
}
void zu::xml_writer::do_dereference_node(zu::dereference_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  openTag(node, lvl);

  openTag("lvalue", lvl + 2);
  node->argument()->accept(this, lvl + 4);
  closeTag("lvalue", lvl + 2);

  closeTag(node, lvl);
}
void zu::xml_writer::do_reserve_memory_node(zu::reserve_memory_node * const node, int lvl) { 
  CHECK_TYPES(_compiler, _symtab, node);
  
  openTag(node, lvl);

  openTag("size", lvl + 2);
  node->size()->accept(this, lvl + 4);
  closeTag("size", lvl + 2);

  closeTag(node, lvl);
}
