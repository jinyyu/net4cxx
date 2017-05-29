#ifndef LIBNET_DISTRIBUTION_EVENTLOOP_H
#define LIBNET_DISTRIBUTION_EVENTLOOP_H
#include <pthread.h>
#include <vector>
#include <unordered_map>

#include "callbacks.h"
#include "NonCopyable.h"

namespace net
{

class Channel;
class Selector;
class SelectionKey;
class ByteBuffer;

class EventLoop : NonCopyable
{
public:
  explicit EventLoop();

  ~EventLoop();

  //Run the EventLoop object's event processing loop
  void run();

  //Request the EventLoop to invoke the given callback and return immediately
  void post(const Callback& callback);

  void on_new_connection(ConnectionPtr conn, const Timestamp& timestamp);

  void allocate_receive_buffer(uint32_t capacity);

  void stop();

  bool is_in_loop_thread() const { return pthread_id_ == pthread_self(); }

private:
  friend class Connection;
  Selector* selector() const { return selector_; }

  void remove_connection(int fd) { connections_.erase(fd); }

  void wake_up();

private:
  pthread_t pthread_id_;
  volatile bool is_quit_;

  friend class Acceptor;
  friend class TcpClient;
  Selector* selector_;
  std::vector<SelectionKey*> active_keys_;

  int wakeup_fd_;
  Channel* wakeup_channel_;

  pthread_mutex_t mutex_;
  std::vector<Callback> callbacks_; //lock by mutex_

  std::unordered_map<int, ConnectionPtr> connections_;

  ByteBuffer* recv_buffer_;

};


}

#endif //LIBNET_DISTRIBUTION_EVENTLOOP_H
