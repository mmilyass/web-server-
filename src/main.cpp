/* ************************************************************************** */
/*                    file: main.cpp                                          */
/*                    by: ybounite <ybounite@student.1337.ma>                 */
/*                                                                            */
/*                    Created: 2025/11/26 13:25 by ybounite                   */
/*                    Updated: 2025/11/26 13:25 by ybounite                   */
/* ************************************************************************** */

#include "../includes/Webserv.hpp"
#include "../includes/Parser.hpp"
#include "../includes/Msg.hpp"

int main(int argc, char const *argv[])
{
	std::srand(std::time(0));
	std::vector<std::string> vector;
	signal(SIGPIPE, SIG_IGN);
	if (argc == 1 || argc == 2)
	{
		try
		{
			std::string config_path;
			Tokenizer token;
			Parser parser;
			config_path = (argc == 1 ? "configs/default.conf" : argv[1]);
			vector = token.tokenize(ConfigFileReader::read(config_path));
			Config config = parser.parse(vector);
			Server sct(config);
			sct.run();
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
	else
	{
		Msg::error("Bad arguments, should be ./webserv [configuration file], ");
		return 1;
	}
	return 0;
}
