# emptythreat

Linux proof of concept that loads and executes AES encrypted shellcode from extended file attributes(1)  

Uses the excellent tiny-AES-C https://github.com/kokke/tiny-AES-c  

load.c - This file will fetch key, iv and encrypted data from extended attributes. Data is decrypted and executed in memory. 
put.c  - This file will configure the extended attributes on a 'load' binary such that shellcode executes. 

This PoC explores an unusual method and location to store code. Copies of the file will not run the same as the original unless they have also been 'blessed' with the extended attributes. 

The code can be neutralised by removing any of those extended attributes.. 

```
gcc -Wall -o load load.c aes.c
gcc -Wall -o put put.c aes.c
```
Then run:
```
./load
```
And it will exit as no key or iv can be loaded. 

Next the file can be 'blessed' with ./put
```
./put ./shellcode silkyundulates11 meatypuppets0000 ./load
```
And next time ./load is run the shellcode will be executed
```
./load
Yello World 
```
However.. copies of the file that are made for backups etc don't work the same way..(2)   
e.g

```
cp ./load ./load.bak
./load.bak
echo $?
1
```


NOTES:    
(1) Assuming those are supported which is a _reasonably_ safe assumption    
(2) Depending how diligently the copy was made etc..    
