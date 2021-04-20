#include <stdio.h>

int main(void)
{
    FILE *inputFile, *outputFile;
    char inputPath[20],outputPath[20];

    printf("Input path of the source: ");
    scanf("%s",&inputPath);
    if((inputFile = fopen(inputPath, "r")) == NULL)
    {
        printf("%s is not exist!\n",inputPath);
        exit(1);
    }

    printf("Input path of the destination: ");
    scanf("%s",&outputPath);
    if((outputFile = fopen(outputPath, "w")) == NULL)
    {
        printf("%s is not exist!\n",outputPath);
        exit(1);
    }

    char buf[256];
    int bufSize = sizeof(buf);
    while(fgets(buf,bufSize,inputFile) != NULL)
        fputs(buf,outputFile);
    fclose(inputFile);
    fclose(outputFile);
    return 0;
}
