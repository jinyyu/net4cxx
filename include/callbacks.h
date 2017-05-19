#ifndef LIBNET_DISTRIBUTION_CALLBACKS_H
#define LIBNET_DISTRIBUTION_CALLBACKS_H

#include <functional>
#include <memory>

namespace net
{

class Timestamp;
class SelectionKey;
class InetSocketAddress;
class Connection;

typedef std::shared_ptr<Connection> ConnectionPtr;

typedef std::function<void(const Timestamp &, SelectionKey *)> SelectionCallback;

typedef std::function<void()> Callback;


typedef std::function<void(int, const Timestamp &, const InetSocketAddress &, const InetSocketAddress &)> NewConnectionCallback;

typedef std::function<bool (ConnectionPtr, const Timestamp &)> __NewConnectionCallback;


}


#endif //LIBNET_DISTRIBUTION_CALLBACKS_H
