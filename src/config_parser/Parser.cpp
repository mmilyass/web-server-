#include "../../includes/Webserv.hpp"

std::map<std::string, std::string> LoadMimeTypes(std::string filename)
{
    std::map<std::string, std::string> mimeMap;
    std::ifstream file(filename.c_str());

    if (!file.is_open())
        throw std::runtime_error("Failed to open mime types file: " + filename);

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
            continue;

        std::istringstream iss(line);
        std::string mimeType;
        iss >> mimeType;

        std::string ext;
        while (iss >> ext)
        {
            if (ext[ext.size() - 1] == ';')
                ext.erase(ext.size() - 1);
            mimeMap[ext] = mimeType;
        }
    }
    return mimeMap;
}

Config Parser::parse(const std::vector<std::string> &tokens)
{
    Config config;
    unsigned long i = 0;
    while (i < tokens.size())
    {
        if (tokens[i] == "server")
            config.servers.push_back(parseServer(tokens, i));
        else
            throw std::runtime_error("Unexpected token: in `parse` " + tokens[i]);
        i++;
    }
    parse_config(config);
    config.MimeTypes = LoadMimeTypes("configs/mimetypes.txt");
    return config;
}

int parseListen(const std::vector<std::string> &tokens, unsigned long &i)
{
    i++;
    if (tokens[i].find_first_not_of("+0123456789 ") != std::string::npos)
        throw std::runtime_error("Unexpected token: should be 0123456789 but token is " + tokens[i]);
    char *end ;
    double result = std::strtod(tokens[i].c_str(), &end);
    int number = static_cast<int>(result);
    if (*end == '\0')
    {
        if (number > 65535 || number < 1)
            throw std::runtime_error("Unexpected token: should be number > 65535 || number < 1 but token is " + tokens[i]);
        i++;
        return number;
    }
    throw std::runtime_error("Unexpected token: " + tokens[i]);
}

size_t parseSizeDirective2(const std::vector<std::string> &tokens, unsigned long &i)
{
    i++;

    const std::string &tok = tokens[i];

    if (tok.size() < 2)
        throw std::runtime_error("Invalid size format: " + tok);

    if (tok[tok.size() - 1] != 'M')
        throw std::runtime_error("Size must end with 'M': " + tok);

    std::string number = tok.substr(0, tok.size() - 1);

    if (number.find_first_not_of("0123456789") != std::string::npos)
        throw std::runtime_error("Invalid number in size: " + tok);

    size_t size = std::atoi(number.c_str()) * 1024 * 1024;
    if (size > 10 * 1024 * 1024)
         throw std::runtime_error("Invalid number in size: " + tok + " use less than 10M");
    i++;
    return size;
}

std::string strip_quotes(const std::string &s) {
    if (s.length() >= 2 && s[0] == '"' && s[s.length() - 1] == '"') {
        return s.substr(1, s.length() - 2);
    }
    return s;
}

void parseErrorPage(ServerConfig &server, const std::vector<std::string> &tokens, unsigned long &i)
{
    i++;
    std::string path;
    unsigned long error;
    if (tokens[i].find_first_not_of("+0123456789 ") != std::string::npos)
        throw std::runtime_error("Unexpected token: should be 0123456789 but token is " + tokens[i]);
    char *end ;
    double result = std::strtod(tokens[i].c_str(), &end);
    error = static_cast<int>(result);
    if (*end == '\0')
    {
        if (error > 599 || error < 400)
            throw std::runtime_error("Unexpected token: should be error > 599 || error < 400 but token is " + tokens[i]);
        i++;
    }
    path = strip_quotes(tokens[i]);
    i++;
    server.error_pages[error] = path;
}

void expect(const std::vector<std::string> &tokens, unsigned long &i)
{
    if (tokens[i] == ";")
        i++;
    else
        throw std::runtime_error("Should be : |;| but token is : " + tokens[i]);
}

void init_map(std::map<std::string, bool> &directive_flags)
{
    directive_flags["root"] = false;
    directive_flags["client_max_body_size"] = false;
    directive_flags["server_name"] = false;
}

void check_map(std::map<std::string, bool> &directive_flags, std::string directive)
{
    if (directive_flags[directive] == false)
        directive_flags[directive] = true;
    else
        throw std::runtime_error("Unexpected token: must appear exactly once " + directive);
}

void last_check_in_map(std::map<std::string, bool> &directive_flags)
{
    std::map<std::string, bool>::const_iterator it = directive_flags.begin();
    while (it != directive_flags.end())
    {
        if (!it->second)
            throw std::runtime_error("This directives are set to false :" + it->first);
        it++;
    }
    
}

ServerConfig Parser::parseServer(const std::vector<std::string> &tokens, unsigned long &i)
{
    ServerConfig server;
    if (tokens[i] != "server")
        throw std::runtime_error("Unexpected token: should be `server` but token is " + tokens[i]);
    if (tokens[i + 1] != "{")
        throw std::runtime_error("Unexpected token: should be `{` but token is" + tokens[i + 1]);
    i += 2;
    std::map<std::string, bool> directive_flags;
    init_map(directive_flags);
    while (tokens[i] != "}")
    {
        if (tokens[i] == "listen")
        {
            server.listen_ports.push_back(parseListen(tokens, i));
            expect(tokens, i);
        }
        else if (tokens[i] == "root")
        {
            check_map(directive_flags, tokens[i]);
            server.root = strip_quotes(tokens[++i]);
            i++;
            expect(tokens, i);
        }
        else if (tokens[i] == "index")
        {
            server.index = strip_quotes(tokens[++i]);
            i++;
            expect(tokens, i);
        }
        else if (tokens[i] == "server_name")
        {
            check_map(directive_flags, tokens[i]);
            server.server_name = strip_quotes(tokens[++i]);
            i++;
            expect(tokens, i);
        }
        else if (tokens[i] == "client_max_body_size")
        {
            check_map(directive_flags, tokens[i]);
            server.client_max_body_size = parseSizeDirective2(tokens, i);
            expect(tokens, i);
        }
        else if (tokens[i] == "error_page")
        {
            parseErrorPage(server, tokens, i);
            expect(tokens, i);
        }
        else if (tokens[i] == "location")
        {
            server.locations.push_back(parseLocation(tokens, i));
            if (server.locations[server.locations.size() - 1].autoindex == true)
            {
                if (server.locations[server.locations.size() - 1].root == "")
                    server.locations[server.locations.size() - 1].list = buildDirectoryListing(server.root + server.locations[server.locations.size() - 1].path);
                else
                    server.locations[server.locations.size() - 1].list = buildDirectoryListing(server.locations[server.locations.size() - 1].root + server.locations[server.locations.size() - 1].path);
            }
        }
        else
            throw std::runtime_error("Unexpected token in parseServer: " + tokens[i]);
    }
    if (tokens[i] != "}")
        throw std::runtime_error("Unexpected token in parseServer: should be `}` but token is " + tokens[i]);
    last_check_in_map(directive_flags);
    if (server.index.empty())
        server.index = "index.html";
    if (server.listen_ports.empty())
        throw std::runtime_error("No listenport in config file");
    return server;
 }

 LocationConfig Parser::parseLocation(const std::vector<std::string> &tokens, unsigned long &i)
 {
    LocationConfig location;
    if (tokens[i] != "location")
        throw std::runtime_error("Unexpected token in parseLocation: should be `location` but token is  " + tokens[i]);
    if (tokens[i + 2] != "{")
        throw std::runtime_error("Unexpected token in parseLocation: should be { but token is  " + tokens[i + 2]);
    location.path = tokens[i + 1];
    i += 3;
    while (tokens[i] != "}")
    {
        if (tokens[i] == "autoindex")
        {
            i++;
            if (tokens[i] == "off")
                location.autoindex = false;
            else if (tokens[i] == "on")
                location.autoindex = true;
            else
                throw std::runtime_error("Unexpected token in parseLocation: should be `off or on ` but " + tokens[i]);
            i++;
            expect(tokens, i);
        }
        else if (tokens[i] == "upload_path")
        {
            i++;
            location.upload_path = strip_quotes(tokens[i]);
            i++;
            expect(tokens, i);
        }
        else if (tokens[i] == "cgi_path")
        {
            i++;
            location.cgi_path = strip_quotes(tokens[i]);
            i++;
            expect(tokens, i);
        }
        else if (tokens[i] == "root")
        {
            location.root = strip_quotes(tokens[++i]);
            i++;
            expect(tokens, i);
        }
        else if (tokens[i] == "index")
        {
            location.index = tokens[++i];
            i++;
            expect(tokens, i);
        }
        else if (tokens[i] == "methods")
        {
            i++;
            while (tokens.size() > i && tokens[i] != ";")
            {
                location.methods.push_back(tokens[i]);
                i++;
            }
            expect(tokens, i);
        }
        else if (tokens[i] == "return")
        {
            i++;
            if (tokens[i].find_first_not_of("+0123456789 ") != std::string::npos)
                throw std::runtime_error("Unexpected token: should be 0123456789 but token is " + tokens[i]);
            unsigned long return_code;
            char *end ;
            double result = std::strtod(tokens[i].c_str(), &end);
            return_code = static_cast<int>(result);
            if (*end == '\0')
            {
                if (return_code > 399 || return_code < 300)
                    throw std::runtime_error("Unexpected token: should be return_code > 399 || return_code < 300 but token is " + tokens[i]);
                i++;
            }
            location.return_url = strip_quotes(tokens[i]);
            location.return_code = return_code;
            i++;
            expect(tokens, i);
        }
        else
            throw std::runtime_error("Unexpected token in parseLocation: " + tokens[i]);
    }
    if (tokens[i] != "}")
        throw std::runtime_error("Unexpected token in parseLocation: should be `}` but token is " + tokens[i]);
    i++;
    return location;
}
