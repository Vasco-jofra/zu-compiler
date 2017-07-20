// $Id: symbol.h,v 1.4 2016/05/19 23:47:15 ist175219 Exp $ -*- c++ -*-
#ifndef __ZU_SEMANTICS_SYMBOL_H__
#define __ZU_SEMANTICS_SYMBOL_H__

#include <string>
#include <cdk/basic_type.h>


namespace zu {
    class symbol {
      basic_type *_type;
      std::string _name;

      // Function stuff
      bool _isFunction;
      bool _isDefined;

      // Variable stuff (set in variable declaration node (postfix_writer))
      bool _isGlobal = true;
      int _offset = 0; // Only used if it is a local var

    public:
      inline symbol(basic_type *type, const std::string &name, 
                    bool isFunction, bool isDefined) :
          _type(type), _name(name), _isFunction(isFunction), _isDefined(isDefined) {
      }

      virtual ~symbol() {
        delete _type;
      }

      inline basic_type *type() const {
        return _type;
      }
      inline const std::string &name() const {
        return _name;
      }

    public:
      inline bool isFunction() const {
        return _isFunction;
      }
      inline bool isVar() const {
        return !_isFunction;
      }

    public:
      inline bool isDefined() const {
        return _isDefined;
      }

      inline void setIsDefined(bool newD) {
        _isDefined = newD;
      }

    public:
      inline bool isGlobal() const {
        return _isGlobal;
      }
      inline void setIsGlobal(bool newG) {
        _isGlobal = newG;
      }
      inline bool isLocal() const {
        return !_isGlobal;
      }

    public:
      inline int offset() const {
        return _offset;
      }
      inline void setOffset(int newO) {
        _offset = newO;
      }
    };

} // zu

#endif
