// $Id: type_checker.cpp,v 1.15 2016/05/20 12:47:13 ist175219 Exp $ -*- c++ -*-
#include <string>
#include "targets/type_checker.h"
#include <sstream>
#include "ast/all.h"  // automatically generated


#define ASSERT_UNSPEC \
    { if (node->type() != nullptr && \
          node->type()->name() != basic_type::TYPE_UNSPEC) return; }

#define DEBUG_PRINT { debugPrint(node); }

#define DEBUG_INSERTION(id) { \
  std::ostringstream buffer; \
  buffer << "Iserted " << id << " in the symbol table."; \
  debugPrint(buffer.str()); \
}

#define DEBUG_SEARCH(id) { \
  std::ostringstream buffer; \
  buffer << "Looked for " << id << " in the symbol table."; \
  debugPrint(buffer.str()); \
}


#define __TYPE_CHECKER_DEBUG__

inline void zu::type_checker::debugPrint(const cdk::basic_node *node) {
  #ifdef __TYPE_CHECKER_DEBUG__
    std::cout << "[DEBUG][TYPE_CHECKER]: <-- " << node->name() << " -->" << std::endl;
  #endif
}

inline void zu::type_checker::debugPrint(const std::string &str) {
  #ifdef __TYPE_CHECKER_DEBUG__
    std::cout << "[DEBUG][TYPE_CHECKER]:   " << str << std::endl;
  #endif
}


void zu::type_checker::do_block_node(zu::block_node * const node, int lvl){
  // Nothing to do here
}

//---------------------------------------------------------------------------

void zu::type_checker::do_integer_node(cdk::integer_node * const node, int lvl) {
  ASSERT_UNSPEC;
  DEBUG_PRINT;
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void zu::type_checker::do_double_node(cdk::double_node * const node, int lvl) { 
  ASSERT_UNSPEC;
  DEBUG_PRINT;
  node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
}

void zu::type_checker::do_string_node(cdk::string_node * const node, int lvl) {
  ASSERT_UNSPEC;
  DEBUG_PRINT;
  node->type(new basic_type(4, basic_type::TYPE_STRING));
}


//---------------------------------------------------------------------------
//UNARY_NODES----------------------------------------------------------------
//---------------------------------------------------------------------------
inline void zu::type_checker::processUnaryExpression(cdk::unary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  // Check if argument is an integer or a double
  if(node->argument()->type()->name() != basic_type::TYPE_INT &&
     node->argument()->type()->name() != basic_type::TYPE_DOUBLE)
    throw std::string("wrong type in argument of unary expression");

  node->type(node->argument()->type());
}

void zu::type_checker::do_neg_node(cdk::neg_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
  DEBUG_PRINT;
}
void zu::type_checker::do_identity_node(zu::identity_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
  DEBUG_PRINT;
}

void zu::type_checker::do_dereference_node(zu::dereference_node * const node, int lvl) { 
  ASSERT_UNSPEC;
  DEBUG_PRINT;
  node->argument()->accept(this, lvl + 2);
  

  basic_type *newType = new basic_type(4, basic_type::TYPE_POINTER);
  newType->_subtype = node->argument()->type();
  node->type(newType); // (4, TYPE_POINTER)
}



//---------------------------------------------------------------------------
//BINARY_NODES---------------------------------------------------------------
//---------------------------------------------------------------------------
// Check if arguments are of type int double or pointer and no pointer double operations 
inline void zu::type_checker::initialProcessOfAdditiveBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;
  DEBUG_PRINT;
  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() != basic_type::TYPE_INT &&
      node->left()->type()->name() != basic_type::TYPE_DOUBLE &&
      node->left()->type()->name() != basic_type::TYPE_POINTER) {
    throw std::string("wrong type in left argument of a additive binary expression");
  }

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() != basic_type::TYPE_INT &&
      node->right()->type()->name() != basic_type::TYPE_DOUBLE &&
      node->right()->type()->name() != basic_type::TYPE_POINTER) {
    throw std::string("wrong type in right argument of a additive binary expression");
  }

  // Check that we are not adding/subtracting pointers with doubles
  if(node->left()->type()->name() == basic_type::TYPE_POINTER &&
     node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    throw std::string("pointer with double in a additive binary expression");
  }
  
  if(node->left()->type()->name() == basic_type::TYPE_DOUBLE &&
     node->right()->type()->name() == basic_type::TYPE_POINTER) {
    throw std::string("pointer with double in a additive binary expression");
  }

  // Determine which type we are
  if(node->left()->type()->name() == basic_type::TYPE_INT &&
     node->right()->type()->name() == basic_type::TYPE_POINTER) {
    node->type(node->left()->type()); //int
  } 
  else if(node->left()->type()->name() == basic_type::TYPE_POINTER &&
          node->right()->type()->name() == basic_type::TYPE_INT) {
    node->type(node->left()->type()); //int
  }
  else if(node->left()->type()->name() == basic_type::TYPE_DOUBLE ||
          node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  } 
  else {
    node->type(new basic_type(4, basic_type::TYPE_INT)); 
  }
}

void zu::type_checker::do_add_node(cdk::add_node * const node, int lvl) {
  initialProcessOfAdditiveBinaryExpression(node, lvl);
  DEBUG_PRINT;

  // pointer with pointer is impossible
  if(node->left()->type()->name() == basic_type::TYPE_POINTER &&
     node->right()->type()->name() == basic_type::TYPE_POINTER) {
    throw std::string("pointer with pointer in a add operation is impossible");
  }
}

void zu::type_checker::do_sub_node(cdk::sub_node * const node, int lvl) {
  initialProcessOfAdditiveBinaryExpression(node, lvl);
  DEBUG_PRINT;

  // pointer with pointer is impossible
  if(node->left()->type()->name() == basic_type::TYPE_POINTER &&
     node->right()->type()->name() == basic_type::TYPE_POINTER) {
    if(node->left()->type()->subtype() != node->right()->type()->subtype()) {
      throw std::string("pointers have different types in a sub operation");
    }
    node->type(new basic_type(4, basic_type::TYPE_INT)); 
  }
}


// Check if arguments are of type int or double 
inline void zu::type_checker::processMultiplicativeBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;

  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() != basic_type::TYPE_INT &&
      node->left()->type()->name() != basic_type::TYPE_DOUBLE)
    throw std::string("wrong type in left argument of a multiplicative binary expression");

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() != basic_type::TYPE_INT &&
      node->right()->type()->name() != basic_type::TYPE_DOUBLE)
    throw std::string("wrong type in right argument of a multiplicative binary expression");

  // Determine which type we are
  if(node->left()->type()->name() == basic_type::TYPE_DOUBLE ||
     node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  } else {
    node->type(new basic_type(4, basic_type::TYPE_INT)); 
  } 
}

// Check if arguments are of type int
inline void zu::type_checker::processIntegerOnlyBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;

  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in left argument of a binary expression");

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in right argument of a binary expression");

  node->type(new basic_type(4, basic_type::TYPE_INT)); 
}

inline void zu::type_checker::processComparativeBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;

  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() != basic_type::TYPE_INT &&
      node->left()->type()->name() != basic_type::TYPE_DOUBLE)
    throw std::string("wrong type in left argument of a comparative binary expression");

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() != basic_type::TYPE_INT &&
      node->right()->type()->name() != basic_type::TYPE_DOUBLE)
    throw std::string("wrong type in right argument of a comparative binary expression");

  node->type(new basic_type(4, basic_type::TYPE_INT));
}

// int double or pointer with arguments of the same type
inline void zu::type_checker::processEqualityBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;

  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() != basic_type::TYPE_INT &&
      node->left()->type()->name() != basic_type::TYPE_DOUBLE &&
      node->left()->type()->name() != basic_type::TYPE_POINTER)
    throw std::string("wrong type in left argument of a equality binary expression");

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() != basic_type::TYPE_INT &&
      node->right()->type()->name() != basic_type::TYPE_DOUBLE &&
      node->right()->type()->name() != basic_type::TYPE_POINTER)
    throw std::string("wrong type in right argument of a equality binary expression");

  // Check if arguments are of the same type
  // @SEE: Should allow int with double?
  if (node->left()->type()->name() != node->right()->type()->name()) { 
        throw std::string("arguments of equality binary expression are not of the same type");
  }

  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void zu::type_checker::do_mul_node(cdk::mul_node * const node, int lvl) {
  processMultiplicativeBinaryExpression(node, lvl);
  DEBUG_PRINT;
}
void zu::type_checker::do_div_node(cdk::div_node * const node, int lvl) {
  processMultiplicativeBinaryExpression(node, lvl);
  DEBUG_PRINT;
}
void zu::type_checker::do_mod_node(cdk::mod_node * const node, int lvl) {
  processIntegerOnlyBinaryExpression(node, lvl);
  DEBUG_PRINT;
}
void zu::type_checker::do_lt_node(cdk::lt_node * const node, int lvl) {
  processComparativeBinaryExpression(node, lvl);
  DEBUG_PRINT;
}
void zu::type_checker::do_le_node(cdk::le_node * const node, int lvl) {
  processComparativeBinaryExpression(node, lvl);
  DEBUG_PRINT;
}
void zu::type_checker::do_ge_node(cdk::ge_node * const node, int lvl) {
  processComparativeBinaryExpression(node, lvl);
  DEBUG_PRINT;
}
void zu::type_checker::do_gt_node(cdk::gt_node * const node, int lvl) {
  processComparativeBinaryExpression(node, lvl);
  DEBUG_PRINT;
}
void zu::type_checker::do_ne_node(cdk::ne_node * const node, int lvl) {
  processEqualityBinaryExpression(node, lvl);
  DEBUG_PRINT;
}
void zu::type_checker::do_eq_node(cdk::eq_node * const node, int lvl) {
  processEqualityBinaryExpression(node, lvl);
  DEBUG_PRINT;
}

//---------------------------------------------------------------------------
//LOGICAL_OPERATIONS---------------------------------------------------------
//---------------------------------------------------------------------------
// Check arguments are all integers
inline void zu::type_checker::processLogicalBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;

  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in left argument of a logical binary expression");

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in right argument of a logical binary expression");

  node->type(new basic_type(4, basic_type::TYPE_INT)); 
}

void zu::type_checker::do_and_node(zu::and_node * const node, int lvl) {
  processLogicalBinaryExpression(node, lvl);
  DEBUG_PRINT;
}
void zu::type_checker::do_or_node(zu::or_node * const node, int lvl) {
  processLogicalBinaryExpression(node, lvl);
  DEBUG_PRINT;
}
void zu::type_checker::do_not_node(zu::not_node * const node, int lvl) {
  ASSERT_UNSPEC;
  DEBUG_PRINT;
  if(node->argument()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in argument not expression");

  node->type(new basic_type(4, basic_type::TYPE_INT));
}

//---------------------------------------------------------------------------

void zu::type_checker::do_rvalue_node(zu::rvalue_node * const node, int lvl) {
  ASSERT_UNSPEC;
  DEBUG_PRINT;
  node->lvalue()->accept(this, lvl);
  node->type(node->lvalue()->type());
}

//---------------------------------------------------------------------------

void zu::type_checker::do_id_node(zu::id_node * const node, int lvl) {
  ASSERT_UNSPEC;
  DEBUG_PRINT;

  const std::string &id = node->id();
  std::shared_ptr<zu::symbol> symbol = _symtab.find(id);

  if (!symbol) 
    throw id + " undeclared";
  
  DEBUG_SEARCH(id);
  
  node->type(symbol->type());
}

//---------------------------------------------------------------------------

void zu::type_checker::do_variable_declaration_node(zu::variable_declaration_node * const node, int lvl) {
  DEBUG_PRINT;

  const std::string &id = node->identifier()->id();
  if (!_symtab.insert(id, std::make_shared<zu::symbol>(node->type(), id, false, false)))
    throw id + " redeclared";
  DEBUG_INSERTION(id);


  if (node->rvalue()) {
    // set is defined as true
    std::shared_ptr<zu::symbol> symbol = _symtab.find(id);
    symbol->setIsDefined(true);

    node->rvalue()->accept(this, lvl+2);
    
    if(node->type()->name() == basic_type::TYPE_POINTER && 
       node->rvalue()->type()->name() == basic_type::TYPE_INT){
      //Allow throught ponter = integer

    } else if (node->type()->name() == basic_type::TYPE_DOUBLE && 
               node->rvalue()->type()->name() == basic_type::TYPE_INT) {
      // Conversão implícita de int para double, Nao faz nada

    } else if (node->type()->name() != node->rvalue()->type()->name()) {
      throw std::string("wrong type for initializer");

    }
  }
}

void zu::type_checker::do_assignment_node(zu::assignment_node * const node, int lvl) {
  ASSERT_UNSPEC;
  DEBUG_PRINT;

  node->lvalue()->accept(this, lvl+2);
  node->rvalue()->accept(this, lvl+2);

  if(node->lvalue()->type()->name() == basic_type::TYPE_POINTER && 
     node->rvalue()->type()->name() == basic_type::TYPE_INT){
    //Allow throught ponter = integer

  } else if (node->lvalue()->type()->name() == basic_type::TYPE_DOUBLE && 
      node->rvalue()->type()->name() == basic_type::TYPE_INT) {
    // Conversão implícita de int para double

  } else if (node->lvalue()->type()->name() != node->rvalue()->type()->name()) {
    std::ostringstream buffer; 
    buffer << "Wrong types in assignement: " << getBasicTypeName(node->lvalue()->type()) << " = " << getBasicTypeName(node->rvalue()->type());
    throw buffer.str();
  }

  node->type(node->lvalue()->type());
}

//---------------------------------------------------------------------------

void zu::type_checker::do_function_declaration_node(zu::function_declaration_node * const node, int lvl) { 
  DEBUG_PRINT;
  
  const std::string &id = node->name()->id();
  if (!_symtab.insert(id, std::make_shared<zu::symbol>(node->type(), id, true, false)))
    throw id + " redeclared";
  
  DEBUG_INSERTION(id);
}

void zu::type_checker::do_function_definition_node(zu::function_definition_node * const node, int lvl) {
  DEBUG_PRINT;

  const std::string &id = node->name()->id();
  std::shared_ptr<zu::symbol> symbol = _symtab.find(id);
  DEBUG_SEARCH(id);

  // Simbolo não foi declarado ainda
  if(!symbol) {
    _symtab.insert(id, std::make_shared<zu::symbol>(node->type(), id, true, true));
    DEBUG_INSERTION(id);
    
    symbol = _symtab.find(id);

  } else { // Simbolo já foi inserido
    if(symbol->isFunction() == false) {
      throw std::string("Trying to define a variable as a function.");
    }
    // Caso exista symbol verificar se já não foi definido
    if(symbol->isDefined() == true) {
      throw std::string("Redefinition of function.");

    } else {
      symbol->setIsDefined(true);
    }
  }
  if(!symbol) 
    throw std::string("Compiler bug! dafuq");

  if(node->defaultReturnValue() == NULL) { 
    debugPrint("Default return value is NULL");
    return;
  }

  node->defaultReturnValue()->accept(this, lvl+2);
  if(node->defaultReturnValue()->type()->name() == basic_type::TYPE_INT &&
    symbol->type()->name() == basic_type::TYPE_DOUBLE){

  } else if(node->defaultReturnValue()->type()->name() != symbol->type()->name()) {
    throw std::string("Default return value of funtion doesn't match the return type of the function");
  }
}

void zu::type_checker::do_function_call_node(zu::function_call_node * const node, int lvl) { 
  ASSERT_UNSPEC;
  DEBUG_PRINT;

  const std::string &id = node->name()->id();
  std::shared_ptr<zu::symbol> symbol = _symtab.find(id);
  DEBUG_SEARCH(id);

  if (!symbol) {
    //id + " undeclared"; Let it pass in case it is extern
    {std::ostringstream buffer; 
    buffer << "ID: " << id << " in undeclared in the symbol table in a function call.";
    debugPrint(buffer.str());}

    _symtab.insert(id, std::make_shared<zu::symbol>(node->type(), id, true, false)); 
  }
  
  if(!symbol->isFunction())
    throw id + " is not a function";
  
  node->name()->type(symbol->type());

  //node->name()->accept(this, lvl+2); //Cant be this because of special funtion name
  node->type(node->name()->type());
}

void zu::type_checker::do_function_return_node(zu::function_return_node * const node, int lvl) { 
}

//---------------------------------------------------------------------------

void zu::type_checker::do_evaluation_node(zu::evaluation_node * const node, int lvl) {
  DEBUG_PRINT;
  node->argument()->accept(this, lvl + 2);
}

void zu::type_checker::do_print_node(zu::print_node * const node, int lvl) {
  DEBUG_PRINT;

  node->argument()->accept(this, lvl + 2);
  
  if (node->argument()->type()->name() != basic_type::TYPE_INT &&
      node->argument()->type()->name() != basic_type::TYPE_STRING &&
      node->argument()->type()->name() != basic_type::TYPE_DOUBLE)
    throw std::string("wrong type in write expression");
}

//---------------------------------------------------------------------------

void zu::type_checker::do_read_node(zu::read_node * const node, int lvl) {
  
}

//---------------------------------------------------------------------------
void zu::type_checker::do_for_node(zu::for_node * const node, int lvl) {
  
}

void zu::type_checker::do_continue_node(zu::continue_node * const node, int lvl) {
  
}
void zu::type_checker::do_break_node(zu::break_node * const node, int lvl) {
  
}

//---------------------------------------------------------------------------

void zu::type_checker::do_if_node(zu::if_node * const node, int lvl) {
  DEBUG_PRINT;
  node->condition()->accept(this, lvl + 2);
}

void zu::type_checker::do_if_else_node(zu::if_else_node * const node, int lvl) {
  DEBUG_PRINT;
  node->condition()->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void zu::type_checker::do_index_node(zu::index_node * const node, int lvl) { 
  ASSERT_UNSPEC;
  DEBUG_PRINT;

  node->pointer()->accept(this, lvl+2);
  node->position()->accept(this, lvl+2);

  if(node->pointer()->type()->name() != basic_type::TYPE_POINTER && 
     node->position()->type()->name() != basic_type::TYPE_INT) {
    throw std::string("wrong type in index node");
  }

  node->type(node->pointer()->type()->subtype());
  std::cout << "INDEX_NODE value: " << getBasicTypeName(node->pointer()->type()->subtype()) << std::endl;
}

void zu::type_checker::do_reserve_memory_node(zu::reserve_memory_node * const node, int lvl) {
  ASSERT_UNSPEC;
  DEBUG_PRINT;

  node->size()->accept(this, lvl+2);
  if(node->size()->type()->name() != basic_type::TYPE_INT) {
    throw std::string("wrong type in size of reserve_memory_node");
  }
  node->type(new basic_type(4, basic_type::TYPE_POINTER));
  node->type()->_subtype = new basic_type(0, basic_type::TYPE_UNSPEC);
}

