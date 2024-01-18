#include "hw_check.h"
using namespace std;

Config::Config(string filename)
{
	cfg_name = filename;
	read_cfg();
}

Connection Config::get_connection()
{
	return connection;
}

string Config::get_dir()
{
	return source_dir;
}

void Config::read_cfg()
{
	cfgfile_read_tools cfg(cfg_name);
	if (cfg.is_open() == 0) 
	{
		cout << "配置文件 [" << cfg_name << "] 无法打开." << endl;
		return;
	}
	vector <string> group;
	cfg.get_all_group(group);

	const char* gname = "[数据库]";
	const char* db_host = "db_host";
	const char* db_port = "db_port";
	const char* db_name = "db_name";
	const char* db_username = "db_username";
	const char* db_passwd = "db_passwd";

	if (cfg.item_get_value(gname, db_host, "10.80.42.244", connection.host, 1) < 0)
		return;
	if (cfg.item_get_value(gname, db_port, "3306", connection.port, 1) < 0)
		return;
	if (cfg.item_get_value(gname, db_name, "homework", connection.dbname, 1) < 0)
		return;
	if (cfg.item_get_value(gname, db_username, "hwcheck", connection.username, 1) < 0)
		return;
	if (cfg.item_get_value(gname, db_passwd, "hw_CheCk-For23241*oOP", connection.passwd, 1) < 0)
		return;

	if (cfg.item_get_value("[文件目录]", "src_rootdir", "./", source_dir, 1) < 0)
		return;
}