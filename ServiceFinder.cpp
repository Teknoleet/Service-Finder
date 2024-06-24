#include "ServiceFinder.hpp"
#include <iostream>
#include <thread>
#include <curl/curl.h>
#include <sstream>
#include <iomanip>
#include <algorithm>
using namespace std;
ServiceFinder::ServiceFinder(const std::string& target_path, int port, int thread_count)
    : target_path(target_path), port(port), thread_count(thread_count), found(false) {
    std::stringstream ss;
    ss << std::hex << port;
    std::string port_hex = ss.str();
    base_url = target_path + "/{}/cmdline";
    get_service_info(target_path, port_hex);
}

void ServiceFinder::findService() {
    if (!pid_queue.empty()) {
        std::vector<std::thread> threads;
        for (int i = 0; i < thread_count; ++i) {
            threads.emplace_back(&ServiceFinder::fetch_cmdline, this);
        }

        for (auto& t : threads) {
            t.join();
        }

        std::cout << "Finished fetching all PIDs." << std::endl;
    } else {
        std::cerr << "Failed to retrieve inode from the target." << std::endl;
    }
}

void ServiceFinder::fetch_cmdline() {
    while (true) {
        int pid;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            queue_cv.wait(lock, [&] { return !pid_queue.empty() || found.load(); });
            if (found.load() || pid_queue.empty()) return;
            pid = pid_queue.front();
            pid_queue.pop();
        }

        std::string url = base_url + std::to_string(pid) + "/cmdline";
        std::string content = send_request(url);
        std::replace(content.begin(), content.end(), '\x00', ' ');

        if (content.find("Page not found") == std::string::npos && !content.empty()) {
            if (content.find(std::to_string(port)) != std::string::npos) {
                std::lock_guard<std::mutex> lock(queue_mutex);
                std::cout << "\nThe service running on port " << port << " is: " << content << std::endl;
                found.store(true);
                queue_cv.notify_all();
                return;
            }
        }
    }
}

std::string ServiceFinder::get_service_info(const string& target_path, const string& port_hex) {
    std::string url = target_path + "/net/tcp";
    std::string response = send_request(url);
    std::istringstream iss(response);
    std::string line;

    while (std::getline(iss, line)) {
        std::istringstream line_ss(line);
        std::vector<std::string> fields;
        std::string field;
        while (line_ss >> field) {
            fields.push_back(field);
        }

        if (fields.size() > 1 && fields[1].substr(fields[1].size() - port_hex.size()) == port_hex) {
            for (int i = 1; i <= 1000; ++i) {
                pid_queue.push(i);
            }
            return fields[9];
        }
    }
    std::cerr << "No service info found for port " << std::stoi(port_hex, nullptr, 16) << " in " << url << std::endl;
    return "";
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string ServiceFinder::send_request(const std::string& url) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return readBuffer;
}
