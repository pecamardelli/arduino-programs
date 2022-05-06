#include "../ecu.h"

void logger(char *str, char *type) {
	char	*t_stamp;
	char	log_data[MAX_LINE_LEN];
	int		fd;
	time_t	date;

	fd = open(sys.log_file, O_CREAT | O_WRONLY | O_APPEND, 0666);

	if (fd == -1) return;

	date		= time(NULL);
	t_stamp		= ctime(&date);
	t_stamp[strlen(t_stamp)-1] = 0;

	sprintf(log_data, "%s [%s] %s\n", t_stamp, type, str);
/*
	if (strlen(log_data) + strlen(str) > MAX_LINE_LEN) {
		strncpy(log_data, str, MAX_LINE_LEN - strlen(log_data));
	}
	else strcpy(log_data, str);
*/
	write(fd, log_data, strlen(log_data));			
	close(fd);
}
