#include "../interpreter.h"
#include "../picoc.h"

#include <stdio.h>


#ifdef USE_READLINE
//#include <readline/readline.h>
//#include <readline/history.h>
#include <apps/readline.h>
#endif


/* mark where to end the program for platforms which require this */
int PicocExitBuf[41];

/* deallocate any storage */
void PlatformCleanup()
{
}

/* get a line of interactive input */
char *PlatformGetLine(char *Buf, int MaxLen, const char *Prompt)
{


#ifdef USE_READLINE
    if (Prompt != NULL)
        printf("%s", Prompt);
    ssize_t len = readline(Buf, MaxLen, stdin, stdout);
    Buf[len-1] = '\0';
    return Buf;
    /*
    if (Prompt != NULL)
    {
        char *InLine = readline(Prompt);
        if (InLine == NULL)
            return NULL;
    
        Buf[MaxLen] = '\0';
        strncpy(Buf, InLine, MaxLen-1);
        strncat(Buf, "\n", MaxLen-1);
        
        if (InLine[0] != '\0')
            add_history(InLine);
            
        free(InLine);
        return Buf;
    }
    */
#endif

    if (Prompt != NULL)
        printf("%s", Prompt);
        
    fflush(stdout);
    return fgets(Buf, MaxLen, stdin);
}

/* write a character to the console */
void PlatformPutc(unsigned char OutCh, union OutputStreamInfo *Stream)
{
    if (OutCh == '\n')
        putchar('\r');
        
    putchar(OutCh);
}

/* read a character */
int PlatformGetCharacter()
{
    return getchar();
}

/* exit the program */
void PlatformExit(int RetVal)
{
    PicocExitValue = RetVal;
    PicocExitBuf[40] = 1;
    //longjmp(PicocExitBuf, 1);
}

/* read a file into memory */
char *PlatformReadFile(const char *FileName)
{
    /*
    struct stat FileInfo;
    char *ReadText;
    FILE *InFile;
    int BytesRead;
    
    if (stat(FileName, &FileInfo))
        ProgramFail(NULL, "can't read file %s\n", FileName);
    
    ReadText = malloc(FileInfo.st_size + 1);
    if (ReadText == NULL)
        ProgramFail(NULL, "out of memory\n");
        
    InFile = fopen(FileName, "r");
    if (InFile == NULL)
        ProgramFail(NULL, "can't read file %s\n", FileName);
    
    BytesRead = fread(ReadText, 1, FileInfo.st_size, InFile);
    if (BytesRead == 0)
        ProgramFail(NULL, "can't read file %s\n", FileName);

    ReadText[BytesRead] = '\0';
    fclose(InFile);
    
    return ReadText;    
    */
    return NULL;
}

/* read and scan a file for definitions */
void PicocPlatformScanFile(const char *FileName)
{
    char *SourceStr = PlatformReadFile(FileName);

    PicocParse(FileName, SourceStr, strlen(SourceStr), TRUE, FALSE, TRUE);
}


