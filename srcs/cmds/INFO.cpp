#include "main.hpp"

std::string		time_diff(time_t start)
{
	std::string form_res = " running for: ";

	time_t rawtime;
	time(&rawtime);
	double sec = difftime(rawtime, start);
	double minute = 0;
	double hour = 0;

	for (; sec >= 60; sec -= 60)
	{
		minute += 1;
		if (std::fmod(minute, 60) == 0)
		{
			hour += 1;
			minute = 0;
		}
	}
	std::stringstream ho;
	ho << hour;
	std::stringstream min;
	min << minute;
	std::stringstream s;
	s << sec;
	form_res += ho.str() + "h" + min.str() + "m" + s.str() + "s\n";
	return form_res;
}

void	info(Server *srv, int &userfd, Command &cmd)
{
	(void)cmd;
	User * user = srv->getUser(userfd);
	std::string info_msg;

	info_msg = "-------INFO-------\nSoftware: " + srv->getName() + "\n" + "Version: Beta 1.0\n";

	struct stat file;
	stat(srv->getName().c_str(), &file);
	time_t t = file.st_ctime;
	struct tm *file_timeinfo = localtime(&t);

	info_msg += " compiled on: ";
	info_msg += asctime(file_timeinfo);

	time_t crea_time = srv->getCreatime();
	struct tm *timeinfo = localtime(&crea_time);

	info_msg += " running since: ";
	info_msg += asctime(timeinfo);
	info_msg += time_diff(crea_time);

	info_msg += "Author:\n -ldesnoye\n -aessakhi\n -antbarbi\n------------------";

	srv->sendReply(userfd, RPL_INFO(user->getNickname(), info_msg));
	srv->sendReply(userfd, RPL_ENDOFINFO(user->getNickname()));
}