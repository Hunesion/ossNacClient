#ifndef _HUNE_CORE_SYS_UTILS_H_
#define _HUNE_CORE_SYS_UTILS_H_
#include "macro.h"
#include <string>

BEGIN_HUNE_CORE
std::string getIp();
std::string getIpAndMac();

int getPID();
unsigned long int getThreadId(); 


void hune_system_init(); 
bool hune_is_main_thread(); 
int  hune_command(const std::string &param, int wfd = 0);
int  hune_xdg_open(const std::string &uri);
int  hune_open_default_uri(const std::string &param);
int  hune_open_url(const std::string &url);
int  hune_open_file(const std::string &file);
bool hune_copy_right_click_script();

END_HUNE_CORE

#endif /* _HUNE_CORE_SYS_UTILS_H_ */