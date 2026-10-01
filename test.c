#include <stdio.h>
#include "test.h"

static int reussites = 0;
static int echecs = 0;

void test_verifier(int condition, const char *nom)
{
    if (condition) {
        reussites++;
    } else {
        echecs++;
        printf("ECHEC : %s\n", nom);
    }
}

int test_reussites(void) { return reussites; }
int test_echecs(void)    { return echecs; }

int test_bilan(void)
{
    printf("tests : %d reussis, %d echoues\n", reussites, echecs);
    return echecs;
}
