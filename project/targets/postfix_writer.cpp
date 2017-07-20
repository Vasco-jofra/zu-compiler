#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/mem_counter.h"
#include "targets/postfix_writer.h"
#include "ast/all.h"  // all.h is automatically generated


#define DEBUG_PRINT { debugPrint(node); }
#define DEBUG_WITH_ARG(id, msg) { \
  std::ostringstream buffer; \
  buffer << id << ": " << msg; \
  debugPrint(buffer.str()); \
}

//#define __POSTFIX_DEBUG__

inline void zu::postfix_writer::debugPrint(const cdk::basic_node *node) {
  #ifdef __POSTFIX_DEBUG__
    std::cout << "[DEBUG][POSTFIX_WRITER]: <-- " << node->name() << " -->" << std::endl;
  #endif
}

inline void zu::postfix_writer::debugPrint(const std::string &str) {
  #ifdef __POSTFIX_DEBUG__
    std::cout << "[DEBUG][POSTFIX_WRITER]:   " << str << std::endl;
  #endif
}

//---------------------------------------------------------------------------
//     THIS IS THE VISITOR'S DEFINITION
//---------------------------------------------------------------------------
#define MY_LOAD(node) { \
  if(node->type()->name() == basic_type::TYPE_DOUBLE) _pf.DLOAD(); \
  else _pf.LOAD(); \
}

#define MY_STORE(node) { \
  if(node->type()->name() == basic_type::TYPE_DOUBLE) _pf.DSTORE(); \
  else _pf.STORE(); \
}

#define MY_DUP(node) { \
  if(node->type()->name() == basic_type::TYPE_DOUBLE) _pf.DDUP(); \
  else _pf.DUP(); \
}


void zu::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  DEBUG_PRINT;
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

void zu::postfix_writer::do_block_node(zu::block_node * const node, int lvl){ DEBUG_PRINT;
  // Push a context
  _symtab.push();

  if(node->decls() != NULL) {
    node->decls()->accept(this, lvl+2);
  }

  if(node->instr() != NULL) {
    node->instr()->accept(this, lvl+2);
  }

  // Destroy the current context
  _symtab.pop();
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) { DEBUG_PRINT;
  CHECK_TYPES(_compiler, _symtab, node);
  
  if(inGlobalContext()) {
    _pf.CONST(node->value());
  } else {
    _pf.INT(node->value());
  }
}

void zu::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) { DEBUG_PRINT;
  CHECK_TYPES(_compiler, _symtab, node);
  
  if(inGlobalContext()) {
    _pf.DOUBLE(node->value());
    return;
  }
  
  int lbl1;

  /* generate the double */
  _pf.RODATA(); // literal is DATA readonly
  _pf.ALIGN(); // make sure we are aligned
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the double a name
  _pf.DOUBLE(node->value()); // output string characters

  /* leave the address on the stack */
  _pf.TEXT(); // return to the TEXT segment
  _pf.ADDR(mklbl(lbl1)); // the string to be printed
  _pf.DLOAD();
}

void zu::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) { DEBUG_PRINT;
  CHECK_TYPES(_compiler, _symtab, node);

  int lbl1;

  /* generate the string */
  _pf.RODATA(); // strings are DATA readonly
  _pf.ALIGN(); // make sure we are aligned
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the string a name
  _pf.STR(node->value()); // output string characters

  if (inGlobalContext()) {
    _pf.DATA();
    _pf.ID(mklbl(lbl1));
  }
  else { /* leave the address on the stack */
    _pf.TEXT(); // return to the TEXT segment
    _pf.ADDR(mklbl(lbl1)); // the string to be printed
  }
}

//---------------------------------------------------------------------------
inline void zu::postfix_writer::helpUnary(cdk::unary_expression_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl+2); 

  if(isLVAL(node->argument())) {
    MY_LOAD(node->argument());
  }
}
void zu::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) { DEBUG_PRINT;
  helpUnary(node, lvl);
  
  if(node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DNEG();
  } else {
    _pf.NEG(); // 2-complement
  }
}

void zu::postfix_writer::do_identity_node(zu::identity_node * const node, int lvl) { DEBUG_PRINT;
  helpUnary(node, lvl);
  // Nothing more to do
}

void zu::postfix_writer::do_not_node(zu::not_node * const node, int lvl) { DEBUG_PRINT; 
  helpUnary(node, lvl);
  _pf.NOT();
}

//---------------------------------------------------------------------------
inline void zu::postfix_writer::helpBinary(cdk::binary_expression_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  
  node->left()->accept(this, lvl+2);
  if(isLVAL(node->left())) {
    MY_LOAD(node->left());
  }

  // Conversion to double
  if((node->type()->name() == basic_type::TYPE_DOUBLE)
     && (node->left()->type()->name() == basic_type::TYPE_INT)) {
    _pf.I2D();
  }

  node->right()->accept(this, lvl+2);
  if(isLVAL(node->right())) {
    MY_LOAD(node->right());
  }

  // Conversion to double
  if((node->type()->name() == basic_type::TYPE_DOUBLE)
     && (node->right()->type()->name() == basic_type::TYPE_INT)) {
    _pf.I2D();
  }
}

void zu::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) { DEBUG_PRINT;
  helpBinary(node, lvl);

  if(node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DADD();
  } else {
    _pf.ADD();
  }
}
void zu::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) { DEBUG_PRINT;
  helpBinary(node, lvl);

  if(node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DSUB();
  } else {
    _pf.SUB();
  }
}
void zu::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) { DEBUG_PRINT;
  helpBinary(node, lvl);

  if(node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DMUL();
  } else {
    _pf.MUL();
  }
}
void zu::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) { DEBUG_PRINT;
  helpBinary(node, lvl);

  if(node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DIV();
  } else {
    _pf.DIV();
  }
}
void zu::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) { DEBUG_PRINT;
  CHECK_TYPES(_compiler, _symtab, node);
  
  node->left()->accept(this, lvl+2);
  if(isLVAL(node->left())) {
    MY_LOAD(node->left());
  }
  node->right()->accept(this, lvl+2);
  if(isLVAL(node->right())) {
    MY_LOAD(node->right());
  }
  _pf.MOD();
}


//---------------------------------------------------------------------------
void zu::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) { DEBUG_PRINT;
  helpBinary(node, lvl);

  if(node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  _pf.LT();
}
void zu::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) { DEBUG_PRINT;
  helpBinary(node, lvl);

  if(node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  _pf.LE();
}
void zu::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) { DEBUG_PRINT;
  helpBinary(node, lvl);

  if(node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  _pf.GE();
}
void zu::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) { DEBUG_PRINT;
  helpBinary(node, lvl);

  if(node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  _pf.GT();
}
void zu::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) { DEBUG_PRINT;
  helpBinary(node, lvl);

  if(node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  _pf.NE();
}
void zu::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) { DEBUG_PRINT;
  helpBinary(node, lvl);

  if(node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  _pf.EQ();
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_and_node(zu::and_node * const node, int lvl) { DEBUG_PRINT;
  CHECK_TYPES(_compiler, _symtab, node);
  
  int endLbl = (++_lbl);

  node->left()->accept(this, lvl+2);

  MY_DUP(node->left());
  _pf.JZ(mklbl(endLbl));

  if(isLVAL(node->left())) {
    MY_LOAD(node->left());
  }

  node->right()->accept(this, lvl+2);
  if(isLVAL(node->right())) {
    MY_LOAD(node->right());
  }

  _pf.AND();

  _pf.ALIGN();
  _pf.LABEL(mklbl(endLbl));
}
void zu::postfix_writer::do_or_node(zu::or_node * const node, int lvl) { DEBUG_PRINT;
 CHECK_TYPES(_compiler, _symtab, node);
  
  int endLbl = (++_lbl);

  node->left()->accept(this, lvl+2);

  MY_DUP(node->left());
  _pf.JNZ(mklbl(endLbl));

  if(isLVAL(node->left())) {
    MY_LOAD(node->left());
  }

  node->right()->accept(this, lvl+2);
  if(isLVAL(node->right())) {
    MY_LOAD(node->right());
  }

  _pf.OR();

  _pf.ALIGN();
  _pf.LABEL(mklbl(endLbl));
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_rvalue_node(zu::rvalue_node * const node, int lvl) { DEBUG_PRINT;
  CHECK_TYPES(_compiler, _symtab, node);
  node->lvalue()->accept(this, lvl);
  
  MY_LOAD(node);
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_id_node(zu::id_node * const node, int lvl) { DEBUG_PRINT;
  CHECK_TYPES(_compiler, _symtab, node);

  std::shared_ptr<zu::symbol> symbol = _symtab.find(node->id());

  if(symbol->isGlobal()) {
    _pf.ADDR(node->id());
  } else {
    _pf.LOCAL(symbol->offset());
  }
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_assignment_node(zu::assignment_node * const node, int lvl) { DEBUG_PRINT;
  CHECK_TYPES(_compiler, _symtab, node);

  // Visit the right child
  node->rvalue()->accept(this, lvl+2);
  if(isLVAL(node->rvalue())) { // if is a lval
    MY_LOAD(node->rvalue());
  }

  if(node->rvalue()->type()->name() == basic_type::TYPE_INT &&
     node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.I2D();
  }

  // For setting the return value
  if((inGlobalContext() == false) && (node->lvalue()->name() == "id_node")) {
    DEBUG_WITH_ARG("", "<<IN LOCAL ASSIGNMENT WITH LVALUE AS ID_NODE.>>");
    id_node *idNode = (id_node *)node->lvalue();
    std::shared_ptr<zu::symbol> symbol = _symtab.find(idNode->id());

    // Se for a função atual
    if((symbol->isFunction() == true) && (symbol->name() == _currentFunctionNode->name()->id())) {
      // Actualizar return value
      int retOffset = -(_currentFunctionNode->type()->size());
      _pf.LOCA(retOffset);
      DEBUG_WITH_ARG(symbol->name(), "<- UPDATED RETURN VALUE.");
      return;
    }     
  }

  MY_DUP(node);

  node->lvalue()->accept(this, lvl+2);
  MY_STORE(node);
}

void zu::postfix_writer::do_variable_declaration_node(zu::variable_declaration_node * const node, int lvl) { DEBUG_PRINT;
  CHECK_TYPES(_compiler, _symtab, node);
  std::shared_ptr<zu::symbol> symbol = _symtab.find(node->identifier()->id());

  if(inGlobalContext()) { //global context
    symbol->setIsGlobal(true);
    const std::string &newLbl = symbol->name();

    if(!node->rvalue()) { // Variable uninitialized
      DEBUG_WITH_ARG(symbol->name(), "Declaring GLOBAL variable WITHOUT assignment.");
      _pf.BSS();
      _pf.ALIGN();
      // @SEE: Do we need to do stuff with public stuff here?
      _pf.GLOBAL(newLbl, _pf.OBJ());
      _pf.LABEL(newLbl);
      _pf.BYTE(node->type()->size());
      
    } else { // Variable is initialized
      DEBUG_WITH_ARG(symbol->name(), "Declaring GLOBAL variable WITH assignment.");
      _pf.DATA();
      _pf.ALIGN();
      _pf.GLOBAL(newLbl, _pf.OBJ());
      _pf.LABEL(newLbl);
      node->rvalue()->accept(this, lvl+2);
    }

  } else { // local context 
    symbol->setIsGlobal(false);

    if(inArgumentContext()) { // argument (use _localArgumentOffset)
      DEBUG_WITH_ARG(symbol->name(), "Declaring LOCAL varible in ARGUMENT context.");
      symbol->setOffset(_localArgumentOffset);
      _localArgumentOffset += symbol->type()->size();

    } else { // local vars in function (use _localVariableOffset)
      DEBUG_WITH_ARG(symbol->name(), "Declaring LOCAL varible in LOCAL_VARIABLE context.");
      _localVariableOffset -= symbol->type()->size();
      symbol->setOffset(_localVariableOffset);
    }

    if(node->rvalue()) { // Variable initialized
      DEBUG_WITH_ARG(symbol->name(), "Declaring LOCAL variable WITH assignment.");

      zu::assignment_node* assignment = new zu::assignment_node(node->lineno(), node->identifier(), node->rvalue());
      assignment->accept(this, lvl+2);

    } else { // Variable uninitialized
      DEBUG_WITH_ARG(symbol->name(), "Declaring LOCAL variable WITHOUT assignment.");
    }
  }
}
//---------------------------------------------------------------------------
void zu::postfix_writer::do_function_declaration_node(zu::function_declaration_node * const node, int lvl) { DEBUG_PRINT;
  CHECK_TYPES(_compiler, _symtab, node);
  // All done in the check types. Nothing to do here.
  if(node->isExtern()) {
    //_pf.EXTERN(node->name()->id()); 
    // This commented passes more testes, because aparently you can make an extern funtion and then define it
  }
}

void zu::postfix_writer::do_function_definition_node(zu::function_definition_node * const node, int lvl) { DEBUG_PRINT;
  CHECK_TYPES(_compiler, _symtab, node);

  std::string unalteredId = node->name()->id();
  std::string id = checkFunctionName(node->name()->id());
  _currentFunctionNode = node;
  _localVariableOffset = 0;
  _localArgumentOffset = 8; // "Jump" FP and RetAddr
  
  _symtab.push();

  _pf.TEXT();
  _pf.ALIGN();
  _pf.GLOBAL(id, _pf.FUNC());
  _pf.LABEL(id);

  // Alocar espaço para as variaveis
  int enterSize = 0;
  GET_SIZE(_compiler, _symtab, node, enterSize);
  // Acrescentar o tamanho da variável de retorno
  enterSize += node->type()->size();
  _pf.ENTER(enterSize);
  DEBUG_WITH_ARG(enterSize, " <- Size of ENTER.");

  // Create the return variable
  _localVariableOffset -= node->type()->size();
  if(node->defaultReturnValue() != NULL){
    DEBUG_WITH_ARG("", "Return value setup.");
    node->defaultReturnValue()->accept(this, lvl+2);
    _pf.LOCA(_localVariableOffset);
  } else {
    DEBUG_WITH_ARG(node->defaultReturnValue(), "No return value by default.");
  }

  _inArgumentContext = true;
  node->arguments()->accept(this, lvl+2);
  _inArgumentContext = false;

  DEBUG_WITH_ARG("", "Processing Statements.");
  // Processar os statements todos
  node->statements()->accept(this, lvl+2);

  DEBUG_WITH_ARG("", "Returning form function.");
  // Fazer return 
  function_return_node* retNode = new function_return_node(node->lineno());
  retNode->accept(this, lvl+2); 

  _symtab.pop();
  _currentFunctionNode = NULL; 
  _localVariableOffset = 0;
  _localArgumentOffset = 0;


  if(id == "_main") {
    _pf.EXTERN("readi");
    _pf.EXTERN("readd");
    _pf.EXTERN("printi");
    _pf.EXTERN("printd");
    _pf.EXTERN("prints");
    _pf.EXTERN("println");
    _pf.EXTERN("argc");
    _pf.EXTERN("argv");
    _pf.EXTERN("envp");
  }
}

void zu::postfix_writer::do_function_call_node(zu::function_call_node * const node, int lvl) { DEBUG_PRINT; 
  CHECK_TYPES(_compiler, _symtab, node);
  
  int size = 0;
  // Put the arguments in the stack
  if(node->arguments() != NULL) {
    node->arguments()->accept(this, lvl+2);
    GET_SIZE(_compiler, _symtab, node, size);
  }

  // Call the funtion
  _pf.CALL(node->name()->id());

  _pf.TRASH(size);
  if(node->type()->name() == basic_type::TYPE_VOID) {
    //Do nothing
  } else if (node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DPUSH();
  } else {
    _pf.PUSH();
  }
}

void zu::postfix_writer::do_function_return_node(zu::function_return_node * const node, int lvl) { DEBUG_PRINT;
  if(_currentFunctionNode == NULL) {
    handleError(node, "Return outside of function.");
    return;
  }

  if(_currentFunctionNode->type()->name() != basic_type::TYPE_VOID) { 
    int retOffset = -(_currentFunctionNode->type()->size());
    _pf.LOCV(retOffset);
    _pf.POP();
  }

  _pf.LEAVE();
  _pf.RET();
}
//---------------------------------------------------------------------------

void zu::postfix_writer::do_evaluation_node(zu::evaluation_node * const node, int lvl) { DEBUG_PRINT;
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value
  _pf.TRASH(node->argument()->type()->size());
}

//@SEE: check if correct
void zu::postfix_writer::do_print_node(zu::print_node * const node, int lvl) { DEBUG_PRINT;
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value to print
  
  if(isLVAL(node->argument())) {
    DEBUG_WITH_ARG(node->name(), " <- is LVAL.");
    MY_LOAD(node->argument());

  } else {
    DEBUG_WITH_ARG(node->name(), " <- is not LVAL.");
  }

  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.CALL("printi");
    _pf.TRASH(4); // delete the printed value
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.CALL("printd");
    _pf.TRASH(8); // delete the printed value's address
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.CALL("prints");
    _pf.TRASH(4); // delete the printed value's address
  }
  else {
    handleError(node, "Print with an impossible argument.");
    exit(2);
  }
  if(node->newLine()) 
    _pf.CALL("println"); // print a newline
}

//---------------------------------------------------------------------------
void zu::postfix_writer::do_read_node(zu::read_node * const node, int lvl) { DEBUG_PRINT;
  CHECK_TYPES(_compiler, _symtab, node);
  _pf.CALL("readi");
  _pf.PUSH();
}

//---------------------------------------------------------------------------
void zu::postfix_writer::do_for_node(zu::for_node * const node, int lvl) { DEBUG_PRINT;
  CHECK_TYPES(_compiler, _symtab, node);
 
  int condLbl = ++_lbl;
  int incrementLbl = ++_lbl;
  int endLbl = ++_lbl;

  // For break and continue to know where to jump to
  _continue_lbls.push_back(incrementLbl);
  _break_lbls.push_back(endLbl);

  node->initialization()->accept(this, lvl+2);

  _pf.ALIGN();
  _pf.LABEL(mklbl(condLbl)); // LABEL condition (condLbl)
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(endLbl)); // JZ endLbl (testa condição)

  node->block()->accept(this, lvl+2);

  _pf.ALIGN();
  _pf.LABEL(mklbl(incrementLbl)); //LABEL increment
  node->increment()->accept(this, lvl+2);
  _pf.JMP(mklbl(condLbl)); // Regressa para testar a condição
  
  _pf.ALIGN();
  _pf.LABEL(mklbl(endLbl)); //LABEL endfor

  // Cleanup
  _continue_lbls.pop_back();
  _break_lbls.pop_back();
}

void zu::postfix_writer::do_continue_node(zu::continue_node * const node, int lvl) { DEBUG_PRINT; 
  CHECK_TYPES(_compiler, _symtab, node);
  
  if(_continue_lbls.empty()) {
    handleError(node, "Not inside a loop.");
  }
  _pf.JMP(mklbl(_continue_lbls.back()));
}

void zu::postfix_writer::do_break_node(zu::break_node * const node, int lvl) { DEBUG_PRINT; 
  CHECK_TYPES(_compiler, _symtab, node);

  if(_break_lbls.empty()) {
    handleError(node, "Not inside a loop.");
  }
  _pf.JMP(mklbl(_break_lbls.back()));
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_if_node(zu::if_node * const node, int lvl) { DEBUG_PRINT;
  CHECK_TYPES(_compiler, _symtab, node);

  int endifLbl = ++_lbl;

  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(endifLbl));
  node->block()->accept(this, lvl+2);

  _pf.ALIGN();
  _pf.LABEL(mklbl(endifLbl));
}

void zu::postfix_writer::do_if_else_node(zu::if_else_node * const node, int lvl) { DEBUG_PRINT;
  CHECK_TYPES(_compiler, _symtab, node);

  int endifLbl = ++_lbl;
  int elseLbl = ++_lbl;

  //if
  node->condition()->accept(this, lvl+2);
  _pf.JZ(mklbl(elseLbl));
  node->thenblock()->accept(this, lvl+2);
  _pf.JMP(mklbl(endifLbl));
  //else
  _pf.ALIGN();
  _pf.LABEL(mklbl(elseLbl));
  node->elseblock()->accept(this, lvl+2);

  _pf.ALIGN();
  _pf.LABEL(mklbl(endifLbl));
}

void zu::postfix_writer::do_index_node(zu::index_node * const node, int lvl) { DEBUG_PRINT; 
  CHECK_TYPES(_compiler, _symtab, node);

  node->pointer()->accept(this, lvl+2);
  if(isLVAL(node->pointer())) {
    _pf.LOAD();
  }

  node->position()->accept(this, lvl+2);
  if(isLVAL(node->position())) {
    _pf.LOAD();
  }

  _pf.INT(node->type()->size());
  _pf.MUL();
  _pf.ADD();
}

void zu::postfix_writer::do_dereference_node(zu::dereference_node * const node, int lvl) { DEBUG_PRINT; 
  CHECK_TYPES(_compiler, _symtab, node);

  node->argument()->accept(this, lvl+2);
}

void zu::postfix_writer::do_reserve_memory_node(zu::reserve_memory_node * const node, int lvl) { DEBUG_PRINT; 
  CHECK_TYPES(_compiler, _symtab, node);

  // @FIXME: tipo de TYPE_UNSPEC
  _pf.INT(node->type()->subtype()->size());
  node->size()->accept(this, lvl+2);

  if(isLVAL(node->size())) {
    MY_LOAD(node->size());
  }

  _pf.MUL();
  _pf.ALLOC();
  _pf.SP();
}
