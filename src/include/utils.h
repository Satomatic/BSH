#ifndef _BSH_UTILS_H_
#define _BSH_UTILS_H_

char** listdir(char* directory);
const char* getextension(const char* filename);

char* getInput(int buffer_size);
char **splitline(char *line);
char getch(void);

#endif