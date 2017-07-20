%{
// $Id: zu_parser.y,v 1.15 2016/05/20 09:55:56 ist175219 Exp $
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <cdk/compiler.h>
#include "ast/all.h"
#define LINE       compiler->scanner()->lineno()
#define yylex()    compiler->scanner()->scan()
#define yyerror(s) compiler->scanner()->error(s)
#define YYPARSE_PARAM_TYPE std::shared_ptr<cdk::compiler>
#define YYPARSE_PARAM      compiler
//-- don't change *any* of these --- END!

%}

%union {
  int                   i;	/* integer value */
  double                d;  /* double value  */
  std::string          *s;	/* symbol name or string literal */
  cdk::basic_node      *node;	/* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  basic_type           *type;
  zu::lvalue_node  *lvalue;
};
%token <i> tINTEGER
%token <d> tDOUBLE
%token <s> tIDENTIFIER tSTRING

/* List of tokens */
%token tPRINT_NL tBREAK tCONTINUE tRETURN

%nonassoc tQUESTIONX
%nonassoc ':'

%right '='
%left '|'
%left '&'
%nonassoc '~'
%left tGE tLE tEQ tNE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc tUNARY


%type <node> file decl simpleVarDecl varDecl funcDecl block body
%type <node> stmt condStmt iterStmt iterInitArg
%type <sequence> decls funcArgs stmts blockVarDecls exprs iterArgs iterInitArgs

%type <expression> expr literal funcall
%type <lvalue> lval
%type <type> type

%type <s> string

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

// ficheiro
file : decls  { compiler->ast($1); }
     |        { compiler->ast(new cdk::nil_node(LINE)); }
     ;


// declaração
decl : varDecl ';'     { $$ = $1; }
     | funcDecl        { $$ = $1; }
     ;

// declarações
decls : decl       { $$ = new cdk::sequence_node(LINE, $1); }
      | decls decl { $$ = new cdk::sequence_node(LINE, $2, $1); }
      ;



// declaração simples de variável (sem ! ou ?)
simpleVarDecl : type tIDENTIFIER '=' expr  {$$ = new zu::variable_declaration_node(LINE, $1, new zu::id_node(LINE, $2), $4, false, false);}
              | type tIDENTIFIER           {$$ = new zu::variable_declaration_node(LINE, $1, new zu::id_node(LINE, $2), NULL, false, false);}
              ;

// declaração de variável
varDecl : type tIDENTIFIER '!' '=' expr  {$$ = new zu::variable_declaration_node(LINE, $1, new zu::id_node(LINE, $2), $5, true, false);}
        | type tIDENTIFIER '!'           {$$ = new zu::variable_declaration_node(LINE, $1, new zu::id_node(LINE, $2), NULL, true, false);}
        | type tIDENTIFIER '?'           {$$ = new zu::variable_declaration_node(LINE, $1, new zu::id_node(LINE, $2), NULL, false, true);}
        | simpleVarDecl                  {$$ = $1;}
        ;

// argumentos de uma funcao
funcArgs : simpleVarDecl                {$$ = new cdk::sequence_node(LINE, $1);}
         | funcArgs ',' simpleVarDecl   {$$ = new cdk::sequence_node(LINE, $3, $1);}
         | /* empty */                  {$$ = new cdk::sequence_node(LINE, new cdk::nil_node(LINE));}
         ;

// variáveis num bloco
blockVarDecls : simpleVarDecl ';'                 {$$ = new cdk::sequence_node(LINE, $1);}
              | blockVarDecls simpleVarDecl ';'   {$$ = new cdk::sequence_node(LINE, $2, $1);}
              ;

// bloco
block : '{' blockVarDecls stmts '}'   { $$ = new zu::block_node(LINE, $2, $3); }
      | '{' blockVarDecls '}'         { $$ = new zu::block_node(LINE, $2, NULL); }
      | '{' stmts '}'                 { $$ = new zu::block_node(LINE, NULL, $2); }
      | '{' '}'                       { $$ = new zu::block_node(LINE, NULL, NULL); }
      ; 

// corpo
body : block  {$$ = $1;} 
     ;

// função
funcDecl : type tIDENTIFIER '!' '(' funcArgs ')' '=' literal body 
                {$$ = new zu::function_definition_node(LINE, new zu::id_node(LINE, $2), $1, $8, $5, $9, true, false);}
         | type tIDENTIFIER '!' '(' funcArgs ')' body
                {$$ = new zu::function_definition_node(LINE, new zu::id_node(LINE, $2), $1, NULL, $5, $7, true, false);}
         
         | type tIDENTIFIER '(' funcArgs ')' '=' literal body 
                {$$ = new zu::function_definition_node(LINE, new zu::id_node(LINE, $2), $1, $7, $4, $8, false, false);}
         | type tIDENTIFIER '(' funcArgs ')' body
                {$$ = new zu::function_definition_node(LINE, new zu::id_node(LINE, $2), $1, NULL, $4, $6, false, false);}
         
         | '!' tIDENTIFIER '!' '(' funcArgs ')' body
                {$$ = new zu::function_definition_node(LINE, new zu::id_node(LINE, $2), new basic_type(0, basic_type::TYPE_VOID), NULL, $5, $7, true, false);}
         
         | '!' tIDENTIFIER '(' funcArgs ')' body
                {$$ = new zu::function_definition_node(LINE, new zu::id_node(LINE, $2), new basic_type(0, basic_type::TYPE_VOID), NULL, $4, $6, false, false);}

         | type tIDENTIFIER '!' '(' funcArgs ')'
                {$$ = new zu::function_declaration_node(LINE, new zu::id_node(LINE, $2), $1, $5, true, false);}
         | type tIDENTIFIER '?' '(' funcArgs ')'
                {$$ = new zu::function_declaration_node(LINE, new zu::id_node(LINE, $2), $1, $5, false, true);}
         | type tIDENTIFIER '(' funcArgs ')'
                {$$ = new zu::function_declaration_node(LINE, new zu::id_node(LINE, $2), $1, $4, false, true);}

         | '!' tIDENTIFIER '!' '(' funcArgs ')'
                {$$ = new zu::function_declaration_node(LINE, new zu::id_node(LINE, $2), new basic_type(0, basic_type::TYPE_VOID), $5, true, false);}
         | '!' tIDENTIFIER '?' '(' funcArgs ')'
                {$$ = new zu::function_declaration_node(LINE, new zu::id_node(LINE, $2), new basic_type(0, basic_type::TYPE_VOID), $5, false, true);}
         | '!' tIDENTIFIER '(' funcArgs ')'
                {$$ = new zu::function_declaration_node(LINE, new zu::id_node(LINE, $2), new basic_type(0, basic_type::TYPE_VOID), $4, false, false);}
         ;




// tipo
type : '#'              {$$ = new basic_type(4, basic_type::TYPE_INT);}
     | '%'              {$$ = new basic_type(8, basic_type::TYPE_DOUBLE);}
     | '$'              {$$ = new basic_type(4, basic_type::TYPE_STRING);}
     | '<' type '>'     {$$ = new basic_type(4, basic_type::TYPE_POINTER); $$->_subtype = $2;}
     ;


//intruções condicionais
condStmt : '[' expr ']' '#' stmt                                 { $$ = new zu::if_node(LINE, $2, $5); }
         | '[' expr ']' '?' stmt  %prec tQUESTIONX               { $$ = new zu::if_node(LINE, $2, $5); }
         | '[' expr ']' '?' stmt ':' stmt                        { $$ = new zu::if_else_node(LINE, $2, $5, $7); }

// args for 'for'
iterArgs: exprs         {$$ = $1;}
        | /*no args*/   {$$ = new cdk::sequence_node(LINE, new cdk::nil_node(LINE));}
        ;

// argumento na parte de inicialização do 'for'
iterInitArg : expr          {$$ = $1;}
            | simpleVarDecl {$$ = $1;}

// args para a inicialização do 'for' 
iterInitArgs: /*empty*/                     {$$ = new cdk::sequence_node(LINE, new cdk::nil_node(LINE));}
            | iterInitArg                   {$$ = new cdk::sequence_node(LINE, $1);}
            | iterInitArgs ',' iterInitArg  {$$ = new cdk::sequence_node(LINE, $3, $1);}
            ;

//intruções de iteração
iterStmt: '['  iterInitArgs ';' iterArgs ';' iterArgs ']' stmt   { $$ = new zu::for_node(LINE, $2, $4, $6, $8); }
        ;

// instrução
stmt : condStmt                                      { $$ = $1; }
     | iterStmt                                      { $$ = $1; }
     | expr ';'                                      { $$ = new zu::evaluation_node(LINE, $1); }
     | expr '!'                                      { $$ = new zu::print_node(LINE, $1, false); }
     | expr tPRINT_NL                                { $$ = new zu::print_node(LINE, $1, true); }
     | tBREAK                                        { $$ = new zu::break_node(LINE); }
     | tCONTINUE                                     { $$ = new zu::continue_node(LINE); }
     | tRETURN                                       { $$ = new zu::function_return_node(LINE); }
     | block                                         { $$ = $1; }
     ;

// Sequencias de instrucoes
stmts : stmt       { $$ = new cdk::sequence_node(LINE, $1); }
      | stmts stmt { $$ = new cdk::sequence_node(LINE, $2, $1); }
      ;


string : tSTRING          {$$ = $1;}
       | string tSTRING   {$$ = new std::string(*$1 + *$2); }

// literal
literal : tINTEGER                { $$ = new cdk::integer_node(LINE, $1); }
        | tDOUBLE                 { $$ = new cdk::double_node(LINE, $1); }
        | string                  { $$ = new cdk::string_node(LINE, $1); } 
        ;

// expressões
expr : literal
     | funcall
     | '-' expr %prec tUNARY   { $$ = new cdk::neg_node(LINE, $2); }
     | '+' expr %prec tUNARY   { $$ = new zu::identity_node(LINE, $2); }
     | expr '+' expr	         { $$ = new cdk::add_node(LINE, $1, $3); }
     | expr '-' expr	         { $$ = new cdk::sub_node(LINE, $1, $3); }
     | expr '*' expr	         { $$ = new cdk::mul_node(LINE, $1, $3); }
     | expr '/' expr	         { $$ = new cdk::div_node(LINE, $1, $3); }
     | expr '%' expr	         { $$ = new cdk::mod_node(LINE, $1, $3); }
     | expr '<' expr	         { $$ = new cdk::lt_node(LINE, $1, $3); }
     | expr '>' expr	         { $$ = new cdk::gt_node(LINE, $1, $3); }
     | expr tGE expr	         { $$ = new cdk::ge_node(LINE, $1, $3); }
     | expr tLE expr           { $$ = new cdk::le_node(LINE, $1, $3); }
     | expr tNE expr	         { $$ = new cdk::ne_node(LINE, $1, $3); }
     | expr tEQ expr           { $$ = new cdk::eq_node(LINE, $1, $3); }
     | expr '|' expr           { $$ = new zu::or_node(LINE, $1, $3); }
     | expr '&' expr           { $$ = new zu::and_node(LINE, $1, $3); }
     | '~' expr %prec tUNARY   { $$ = new zu::not_node(LINE, $2); }
     | '@'                     { $$ = new zu::read_node(LINE); }
     | lval '?'                { $$ = new zu::dereference_node(LINE, $1); }
     | '[' expr ']'            { $$ = new zu::reserve_memory_node(LINE, $2); } 
     | '(' expr ')'            { $$ = $2; }
     | lval                    { $$ = $1; }
     | lval '=' expr           { $$ = new zu::assignment_node(LINE, $1, $3); }
     ;

exprs : expr           { $$ = new cdk::sequence_node(LINE, $1); }
      | expr ',' exprs { $$ = new cdk::sequence_node(LINE, $1, $3); }
      ; 

funcall : tIDENTIFIER '(' exprs ')'   { $$ = new zu::function_call_node(LINE, new zu::id_node(LINE, $1), $3); }
        | tIDENTIFIER '(' ')'         { $$ = new zu::function_call_node(LINE, new zu::id_node(LINE, $1), NULL); }
        ;

lval : tIDENTIFIER               { $$ = new zu::id_node(LINE, $1); }
     | lval '[' expr ']'         { $$ = new zu::index_node(LINE, $1, $3); }
     | funcall '[' expr ']'      { $$ = new zu::index_node(LINE, $1, $3); }
     | '(' expr ')' '[' expr ']' { $$ = new zu::index_node(LINE, $2, $5); }
     ;

%%
