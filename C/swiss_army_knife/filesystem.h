#pragma once

int GetFileSize(const char *name);
int DeleteFile(const char *name);

/* @name should be the mount directory's path */
int GetFreeSpace(const char *path);
