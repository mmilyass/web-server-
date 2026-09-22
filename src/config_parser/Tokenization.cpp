#include "../../includes/Webserv.hpp"

bool skip_spaces_and_found_line(const std::string &content)
{
    unsigned long i = 0;
    while (i < content.length() && (content[i] == 32 || (content[i] <= 9 && content[i] >= 1)))
        i++;
    if (content[i] == '\n' || content[i] == '}')
        return true;
    else
        return false;
}

std::vector<std::string> Tokenizer::tokenize(const std::string &content)
{
    std::vector<std::string> vector;
    unsigned long i = 0;
    std::string str;
    bool flag;
    bool flag_str;

    while (i < content.length())
    {
        flag = false;
        flag_str = false;

        while (i < content.length() && 
              (content[i] == '\n' || content[i] == 32 || (content[i] <= 9 && content[i] >= 1)))
        {
            i++;
        }
        if (i >= content.length())
            break;

        if ((content[i] == '{' && content[i - 1] == ' ')|| content[i] == '}' || content[i] == ';')
        {
            vector.push_back(std::string(1, content[i]));
            i++;
            continue;
        }

        // Build token
        while (i < content.length())
        {
            while (content[i] == '#')
            {
                while (content[i] != '\n' && i < content.length())
                    i++;
                if (content[i] == '\n')
                    i++;
            }
            if (content[i] == '\n')
                break;
            if (content[i] == '\"')
            {
                i++;
                str += '\"';
                while (content[i] != '\"' && i < content.length())
                {
                    str += content[i];
                    i++;
                }
                if (content[i - 1] == '\"')
                    str += '\"';
            }

            if (((content[i] == '{' && content[i - 1] == ' ') || content[i] == '}' || content[i] == ';') && skip_spaces_and_found_line(content.substr(i + 1)))
            {
                flag = true;
                break;
            }

            if (content[i] == 32 || (content[i] <= 9 && content[i] >= 1))
                break;

            str += content[i];
            flag_str = true;
            i++;
        }

        if (flag_str)
            vector.push_back(str);

        str.clear();
    
        if (!flag && i < content.length() && content[i] != '{' && content[i] != '}' && content[i] != ';')
            i++;
    }

    return vector;
}
