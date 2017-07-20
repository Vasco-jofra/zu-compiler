// $Id: block_node.h,v 1.1 2016/04/15 02:01:40 ist175219 Exp $ -*- c++ -*-
#ifndef __CDK_BLOCKNODE_H__
#define __CDK_BLOCKNODE_H__

namespace zu {

  class block_node: public cdk::basic_node {
  public:
    cdk::sequence_node *_decls;
    cdk::sequence_node *_instr;

    inline block_node(int lineno, 
                      cdk::sequence_node *decls, 
                      cdk::sequence_node *instr) :
        basic_node(lineno), _decls(decls), _instr(instr){
    }

  public:
    inline cdk::sequence_node *decls() {
      return _decls;
    }
    inline cdk::sequence_node *instr() {
      return _instr;
    }
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_block_node(this, level);
    }

  };

} // zu

#endif //__CDK_BLOCKNODE_H__
