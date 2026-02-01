#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>

#include "persist.hpp"

#define STORE1		"/var/games/kuiper-ranger/scores"
#define STORE2A		"%s/.local"
#define STORE2B		"%s/.local/state"
#define STORE2C		"%s/.local/state/kuiper-ranger"
#define STORE2D		"%s/.local/state/kuiper-ranger/scores"
#define STORE3		"%s/.kr-scores"

bool Persist::load(std::string &buf)
{
    const char *homedir = getenv("HOME");
    FILE *fp = NULL;

    if (access(STORE1, F_OK) == 0 &&
	access(STORE1, R_OK) == 0 &&
	access(STORE1, W_OK) == 0)
	fp = fopen(STORE1, "r+");

    if (fp == NULL && strlen(homedir) < 200) {
	char path[256];
	sprintf(path, STORE2A, homedir);
	(void)mkdir(path, 0777);
	sprintf(path, STORE2B, homedir);
	(void)mkdir(path, 0777);
	sprintf(path, STORE2C, homedir);
	(void)mkdir(path, 0777);
	sprintf(path, STORE2D, homedir);
	fp = fopen(path, "r+");
	if (fp == NULL)
	    fp = fopen(path, "w+");
    }

    if (fp == NULL && strlen(homedir) < 200) {
	char path[256];
	sprintf(path, STORE3, homedir);
	fp = fopen(path, "r+");
	if (fp == NULL)
	    fp = fopen(path, "w+");
    }

    if (fp == NULL)
	return false;

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (size < 0) {
	fclose(fp);
	return false;
    }

    buf.resize(size);
    size_t bytesRead = fread(&buf[0], 1, size, fp);
    if (bytesRead != (size_t)size) {
	fclose(fp);
	return false;
    }

    fclose(fp);
    return true;
}

// Return true on success
bool Persist::save(const std::string& buf)
{
    const char *homedir = getenv("HOME");
    FILE *fp = NULL;

    if (access(STORE1, F_OK) == 0 &&
	access(STORE1, R_OK) == 0 &&
	access(STORE1, W_OK) == 0)
	fp = fopen(STORE1, "w+");

    if (fp == NULL && strlen(homedir) < 200) {
	char path[256];
	sprintf(path, STORE2A, homedir);
	(void)mkdir(path, 0777);
	sprintf(path, STORE2B, homedir);
	(void)mkdir(path, 0777);
	sprintf(path, STORE2C, homedir);
	(void)mkdir(path, 0777);
	sprintf(path, STORE2D, homedir);
	fp = fopen(path, "w+");
    }

    if (fp == NULL && strlen(homedir) < 200) {
	char path[256];
	sprintf(path, STORE3, homedir);
	fp = fopen(path, "w+");
    }

    if (fp == NULL)
	return false;

    if (fwrite(&buf[0], 1, buf.size(), fp) != buf.size()) {
	fclose(fp);
	return false;
    }

    if (fclose(fp) < 0)
	return false;

    return true;
}
