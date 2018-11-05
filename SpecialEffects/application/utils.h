#ifndef __QMLDEMO_UTILS_H__
#define __QMLDEMO_UTILS_H__

#ifndef __EXCEPTION_HEAD
#  define __EXCEPTION_HEAD std::string("") + std::string(__FILE__) + "[line " + std::to_string(__LINE__) + "] "
#endif // __EXCEPTION_HEAD

#ifndef __EXCEPTION_TAIL
#  define __EXCEPTION_TAIL std::string(" ON line ") + std::to_string(__LINE__) + " IN FILE "  + std::string(__FILE__)
#endif // __EXCEPTION_TAIL

#endif // __QMLDEMO_UTILS_H__
