# WebServ - HTTP Server Implementation

> A custom HTTP/1.1 server implementation built from scratch. Test it with a real web browser and deepen your understanding of web protocols.

## 📋 Overview

WebServ is an educational project that implements a fully functional HTTP server from scratch. This project provides hands-on experience with networking, protocol implementation, and server architecture. By building this server, you'll gain a deep understanding of how HTTP works and the intricacies of web communication—even if web development isn't your primary career path.

### Key Features

- **Full HTTP/1.1 Support** - Implements core HTTP/1.1 protocol specifications
- **Configuration-Driven** - Customize server behavior through configuration files
- **Error Handling** - Comprehensive error management and status codes
- **Static File Serving** - Serve static content from the filesystem
- **Real Browser Testing** - Fully compatible with standard web browsers
- **Multi-Connection Support** - Handle multiple simultaneous client connections

## 🚀 Getting Started

### Prerequisites

- C++ compiler (C++98 or later)
- Make
- Linux/Unix-like operating system (macOS, Linux)

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/ybounite/webserv.git
cd webserv

# Build the project
make

# (Optional) Clean build artifacts
make clean
```

### Running the Server

```bash
# Start the server with default configuration
./webserv

# Or specify a custom configuration file
./webserv configs/default.conf
```

The server will start listening on the configured port (typically `localhost:8080`). Open your browser and navigate to `http://localhost:8080` to test the server.

## 📁 Project Structure

```
webserv/
├── configs/          # Configuration files for server setup
├── errors/           # Error page templates and error handling
├── includes/         # Header files (.hpp)
├── src/              # Source implementation files (.cpp)
├── www/              # Static web content and assets
├── Makefile          # Build configuration
└── .gitignore        # Git ignore rules
```

### Directory Details

| Directory | Purpose |
|-----------|---------|
| `configs/` | Server configuration files defining ports, routes, and behaviors |
| `errors/` | Custom error pages and error response handling |
| `includes/` | C++ header files with class declarations and interfaces |
| `src/` | C++ implementation files containing the server logic |
| `www/` | Static files served by the server (HTML, CSS, images, etc.) |

## 🔧 Configuration

The server is configured through configuration files in the `configs/` directory. Common settings include:

- **Port** - The port on which the server listens
- **Host** - The host address to bind to
- **Server Name** - The name of the server
- **Routes** - URL routing and file serving paths
- **Error Pages** - Custom error page locations

Example configuration structure:
```
server {
    listen 8080;
    server_name localhost;
    
    location / {
        root www;
    }
    
    location /error {
        return 404;
    }
}
```

## 📝 HTTP Features Implemented

### Supported HTTP Methods
- GET - Retrieve resources
- POST - Submit data to the server
- DELETE - Remove resources
- HEAD - Retrieve headers without body
- PUT - Upload/update resources

### Status Codes
- **2xx Success** - 200 OK, 201 Created, etc.
- **3xx Redirection** - 301 Moved Permanently, 302 Found, etc.
- **4xx Client Errors** - 400 Bad Request, 404 Not Found, etc.
- **5xx Server Errors** - 500 Internal Server Error, etc.

### Content Support
- Static file serving (HTML, CSS, JavaScript, images)
- MIME type detection
- Content-Length headers
- Keep-Alive connections
- Chunked transfer encoding

## 🧪 Testing

### Browser Testing
```bash
# Open your browser and navigate to:
http://localhost:8080
```

### Command Line Testing (curl)
```bash
# GET request
curl http://localhost:8080/

# POST request
curl -X POST http://localhost:8080/upload

# DELETE request
curl -X DELETE http://localhost:8080/file.txt

# View response headers
curl -i http://localhost:8080/
```

### Testing Different Routes
```bash
curl http://localhost:8080/
curl http://localhost:8080/index.html
curl http://localhost:8080/assets/style.css
```

## 🏗️ Architecture

The server follows a modular architecture:

1. **Server Core** - Main server loop and connection handling
2. **Request Parser** - HTTP request parsing and validation
3. **Request Handler** - Process requests and generate responses
4. **Response Builder** - Construct HTTP responses
5. **Configuration Manager** - Load and manage server configuration
6. **Error Handler** - Generate and send error responses

## 📚 Learning Outcomes

By studying and working with this project, you'll learn:

- **Network Programming** - Socket programming and TCP/IP
- **HTTP Protocol** - Request/response cycle, headers, status codes
- **Server Architecture** - Handling multiple connections, event loops
- **C++ Programming** - Object-oriented design, file I/O
- **Configuration Management** - Parsing and applying configuration files
- **Error Handling** - Robust error management and recovery

## 🔐 Security Considerations

This is an educational project. For production use, consider:

- Input validation and sanitization
- Buffer overflow protection
- Path traversal prevention
- Rate limiting and DoS protection
- HTTPS/SSL support
- Access control and authentication

## 📖 Usage Examples

### Serving Static Files
```bash
# The server will serve files from the www/ directory
# Access them via: http://localhost:8080/filename
```

### Handling Errors
```bash
# Custom error pages are served from the errors/ directory
# 404 errors display: errors/404.html
# 500 errors display: errors/500.html
```

### Multiple Server Instances
```bash
# You can run multiple servers on different ports
./webserv configs/server1.conf  # Port 8080
./webserv configs/server2.conf  # Port 8081
```

## 🛠️ Development

### Adding New Features
1. Modify relevant header files in `includes/`
2. Implement changes in `src/`
3. Rebuild with `make`
4. Test with a browser or curl

### Code Style
- Follow consistent indentation (2-4 spaces)
- Use meaningful variable and function names
- Add comments for complex logic
- Keep functions focused and modular

## 🐛 Troubleshooting

### Port Already in Use
```bash
# Change the port in your configuration file
# Or kill the existing process:
lsof -i :8080
kill -9 <PID>
```

### Permission Denied
```bash
# Ensure the executable has proper permissions
chmod +x webserv
```

### Connection Refused
```bash
# Make sure the server is running
# Check the configuration file for correct host/port settings
```

## 📄 License

This project is created for educational purposes. Check the repository for any specific license information.

## 👤 Author

> - **ybounite** - [GitHub Profile](https://github.com/ybounite)
> - **soufiane-dahani** - [GitHub Profile](https://github.com/soufiane-dahani)
> - **mmilyass** - [GitHub Profile](https://github.com/mmilyass)

## 🤝 Contributing

Contributions are welcome! Feel free to:
- Report bugs and issues
- Suggest improvements
- Submit pull requests
- Share your implementations

## 📚 Additional Resources

- [HTTP/1.1 Specification (RFC 7231)](https://tools.ietf.org/html/rfc7231)
- [Socket Programming Guide](https://linux.die.net/man/7/socket)
- [C++ Network Programming](https://docs.microsoft.com/en-us/cpp/build/building-on-the-command-line)

## 🎯 Next Steps

1. **Explore the Code** - Study the source files to understand the implementation
2. **Modify Configuration** - Try different server configurations
3. **Add Features** - Implement additional HTTP methods or functionality
4. **Optimize Performance** - Analyze and improve server efficiency
5. **Deploy** - Consider running the server in different environments

---

**Happy Coding!** 🚀

If you have questions or need help, feel free to open an issue on the repository.
