#include <stdio.h>
#include <string.h>

void show_help()
{
    printf("Usage: mp3_reader [option] [file]\n");
    printf("Options:\n");
    printf(" -v       View metadata\n");
    printf(" -e       Edit metadata\n");
    printf(" -h       Show help\n");
}
int main(int argc, char *argv[])
{
    // check arguments are passed
    if (argc < 2)
    {
        printf("Error: No arguments provided.\n");
        show_help();
        return 1;
    }

    if (strcmp(argv[1], "-h") == 0)
    {
        show_help();
    }
    else if (strcmp(argv[1], "-v") == 0)
    {
        if (argc < 3)
        {
            printf("Error: No file provided for viewing.\n");
            return 1;
        }

        // Open the file
        FILE *fp = fopen("original.mp3", "rb");
        if (fp == NULL)
        {
            printf("Error to open the file\n");
            return 1;
        }
        printf("File opened sucessfully\n");

        // skip the 10 bytes ID3 header
        fseek(fp, 10, SEEK_SET);

        
        for(int i=0;i<6;i++)
        {
        // Buffer to store the frame header(10 bytes)
        unsigned char frame_header[10];
        fread(frame_header, sizeof(char), 9, fp); // Read the frame header
        frame_header[10] = '\0';

        // Decode Frame ID (4 bytes)
        char frame_id[5];               // To store 4-character Id + null character
        strcpy(frame_id, frame_header); // copy first 4 bytes to frame_id
        frame_id[4] = '\0';
        printf("Frame ID: %s\n", frame_id);

        // Decode Frame size (4 bytes)
        unsigned int frame_size = (frame_header[4] << 24) |
                                  (frame_header[5] << 16) |
                                  (frame_header[6] << 8) |
                                  frame_header[7];
        printf("Frame Size: %u bytes\n", frame_size);

        // Skip Frame Flags (2 bytes)
        fseek(fp, 2, SEEK_CUR);

        // Read the frame content
        unsigned char frame_content[frame_size]; // Buffer for frame content
        fread(frame_content, 1, frame_size-1, fp);
        frame_content[frame_size-1] = '\0';
        printf("Frame Content: %s\n", frame_content);

        if (strcmp(frame_id, "TIT2") == 0)
            {
                printf("Title: %s\n", frame_content);
                printf("\n");
            }
            else if (strcmp(frame_id, "TPE1") == 0)
            {
                printf("Artist: %s\n", frame_content);
                printf("\n");
            }
            else if (strcmp(frame_id, "TALB") == 0)
            {
                printf("Album: %s\n", frame_content);
                printf("\n");
            }
            else if (strcmp(frame_id, "TYER") == 0)
            {
                printf("Year: %s\n", frame_content);
                printf("\n");
            }
            else if(strcmp(frame_id, "TCON") == 0)
            {
                printf("Content: %s\n", frame_content);
                printf("\n");
            }
            else if(strcmp(frame_id, "COMM") == 0)
            {
                printf("Comment: %s\n", frame_content);
                printf("\n");
            }
        }

        
    }

    else if (strcmp(argv[1], "-e") == 0)
    {
        printf("Edit option selected.\n");
    }
    else
    {
        printf("Unknown option\n");
    }
}