#include <unistd.h>

int	main() {
	write(1, "Hello World! (from cgi)\r\n", 25);
	return 0;
}
