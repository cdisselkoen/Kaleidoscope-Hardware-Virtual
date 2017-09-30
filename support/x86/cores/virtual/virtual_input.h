#include <stdbool.h>
#include <string>

// Returns TRUE if successful, FALSE if not
bool initVirtualInput(int argc, char* argv[]);

std::string getLineOfInput(bool anythingHeld);
bool isInteractive(void);
void printHelp(void);
