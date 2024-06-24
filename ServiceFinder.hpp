#ifndef SERVICEFINDER_HPP
#define SERVICEFINDER_HPP

#include <string>
#include <queue>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <vector>

class ServiceFinder {
public:
    ServiceFinder(const std::string& target_path, int port, int thread_count);
    void findService();

private:
    std::string target_path;
    std::string base_url;
    int port;
    int thread_count;
    std::atomic<bool> found;
    std::mutex queue_mutex;
    std::condition_variable queue_cv;
    std::queue<int> pid_queue;

    void fetch_cmdline();
    std::string get_service_info(const std::string& target_path, const std::string& port_hex);
    std::string send_request(const std::string& url);
};

#endif // SERVICEFINDER_HPP
