#ifndef LIBNET_DISTRIBUTION_BYTEBUFFER_H
#define LIBNET_DISTRIBUTION_BYTEBUFFER_H
#include "NonCopyable.h"

namespace net
{

class ByteBuffer : NonCopyable
{
 public:
  explicit ByteBuffer(int capacity);

  ~ByteBuffer();

  //Returns this buffer's capacity
  int capacity() const { return capacity_; }

  //Returns this buffer's position
  int position() const { return position_; }

  //Sets this buffer's position
  void position(int p);

  //Returns this buffer's limit
  int limit()  const { return limit_; }

  //Sets this buffer's limit
  void limit(int limit );

  //Sets this buffer's mark at its position
  void mark() {mark_ = position_; }

  //Resets this buffer's position to the previously-marked position
  //Invoking this method neither changes nor discards the mark's value
  void reset();

  //Clears this buffer. The position is set to zero, the limit is set to the capacity, and the mark is discarded
  //Invoke this method before using a sequence of channel-read or put operations to fill this buffer
  void clear();

  //Flips this buffer. The limit is set to the current position and then the position is set to zero. If the mark is defined then it is discarded
  void flip();

  //Rewinds this buffer. The position is set to zero and the mark is discarded
  void rewind();

  //Returns the number of elements between the current position and the limit
  int remaining() { return limit_ - position_; }

  //Tells whether there are any elements between the current position and the limit
  bool has_remaining() { return position_ < limit_; }

  const void* data() const { return data_ + position_; }

  void* data() { return data_ + position_; }

  //bulk get method
  void get(void* buffer, int len);

  //bulk put method
  void put(void* data, int len);

 private:

  int limit_;
  int mark_;
  int position_;
  const int capacity_;
  char* data_;

};

}

#endif //LIBNET_DISTRIBUTION_BYTEBUFFER_H