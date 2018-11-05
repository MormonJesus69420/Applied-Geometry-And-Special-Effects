#ifndef GM_OPENGL_BUFFEROBJECTS_UNIFORMBUFFEROBJECT_H
#define GM_OPENGL_BUFFEROBJECTS_UNIFORMBUFFEROBJECT_H


#include "../gmbufferobject.h"

namespace GMlib {

namespace GL {

  class UniformBufferObject : public BufferObject {
  public:
    explicit UniformBufferObject();

    void create();
    void create( const std::string& name );

  }; // END class UniformBufferObject

} // END namespace GL

} // END namespace GMlib

#endif // GM_OPENGL_BUFFEROBJECTS_UNIFORMBUFFEROBJECT_H
