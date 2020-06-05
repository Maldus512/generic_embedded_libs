#include <string.h>
#include "unity.h"
#include "watcher/watcher.h"

char      var1 = 0;
int       var2 = 0;
long long var3 = 0;
struct {
    char      a;
    int       b;
    long      c;
    long long d;
} var4             = {0};
uint64_t array[10] = {0};

void setUp(void) {}
void tearDown(void) {}

void callback(void *var, void *data) {
    (void)var;
    (void)data;
}

void test_watcher() {
    watcher_t list[] = {
        WATCHER(&var1, 1, callback, NULL), WATCHER(&var2, 1, callback, NULL),   WATCHER(&var3, 1, callback, NULL),
        WATCHER(&var4, 1, callback, NULL), WATCHER(&array, 10, callback, NULL),
    };

    watcher_list_init(list, sizeof(list) / sizeof(list[0]), malloc);

    TEST_ASSERT(!WATCHER_PROCESS_CHANGES(list));
    var2++;
    TEST_ASSERT(WATCHER_PROCESS_CHANGES(list));
}