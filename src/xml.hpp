#ifndef _DUMB_FW_XML_
#define _DUMB_FW_XML_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifdef _GNUC_
#include <unistd.h>
#endif _GNUC_
#include <stdlib.h>

#include <expat.h>

#define PARSER_HANDLER_BUFFER_SIZE 65536

namespace XML {


/**
 * Parser are parametrized with class implementing the following concept.
 *
 * void startElement(const XML_Char *, const XML_Char **);
 * void endElement(const XML_Char *);
 *
 */
template <typename T> class Parser {
public:

  static void parse(T *target, const char *filename) {
    char buffer[PARSER_HANDLER_BUFFER_SIZE];
    int handle = open(filename, O_RDONLY);
    ssize_t rdLen;

    if(handle < 0) {
      // TODO ERROR
      return;
    }

    XML_Parser parser = XML_ParserCreate(NULL);
    XML_SetUserData(parser, target);
    XML_SetElementHandler(parser, startElementHandler, endElementHandler);

    while((rdLen = read(handle, buffer, PARSER_HANDLER_BUFFER_SIZE)) > 0) {
      if(!XML_Parse(parser,
                    buffer,
                    rdLen,
                    rdLen < PARSER_HANDLER_BUFFER_SIZE)) {
        // TODO ERROR
        break;
      }
    }
  
    XML_ParserFree(parser);
    close(handle);
  }

private:
  static void startElementHandler(void *data,
                                  const XML_Char *name,
                                  const XML_Char **atts) {
    T* t = (T *) data;
    t->startElement(name, atts);
  }

  static void endElementHandler(void *data,
                                const XML_Char *name) {
    T *t = (T *) data;
    t->endElement(name);
  }
};

}


#endif
