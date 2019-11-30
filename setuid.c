#include "setuid.mdh"
#include "setuid.pro"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static int bin_setuid(char *nam, char **argv, Options ops, int func)
{
    int userId = strtol(*argv, (char**)NULL, 10);
    printf("calling setuid with user id %d\n", userId);
    if (setuid(userId)) {
        printf("setuid failed: %s\n", strerror(errno));
    } else {
        printf("success\n");
    }
    return 1;
}

static struct builtin bintab[] = {
    BUILTIN("setuid",    0, bin_setuid,    1,   1, 0, NULL, NULL),
};

static struct features module_features = {
    bintab, sizeof(bintab)/sizeof(*bintab),
    NULL, 0,
    NULL, 0,
    NULL, 0,
    0
};

/**/
int
setup_(Module m)
{
    printf("module loaded\n");
    return 0;
}

/**/
int
features_(Module m, char ***features)
{
    *features = featuresarray(m, &module_features);
    return 0;
}

/**/
int
enables_(Module m, int **enables)
{
    return handlefeatures(m, &module_features, enables);
}

/**/
int
boot_(Module m)
{
    return 0;
}

/**/
int
cleanup_(Module m)
{
    return setfeatureenables(m, &module_features, NULL);
}

/**/
int
finish_(Module m)
{
    return 0;
}
