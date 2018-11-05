#include "gmuniformbufferobject.h"

namespace GMlib {

namespace GL {

  UniformBufferObject::UniformBufferObject() {}

  void UniformBufferObject::create() {
    BufferObject::create(GL_UNIFORM_BUFFER, GL_UNIFORM_BUFFER_BINDING );
  }

  void UniformBufferObject::create(const std::string &name) {
    BufferObject::create( name, GL_UNIFORM_BUFFER, GL_UNIFORM_BUFFER_BINDING );
  }

} // END namespace GL

} // END namespace GMlib
