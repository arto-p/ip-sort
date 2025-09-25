/* =====================================================================================
 *
 *       Filename: ip-sort.c
 *
 *    Description: ip addr sort 
 *
 *        Version:  1.0
 *        Created:  09/25/25 11:24:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  Private disorganize person
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>

#define OCTET1(a) ((((uint32_t)a) >> 24) & 0xFF)
#define OCTET2(a) ((((uint32_t)a) >> 16) & 0xFF)
#define OCTET3(a) ((((uint32_t)a) >> 8)  & 0xFF)
#define OCTET4(a) ((((uint32_t)a) >> 0)  & 0xFF)

#define ADDRS_SIZE 1024

in_addr_t *addrs;
size_t addrs_count = 0, addrs_size = 0;

static bool addr_add(in_addr_t addr)
{
    if (addrs_size == 0 || addrs_count >= addrs_size) {
        if ((addrs = reallocarray(addrs, addrs_size += ADDRS_SIZE, sizeof(*addrs))) == NULL) {
            perror("reallocarray");
            return false;
        }
    }
    addrs[addrs_count++] = addr;
    return true;
}

static int cmpr(const void *a, const void *b)
{
    uint32_t addr1 = htonl((uint32_t)(*(in_addr_t *)a));
    uint32_t addr2 = htonl((uint32_t)(*(in_addr_t *)b));
    if (addr1 < addr2) return 1;
    if (addr1 > addr2) return -1;
    return 0;
}

int main(int argc, char **argv)
{
    char buffer[512], *pc;
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if ((pc = strchr(buffer, '\t')) != NULL) *pc = '\0';
        in_addr_t addr = inet_addr(buffer);
        if (addr == INADDR_NONE) {
            fprintf(stderr, "invalid ip addr: %s\n", buffer);
            continue;
        }
        if (!addr_add(addr)) abort();
    }

    qsort(addrs, addrs_count, sizeof(in_addr_t), cmpr);

    // Sorted list
    for (int i = 0; i < addrs_count; i++) {
        printf("%s\n", inet_ntoa((struct in_addr){ .s_addr = addrs[i] }));
    }
    // First byte == 1
    for (int i = 0; i < addrs_count; i++) {
        if (OCTET4(addrs[i]) == 1) {
            printf("%s\n", inet_ntoa((struct in_addr){ .s_addr = addrs[i] }));
        }
    }
    // First byte == 46, second byte == 70
    for (int i = 0; i < addrs_count; i++) {
        if (OCTET4(addrs[i]) == 46 && OCTET3(addrs[i]) == 70) {
            printf("%s\n", inet_ntoa((struct in_addr){ .s_addr = addrs[i] }));
        }
    }
    // Any byte == 46
    for (int i = 0; i < addrs_count; i++) {
        if (OCTET4(addrs[i]) == 46 || OCTET3(addrs[i]) == 46 ||
            OCTET2(addrs[i]) == 46 || OCTET1(addrs[i]) == 46) {
            printf("%s\n", inet_ntoa((struct in_addr){ .s_addr = addrs[i] }));
        }
    }
    exit(EXIT_SUCCESS);
}

// vim: syn=c ts=4 sw=4 smarttab expandtab
/// That's all, folks!
