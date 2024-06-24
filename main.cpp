#include <iostream>
#include <nlohmann/json.hpp>
#include "ServiceFinder.hpp"

using json = nlohmann::json;

int main(int argc, char* argv[]) {
    if (argc < 7) {
        std::cerr << "Usage: " << argv[0] << " --url <URL> --port <port> --threads <threads>" << std::endl;
        return 1;
    }

    json args;
    args["port"] = nullptr;
    args["threads"] = nullptr;
    args["target"] = nullptr;

    for (int i = 1; i < argc; i += 2) {
        std::string key = argv[i];
        if (key == "--port") {
            args["port"] = std::stoi(argv[i + 1]);
        } else if (key == "--threads") {
            args["threads"] = std::stoi(argv[i + 1]);
        }
        else if (key == "--url") {
            args["target"] = std::string(argv[i + 1]);
        }
    }

    if (args["port"].is_null() || args["threads"].is_null()|| args["target"].is_null()) {
        std::cerr << "Both port and threads are required." << std::endl;
        return 1;
    }

    int port = args["port"];
    int threads_count = args["threads"];
    std::string target_path = args["target"];

    ServiceFinder serviceFinder(target_path, port, threads_count);
    serviceFinder.findService();

    return 0;
}
