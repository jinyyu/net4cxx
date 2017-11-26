#include "net4cxx/reactor/TcpServer.h"
#include "net4cxx/reactor/EventLoop.h"
#include "net4cxx/reactor/Acceptor.h"
#include "net4cxx/reactor/Connection.h"
#include "net4cxx/common/Timestamp.h"
#include <log4cxx/logger.h>

namespace net4cxx
{

static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("net4cxx"));

TcpServer::TcpServer(int port, int num_io_threads)
    : port_(port),
      num_io_threads_(num_io_threads)
{
}

TcpServer::~TcpServer()
{
    for (auto i = 0; i < threads_.size(); ++i) {
        threads_[i].join();
    }
    for (auto it = io_loops_.begin(); it < io_loops_.end(); ++it) {
        delete (*it);
    }
}

void TcpServer::run()
{
    io_loops_.resize(num_io_threads_, NULL);
    auto run_cb = [this](int index)
    {
        EventLoop* loop = new EventLoop();
        loop->allocate_receive_buffer(6 * 1024 * 1024); //6M
        NewConnectionCallback cb = [this, loop](int fd,
                                                const Timestamp& timestamp,
                                                const InetSocketAddress& local,
                                                const InetSocketAddress& peer)
        {
            ConnectionPtr conn(new Connection(fd, loop, local, peer));
            conn->connection_established_callback(connection_established_callback_);
            conn->read_message_callback(read_message_callback_);
            conn->connection_closed_callback(connection_closed_callback_);
            loop->on_new_connection(conn, timestamp);
        };
        Acceptor acceptor(loop, port_);
        acceptor.new_connection_callback(cb);
        io_loops_[index] = loop;
        loop->run();
        LOG4CXX_INFO(logger, "loop exit " << index);

        std::unique_lock<std::mutex> lock(mutex_);
        --count_down_;
        cv_.notify_one();
    };

    count_down_ = num_io_threads_;
    for (int i = 0; i < num_io_threads_; ++i) {
        threads_.emplace_back(std::bind(run_cb, i));
    }
    std::unique_lock<std::mutex> lock(mutex_);
    while (count_down_ > 0) {
        cv_.wait(lock);
    }
}

void TcpServer::shutdown()
{
    for (auto it = io_loops_.begin(); it < io_loops_.end(); ++it) {
        (*it)->stop();
    }
}

}