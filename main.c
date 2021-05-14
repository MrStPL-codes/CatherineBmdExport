/**<
I know the program is not optimal, but it doesn't have to be.
The files are small, so un-optimized code won't hurt them.
If I had more time, I might have improved it. :)

I figured out the file structure myself, so it's probably
not perfect, but as far as I know it's good enough.

Author: MrStPL
Icons: Blagoicons (from deviantart)
*/
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
FILE *fileBmd, *fileTxt, *fileBin;

//Thanks to Simon for this function - I'm still learning C
char *combine(char str1[], char str2[])
{
    static char result[50];
    strcpy(result, str1);
    strcat(result, str2);
    return result;
}

unsigned char read8()
{
    unsigned char memory = 0;
    fread(&memory, 1, 1, fileBmd);
    return memory;
}

void write8(unsigned char memory)
{
    fwrite(&memory, 1, 1, fileBin);
}

unsigned short read16()
{
    unsigned short memory = 0;
    fread(&memory, 2, 1, fileBmd);
    return __builtin_bswap16(memory);
}

unsigned int read32()
{
    unsigned int memory = 0;
    fread(&memory, 4, 1, fileBmd);
    return __builtin_bswap32(memory);;
}

void write16(unsigned short memory)
{
    unsigned short memoSwap = __builtin_bswap16(memory);
    fwrite(&memoSwap, 2, 1, fileBin);
}

void write32(unsigned int memory)
{
    unsigned int memoSwap = __builtin_bswap32(memory);
    fwrite(&memoSwap, 4, 1, fileBin);
}

void errorMess(char message[])
{
    printf("ERROR!\n%s\n", message);
}

void write16totxt(unsigned short memory)
{
    unsigned short memoSwap = __builtin_bswap16(memory);
    fwrite(&memoSwap, 2, 1, fileTxt);
}

//this function check what to do with bytes
void bmdDestoryForPc(unsigned short buff)
{
    bool imdnex = false;
    unsigned short buffForward = 0x0000; //buffer to checking next byte - need for "choosing dialog"
    unsigned short msgStop = 0xD821;
    while(buff!=msgStop){
        buff = read16();
        buffForward = read16();

        //I know this function it could be better but I don't have time :p
        if((buff == 0x0000)&&(buffForward==0x0000)){ //if there is two 0x0000 it means it "choosing dialog" and just make ENTER
            if(imdnex){
                write16(0x0000);
                write16totxt(0x000D);
                write16totxt(0x000A);
                imdnex = false;
                break;
            }
            else{
                write16totxt(0x005B);
                write16totxt(0x0051); //Q
                write16totxt(0x005D);
                write16totxt(0x000D);
                write16totxt(0x000A);
                break;
            }
        }
        else fseek(fileBmd, -2, SEEK_CUR); //if it's not "choosing dialog" return to previous position

        switch(buff){
        case 0xD821:
            write16(buff);
            break;
        case 0x0000:
            write16totxt(0x000D);
            write16totxt(0x000A);
            break;
        case 0xFFE3:
            buff = 0x0020;
            write16totxt(buff);
            break;
        case 0xDBA1:
            write16(buff);
            write16(read16());
            write16(read16());
            write16(read16());
            write16totxt(0x005B);
            write16totxt(0x0041); //A
            write16totxt(0x005D);
            break;
        case 0xDA8A:
            write16(buff);
            write16(read16());
            write16(read16());
            write16totxt(0x005B);
            write16totxt(0x0049); //I
            write16totxt(0x005D);
            break;
        case 0xDA61:
            write16(buff);
            write16(read16());
            write16(read16());
            write16totxt(0x005B);
            write16totxt(0x0043); //C
            write16totxt(0x005D);
            break;
        case 0xD828:
            write16totxt(0x007B);
            break;
        case 0xD829:
            write16(read16());
            write16totxt(0x007D);
            break;
        case 0xDB62:
            write16(buff);
            write16(read16());
            write16(read16());
            write16(read16());
            write16totxt(0x005B);
            write16totxt(0x0044); //D
            write16totxt(0x005D);
            break;
        case 0xD841:
            write16totxt(0x005B);
            write16totxt(0x0071); //q
            write16totxt(0x005D);
            break;
        case 0xD948:
            write16(buff);
            write16(read16());
            write16totxt(0x005B);
            write16totxt(0x0045); //E
            write16totxt(0x005D);
            break;
        case 0xDA63:
            write16(buff);
            write16(read16());
            write16(read16());
            write16totxt(0x005B);
            write16totxt(0x0046); //F
            write16totxt(0x005D);
            break;
        case 0xD901:
            write16(buff);
            write16(read16());
            write16totxt(0x005B);
            write16totxt(0x0042); //B
            write16totxt(0x005D);
            break;
        case 0xD94A:
            write16(buff);
            write16(read16());
            write16totxt(0x005B);
            write16totxt(0x0047); //G
            write16totxt(0x005D);
            break;
        case 0xDA49:
            write16(buff);
            write16(read16());
            write16(read16());
            write16totxt(0x005B);
            write16totxt(0x0048); //H
            write16totxt(0x005D);
            break;
        default:
            if(buff==0x3093) imdnex = true;
            write16totxt(buff);
            break;
        }
    }
}

//all almost the same but reversed bytes
void bmdDestoryForVita(unsigned short buff)
{
    bool imdnex = false;
    unsigned short buffForward = 0x0000;
    unsigned short msgStop = 0x21D8;
    while(buff!=msgStop){
        buff = read16();
        buffForward = read16();

        if((buff == 0x0000)&&(buffForward==0x0000)){
            if(imdnex){
                write16(0x0000);
                write16totxt(0x0D00);
                write16totxt(0x0A00);
                imdnex = false;
                break;
            }
            else{
                write16totxt(0x5B00);
                write16totxt(0x5100); //Q
                write16totxt(0x5D00);
                write16totxt(0x0D00);
                write16totxt(0x0A00);
                break;
            }
        }
        else fseek(fileBmd, -2, SEEK_CUR);

        switch(buff){
        case 0x21D8:
            write16(buff);
            break;
        case 0x0000:
            write16totxt(0x0D00);
            write16totxt(0x0A00);
            break;
        case 0x5F00:
            buff = 0x2000;
            write16totxt(buff);
            break;
        case 0xA1DB:
            write16(buff);
            write16(read16());
            write16(read16());
            write16(read16());
            write16totxt(0x5B00);
            write16totxt(0x4100); //A
            write16totxt(0x5D00);
            break;
        case 0x8ADA:
            write16(buff);
            write16(read16());
            write16(read16());
            write16totxt(0x5B00);
            write16totxt(0x4900); //I
            write16totxt(0x5D00);
            break;
        case 0x61DA:
            write16(buff);
            write16(read16());
            write16(read16());
            write16totxt(0x5B00);
            write16totxt(0x4300); //C
            write16totxt(0x5D00);
            break;
        case 0x28D8:
            write16totxt(0x7B00);
            break;
        case 0x29D8:
            write16(read16());
            write16totxt(0x7D00);
            break;
        case 0x62DB:
            write16(buff);
            write16(read16());
            write16(read16());
            write16(read16());
            write16totxt(0x5B00);
            write16totxt(0x4400); //D
            write16totxt(0x5D00);
            break;
        case 0x41D8:
            write16totxt(0x5B00);
            write16totxt(0x7100); //q
            write16totxt(0x5D00);
            break;
        case 0x48D9:
            write16(buff);
            write16(read16());
            write16totxt(0x5B00);
            write16totxt(0x4500); //E
            write16totxt(0x5D00);
            break;
        case 0x63DA:
            write16(buff);
            write16(read16());
            write16(read16());
            write16totxt(0x5B00);
            write16totxt(0x4600); //F
            write16totxt(0x5D00);
            break;
        case 0x01D9:
            write16(buff);
            write16(read16());
            write16totxt(0x5B00);
            write16totxt(0x4200); //B
            write16totxt(0x5D00);
            break;
        case 0x4AD9:
            write16(buff);
            write16(read16());
            write16totxt(0x5B00);
            write16totxt(0x4700); //G
            write16totxt(0x5D00);
            break;
        case 0x49DA:
            write16(buff);
            write16(read16());
            write16(read16());
            write16totxt(0x5B00);
            write16totxt(0x4800); //H
            write16totxt(0x5D00);
            break;
        default:
            if(buff==0x3093) imdnex = true;
            write16totxt(buff);
            break;
        }
    }
}


void makeMagicPC(char name[], bool forced)
{
    //checking file exist and make names
    fileBmd = fopen(name, "rb");
    if(!fileBmd){
        errorMess("BMD file not found!");
        return;
    }

    //making new file
    fileBin = fopen(combine(name, ".bin"), "wb");
    fileTxt = fopen(combine(name, ".txt"), "wb");

    //making buffer
    unsigned short buff16=0;

    //set txt file to BigEndian
    buff16 = 0xFEFF;
    write16totxt(buff16);

    //checking header file
    if(forced){
        fseek(fileBmd, 0x04, SEEK_SET);
    }
    else{
        if(read32()!=0x12345678){
                errorMess("missing header!");
                return;
        }
    }
    fseek(fileBmd, 0x1E, SEEK_SET);

    //checking quantity of messages
    int quantityOfMessages = 0;
    quantityOfMessages = read16();
    printf("There are %i messages.\n", quantityOfMessages);

    //checking size of file
    fseek(fileBmd, 0, SEEK_END);
    int sizeOfFile = ftell(fileBmd);
    printf("Size of file: %04X\n", sizeOfFile);

    fseek(fileBmd, 0, SEEK_SET); //Go to beginning of file
    unsigned short msgStart = 0xD841;

    printf("Please wait...\n");
    while(!feof(fileBmd)){
        buff16 = read16();
        if(buff16==msgStart){
            write16(msgStart);
            bmdDestoryForPc(buff16); //check what next in bmdDestoryForPc function
            write16totxt(0x005F); // adding '_' and ENTER to txt file
            write16totxt(0x000D);
            write16totxt(0x000A);
        }
        else if(!feof(fileBmd)) write16(buff16);
    }
    //check if last byte was skipped (if yes - fix that)
    if(sizeOfFile%2==1){
        fseek(fileBmd, -1, SEEK_END);
        write8(read8());
    }

    printf("Done.\n");
    printf("Author: MrStPL\n");
}

//all almost the same but reversed bytes
void makeMagicVita(char name[], bool forced)
{
    fileBmd = fopen(name, "rb");
    if(!fileBmd){
        errorMess("BMD file not found!");
        return;
    }
    fileBin = fopen(combine(name, ".bin"), "wb");
    fileTxt = fopen(combine(name, ".txt"), "wb");


    unsigned short buff16=0;
    buff16 = 0xFFFE;
    write16totxt(buff16);

    if(forced){
        fseek(fileBmd, 0x04, SEEK_SET);
    }
    else{
        if(read32()!=0x78563412){
                errorMess("missing header!");
                return;
        }
    }
    fseek(fileBmd, 0x1C, SEEK_SET);

    int quantityOfMessages = 0;
    quantityOfMessages = __builtin_bswap16(read16());
    printf("There are %i messages.\n", quantityOfMessages);

    fseek(fileBmd, 0, SEEK_END);
    int sizeOfFile = ftell(fileBmd);
    printf("Size of file: %04X\n", sizeOfFile);

    fseek(fileBmd, 0, SEEK_SET);
    unsigned short msgStart = 0x41D8;

    printf("Please wait...\n");
    while(!feof(fileBmd)){
        buff16 = read16();
        if(buff16==msgStart){
            write16(msgStart);
            bmdDestoryForVita(buff16);
            write16totxt(0x5F00);
            write16totxt(0x0D00);
            write16totxt(0x0A00);
        }
        else if(!feof(fileBmd)) write16(buff16);
    }

    if(sizeOfFile%2==1){
        fseek(fileBmd, -1, SEEK_END);
        write8(read8());
    }
    printf("Done.\n");
    printf("Author: MrStPL\n");
}

int main(int argc, char** argv)
{
    char name[50];
    bool reverse=false, forced=false;
    if(argc>1) {
        for(int i=1; i<argc; i++){
            switch(*argv[i]){
            case 'f': //setting forced method
                forced = true;
                printf("forced = true\n");
                break;
            case 'r': //setting reverse byte method
                reverse = true;
                printf("reverse = true\n");
                break;
            default:
                memcpy(name, argv[i], 50);
                break;
            }
        }
        if(reverse) makeMagicVita(name, forced);
        else makeMagicPC(name, forced);
    }
    else {
        printf("\nAuthor: MrStPL\n\n");
        printf("usage: catherineBmdExport \"files name\" *arguments* \n");
        printf("eg: catherineBmdExport lgmes.bmd f \n\n");
        printf("arguments:\n");
        printf("  - \t\tnothing - if you want to export text from standard bmd (PC/PS3/X360)\n");
        printf("r - \t\treverse bytes for Catherine Full Body (PsVita/PS4)\n");
        printf("f - \t\tforced method (if you want skip checking header file)\n");
        printf("\nWarning:\n");
        printf("If your filename starts with 'r' or 'f' then some error may occur!\n");
        printf("In that case, just change its name for a while to something else. ;)\n");
    }
    return 0;
}
