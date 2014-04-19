#ifndef MYCONIO_H
#define MYCONIO_H

#if defined(unix) || defined(__unix__) || defined(__unix)

int _getche();
int _getch();

#else
# include <conio.h>
#endif

#endif

