#include <unistd.h>

int	main() {
	write(1, "Hello World! (from cgi)\n", 24);
	return 0;
}
