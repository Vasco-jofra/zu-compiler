// $Id: factory.h,v 1.2 2016/05/18 20:26:41 ist175219 Exp $ -*- c++ -*-
#ifndef __ZU_FACTORY_H__
#define __ZU_FACTORY_H__

#include <memory>
#include <cdk/yy_factory.h>
#include "zu_scanner.h"

namespace zu {

  /**
   * This class implements the compiler factory for the Zu compiler.
   */
  class factory: public cdk::yy_factory<zu_scanner> {
    /**
     * This object is automatically registered by the constructor in the
     * superclass' language registry.
     */
    static factory _self;

  protected:
    /**
     * @param language name of the language handled by this factory (see .cpp file)
     */
    factory(const std::string &language = "zu") :
        cdk::yy_factory<zu_scanner>(language) {
    }
  };

} // zu

#endif
