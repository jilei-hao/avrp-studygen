
#ifndef __format_exception_hxx_
#define __format_exception_hxx_

#include <cstdio>
#include <cstdarg>
#include <exception>

/**
 * A simple exception class with string formatting
 */
class FormatException : public std::exception
{
public:

  FormatException(const char *format, ...)
    {
    buffer = new char[4096];
    va_list args;
    va_start (args, format);
    vsnprintf (buffer, 4096, format, args);
    va_end (args);
    }

  virtual const char* what() const throw() { return buffer; }

  virtual ~FormatException() throw() { delete buffer; }

private:

  char *buffer;

};


#endif //__format_exception_hxx_
