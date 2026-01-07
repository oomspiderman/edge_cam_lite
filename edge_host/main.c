// edge_host/main.c
#define _POSIX_C_SOURCE 200809L
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static const char *IN_DIR  = "img_samples/in";
static const char *OUT_DIR = "img_samples/out";

static int ensure_out_dir(void) {
    struct stat st;
    if (stat(OUT_DIR, &st) == 0) return 0;
    return mkdir(OUT_DIR, 0755);
}

static int is_regular_file(const char *path) {
    struct stat st;
    return (stat(path, &st) == 0 && S_ISREG(st.st_mode));
}

static int copy_file(const char *src, const char *dst) {
    int in = open(src, O_RDONLY);
    if (in < 0) { perror("open src"); return -1; }
    int out = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out < 0) { perror("open dst"); close(in); return -1; }

    char buf[64 * 1024];
    ssize_t n;
    while ((n = read(in, buf, sizeof buf)) > 0) {
        ssize_t off = 0;
        while (off < n) {
            ssize_t w = write(out, buf + off, (size_t)(n - off));
            if (w < 0) { perror("write"); close(in); close(out); return -1; }
            off += w;
        }
    }
    if (n < 0) perror("read");

    close(in);
    close(out);
    return (n < 0) ? -1 : 0;
}

int main(void) {
    if (ensure_out_dir() != 0) { perror("mkdir out"); return 1; }

    DIR *d = opendir(IN_DIR);
    if (!d) { perror("opendir in"); return 1; }

    struct dirent *e;
    char src[4096], dst[4096];
    while ((e = readdir(d)) != NULL) {
        if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0) continue;
        snprintf(src, sizeof src, "%s/%s", IN_DIR, e->d_name);
        if (!is_regular_file(src)) continue;
        snprintf(dst, sizeof dst, "%s/%s", OUT_DIR, e->d_name);
        if (copy_file(src, dst) != 0) {
            fprintf(stderr, "copy failed: %s -> %s\n", src, dst);
            // keep going; baby steps
        }
    }
    closedir(d);
    return 0;
}

