namespace Crypt
{
	extern std::string encrypt(const std::string& str_in, const std::string& key, const std::string& iv);
	extern std::string decrypt(const std::string& str_in, const std::string& key, const std::string& iv);
}