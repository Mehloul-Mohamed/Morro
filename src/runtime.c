#include <stddef.h>
#include <stdint.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 

struct cov_entry {
    char* start;
    char* end;
    size_t offset;
};

static struct cov_entry cov_cntrs[16];

static size_t cov_i = 0;
static size_t cov_t = 0;

static int shm_fd;
static uint8_t *shm_ptr;
#define COV_SIZE 65536
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

void __sanitizer_cov_8bit_counters_init(char *start, char *end) {
    if (cov_i >= 16) {
        return;
    }

    if (end <= start) {
        return;
    }

    cov_cntrs[cov_i].start = start;
    cov_cntrs[cov_i].end = end;
    cov_cntrs[cov_i].offset = cov_t;

    cov_i++;
    cov_t += (end-start);
}


static void copy_cov_map() {
    for (size_t i = 0; i < cov_i; i++) {
        size_t o = cov_cntrs[i].offset;
        char* s = cov_cntrs[i].start;
        char* e = cov_cntrs[i].end;

        if (o >= COV_SIZE) {
            break;
        }
        
        memcpy(shm_ptr + o, s, MIN((COV_SIZE - o), (size_t)(e - s)));
    }
}

__attribute__((constructor))
static void cov_init() {
    shm_fd = shm_open("/cov_map", O_RDWR, 0);
    if (shm_fd == -1) {
        return;
    }

    shm_ptr = mmap(0, COV_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    close(shm_fd);

    if (shm_ptr == MAP_FAILED) {
        shm_ptr = NULL;
        return;
    }

    atexit(copy_cov_map);
}
