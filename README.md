# Service Finder

This is a C++17 program that uses Local File Inclusion (LFI) to identify a service running on a specified port. The program was inspired by and based on a Python script created by Tyler Ramsbey.

## Features

- Multi-threaded execution to speed up the process
- Fetches command line arguments for given PIDs
- Identifies the service running on the specified port
- Utilizes `libcurl` for making HTTP requests

## Requirements

- C++17 compatible compiler
- `libcurl` library
- `pthread` library

## Building the Project

To compile the project, you can use the following command:

```sh
g++ -std=c++17 -o service_finder main.cpp ServiceFinder.cpp -lcurl -lpthread
```

## Usage

Run the compiled binary with the following command-line arguments:

```sh
./service_finder --url <target url> --port <port> --threads <threads>
```
- `--url <target URL>`: The URL for the target.
- `--port <port>`: The port number to identify the service running on.
- `--threads <threads>`: The number of threads to use.

### Example

```sh
./service_finder --port 8080 --threads 10
```

## Project Structure

- `main.cpp`: The entry point of the program. Handles argument parsing and initiates the service finder process.
- `ServiceFinder.hpp`: Header file containing the `ServiceFinder` class declaration.
- `ServiceFinder.cpp`: Implementation file for the `ServiceFinder` class.

## Acknowledgements

A big thank you to Tyler Ramsbey for his original Python script, which served as the foundation for this project. You can find his work in the [offensivesecurity repository](https://github.com/TeneBrae93/offensivesecurity/).

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.