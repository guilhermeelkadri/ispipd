# ISP IP Retrieval Publisher

This is a C++ project designed for retrieving the public IP address of a network and publishing it to a specified MQTT topic. It is composed of multiple threads, each with a specific role. The project utilizes the `mosquittopp` library for MQTT communication.

## Table of Contents

- [ISP IP Retrieval Publisher](#isp-ip-retrieval-publisher)
  - [Table of Contents](#table-of-contents)
  - [Overview](#overview)
  - [Project Structure](#project-structure)
  - [Building and Running](#building-and-running)
  - [Usage](#usage)
  - [Contributing](#contributing)
  - [License](#license)

## Overview

The project consists of the following main components:

1. **`ip_retrieval.h` and `ip_retrieval.cpp`**: This component handles the retrieval of the public IP address using a custom HTTP protocol. It sends a GET request to "api64.ipify.org" every 30 seconds and pushes the response into a thread-safe queue.

2. **`mqtt_client.h` and `mqtt_client.cpp`**: The MQTT client component manages the connection to a MQTT broker, subscribes to a specified topic, and publishes IP address updates to the topic "isp/ip/volvo."

3. **`thread_safe_queue.h`**: A thread-safe queue implementation used to communicate between the IP retrieval and MQTT client threads.

4. **`main.cpp`**: The main entry point of the application. It initializes the threads and manages command-line options.

## Project Structure

- `inc/` directory: Contains header files for the project components.
- `src/` directory: Contains the source code files for the project components.
- `CMakeLists.txt`: Configuration file for building the project using CMake.
- `README.md`: This readme file.

## Building and Running

To build the project, you can use CMake. Make sure you have CMake installed on your system. From the project root directory, follow these steps:

1. Create a build directory (e.g., `build`) and navigate to it:

   ```
   mkdir build
   cd build
   ```

2. Generate the build files with CMake:

   ```
   cmake ..
   ```

3. Build the project:

   ```
   make
   ```

This will generate an executable named `ispipd` in the build directory.

To run the program, execute the following command:

```
./ispipd
```

## Usage

The `ispipd` program supports the following command-line options:

- `-h`, `--help`: Display help and usage information.
- `-v`, `--version`: Display the program version information.

The program will continuously retrieve the public IP address using the `ip_retrieval` component and publish it to the specified MQTT topic using the `mqtt_client` component. It runs in the background as a service.

## Contributing

If you would like to contribute to this project, please follow these steps:

1. Fork the repository.
2. Create a new branch for your feature or bug fix.
3. Make your changes and commit them.
4. Create a pull request with a clear description of your changes.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.