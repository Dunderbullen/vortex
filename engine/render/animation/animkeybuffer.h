#pragma once
#include <stddef.h>
#include "foundation/math/quaternion.h"

namespace Render
{

class AnimKeyBuffer
{
public:
  AnimKeyBuffer();
  ~AnimKeyBuffer();
  
  void Setup(size_t numKeys);
  
  void Discard();
  
  size_t GetNumKeys() const;
  
  size_t GetByteSize() const;
  
  Math::vec4* GetKeyBufferPointer() const;
  
private:
  size_t numKeys;
  void* keyBuffer;
  
};

inline AnimKeyBuffer::AnimKeyBuffer() :
  numKeys(0),  
  keyBuffer(0)
{

}

inline AnimKeyBuffer::~AnimKeyBuffer()
{
  this->Discard();
}

inline void AnimKeyBuffer::Setup(size_t numKeys)
{
  this->numKeys = numKeys;
  this->keyBuffer = new Math::vec4[numKeys];
    
}

inline size_t AnimKeyBuffer::GetByteSize() const
{
	return this->numKeys * sizeof(Math::vec4);
}

inline size_t AnimKeyBuffer::GetNumKeys() const
{
  return this->numKeys;
}

inline void AnimKeyBuffer::Discard()
{
  //delete[] this->keyBuffer;
  this->keyBuffer = 0;
  this->numKeys = 0;
}


inline Math::vec4* AnimKeyBuffer::GetKeyBufferPointer() const
{
  return (Math::vec4*) this->keyBuffer;
}

}