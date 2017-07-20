%option c++ yyclass="zu_scanner" outfile="zu_scanner.cpp"
%option stack noyywrap yylineno 8bit debug
%{ 
/* $Id: zu_scanner.l,v 1.9 2016/05/18 20:26:41 ist175219 Exp $ */
// make relevant includes before including the parser's tab file
#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include "zu_scanner.h"
#include "zu_parser.tab.h"

// don't change this
#define yyerror LexerError

%}

D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
DECIMAL 	"."{D}+ 	
DOUBLE		{D}+(({DECIMAL})|{DECIMAL}?{E})

%x X_COMMENT X_STRING X_STRING_IGNORE X_STRING_SLASH
%%
	{/*set_debug(true);*/ /*export YYDEBUG = 1; in console*/}


"//".*$                ; /* ignore comments */
"/*"            		yy_push_state(X_COMMENT);
<X_COMMENT>"/*" 		yy_push_state(X_COMMENT);
<X_COMMENT>"*/" 		yy_pop_state();
<X_COMMENT>.|\n 		;

">="                   return tGE; 
"<="                   return tLE; 
"=="                   return tEQ; 
"!="                   return tNE; 
 
"!!"                   return tPRINT_NL; 
"><"				   return tBREAK;
"<>"				   return tCONTINUE;
"!!!"				   return tRETURN;

{L}({L}|{D})*		   yylval.s = new std::string(yytext); return tIDENTIFIER;

\"                     	/* "*/   yy_push_state(X_STRING); yylval.s = new std::string(""); 
<X_STRING>\"           	/* "*/	 yy_pop_state(); return tSTRING; 
<X_STRING>\\         			 yy_push_state(X_STRING_SLASH);
<X_STRING>\\0      				 yy_pop_state(); yy_push_state(X_STRING_IGNORE);
<X_STRING>.						 *yylval.s += yytext;
<X_STRING>\n    				 yyerror("newline in string");

<X_STRING_SLASH>[\\\"]	/*"*/	 yy_pop_state(); *yylval.s += yytext;
<X_STRING_SLASH>n 				 yy_pop_state(); *yylval.s += std::string(1, 10);
<X_STRING_SLASH>r 				 yy_pop_state(); *yylval.s += std::string(1, 13);
<X_STRING_SLASH>t 				 yy_pop_state(); *yylval.s += std::string(1, 9);
<X_STRING_SLASH>{H}{H}?			 yy_pop_state(); *yylval.s += std::string(1, strtol(yytext, nullptr, 16));
<X_STRING_SLASH>.|\n 			 yyerror("Invalid slashed character");

<X_STRING_IGNORE>\"      /*"*/ yy_pop_state(); return tSTRING; 
<X_STRING_IGNORE>.             ;
<X_STRING_IGNORE>\n            yyerror("newline in string");

{DOUBLE} 			 		{yylval.d = atof(yytext); return tDOUBLE;}

[0][xX]{H}+				{ yylval.i = strtol(yytext+2, nullptr, 16); if(errno == ERANGE) yyerror("Invalid number range, overflow occurred"); return tINTEGER; }
[0]|[1-9]{D}*			{ yylval.i = strtol(yytext, nullptr, 10); if(errno == ERANGE) yyerror("Invalid number range, overflow occurred"); return tINTEGER; }

[[\]&|@#%$!:?()<>=+\-*/;{}.,]       return *yytext;

[ \r\t\n]+               ; /* ignore whitespace */

.                      yyerror("Unknown character");

%%
// Very, very dirty hack: flex is a mess generating C++ scanners.
int zu_scanner::yywrap() { return 1; }