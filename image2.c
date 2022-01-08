#include <FreeImage.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>


#define frontBlack    "\033[30m"
#define frontRed      "\033[31m"
#define frontGreen    "\033[32m"
#define frontYellow   "\033[33m"
#define frontBlue     "\033[34m"
#define frontMagenta  "\033[35m"
#define frontcyan     "\033[36m"
#define frontWhite    "\033[37m"

#define backBlack    "\033[40m"
#define backRed      "\033[41m"
#define backGreen    "\033[42m"
#define backYellow   "\033[43m"
#define backBlue     "\033[44m"
#define backMagenta  "\033[45m"
#define backcyan     "\033[46m"
#define backWhite    "\033[47m"
#define end          "\033[0m"

typedef struct rgb{
    int r;
    int g;
    int b;
}RGB;

typedef struct color{
    char *str;
    RGB rgb;
}Color;


Color c[] = {
    {frontBlack, {0, 0, 0}},
    {frontRed, {255, 0, 0}},
    {frontGreen, {0,255,0}},
    {frontYellow, {255, 255, 0}},
    {frontBlue, {0, 0, 255}},
    {frontMagenta, {255, 0, 255}},
    {frontcyan, {0, 255, 255}},
    {frontWhite, {255, 255, 255}}
};


Color bc[] = {
    //{frontBlack, {0, 0, 0}},
    {backRed, {255, 0, 0}},
    {backGreen, {0,255,0}},
    {backYellow, {255, 255, 0}},
    {backBlue, {0, 0, 255}},
    {backMagenta, {255, 0, 255}},
    {backcyan, {0, 255, 255}},
    {backWhite, {255, 255, 255}}
};

int calColorDistance(RGB c1, RGB c2){
    int r = c1.r - c2.r;
    int g = c1.g - c2.g;
    int b = c1.b - c2.b;

    return sqrt(r*r + g*g + b*b);
}

char banner[] = {"       Merry Christmas         "};

typedef struct score{
    int i;
    int score;
}Score;

char* findSimilarColor(int r, int g, int b){
    if (r == 0 && g == 0 && b==0){
        return c[0].str;
    }
    RGB c1 = {r, g, b};
    int i = 0, minScore = 65535, score = 0, j = 0;
    for (i = 1; i < sizeof(c)/sizeof(Color);i++){
        score = calColorDistance(c1, c[i].rgb);
        //printf("%d:%d-%d-%d\n\r", score, c1.r, c1.g, c1.b);
        if (score > minScore){
            continue;
        }else{
            minScore = score;
            j = i;
        }
    }
    
    return c[j].str;
}

int Delta = 2;

char ch[][100]= {
    {"@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. "},
    {"@%#*+=-:. "},
    {"@MBHENR#KWXDFPQASUZbdehx*8Gm&04LOVYkpq5Tagns69owz$CIu23Jcfry%1v7l+it[]{}?j|()=~!-/<>\\\"^_';,:`. "}};
char getCh(int r, int g, int b, int i){
    int gray = (int)(0.2126 * r + 0.7152 * g + 0.0722 * b);

    int len = strlen(ch[i]);

    int unit = 256/len + 1;
    
    //printf("len:%d unit:%d\r\n", len, unit);
    return ch[i][gray/unit];
}

int main(int argc, char** argv){
    if (argc < 2){
        printf("usage:./a.out path");
        return 0;
    }
    
    if (argc >= 3){
        Delta = atoi(argv[2]);
    }
    
    int i = 0;
    if (argc >= 4){
        i = atoi(argv[3]);
    }
    printf("%s", "\033[2J");
    FreeImage_Initialise(TRUE);

    const char* imageFile = "girl.jpg";
    imageFile = argv[1];

    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

    fif = FreeImage_GetFileType(imageFile, 0);
    if (fif == FIF_UNKNOWN){
        fif = FreeImage_GetFIFFromFilename(imageFile);
    }
    FIMULTIBITMAP *bitmap = NULL;

    bitmap = FreeImage_OpenMultiBitmap(fif, imageFile, FALSE, TRUE, FALSE, FIF_GIF);
    if (!bitmap){
        printf("Fail to load image\r\n");
    }else{
        //printf("load ok\r\n");
    }
    int page = FreeImage_GetPageCount(bitmap);
    int idx = 0;
    FIBITMAP *bitmap1 = NULL;
    FIBITMAP *bitmap2 = NULL;
    char loc[50] = {};
    for (idx = 0; idx < page; idx++){
        sprintf(loc, "\033[%d;%dH", 0, 0);
        printf("%s", loc);

        bitmap1 = FreeImage_LockPage(bitmap, idx);
        int width = FreeImage_GetWidth(bitmap1);
        int height = FreeImage_GetHeight(bitmap1);
        int bpp = FreeImage_GetBPP(bitmap1);
        int pitch = FreeImage_GetPitch(bitmap1);
        //printf("width:%d height:%d bpp:%d pitch:%d\r\n", width, height, bpp, pitch);
    int y = 0;
    int x = 0;
    int z = 0;
    for (y = height - 1; y >= 0; y = y - Delta){
        for (x = 0, z = 0; x < width; x = x + Delta, z++){
            RGBQUAD color;
            int ret = 0;
            bitmap2 = FreeImage_ConvertTo24Bits(bitmap1);
            ret = FreeImage_GetPixelColor(bitmap2, x, y, &color);
            if (ret){
                //printf("FreeImage_GetPixelColor ok\r\n");
            }else{
                printf("FreeImage_GetPixelColor fail\r\n");
            }

            //printf("%d_%d r:%d g:%d b:%d\r\n", x, y, color.rgbRed, color.rgbGreen, color.rgbBlue);
            if (y == height - 1 -Delta){
                 printf("%s%c%s",findSimilarColor(color.rgbRed, color.rgbGreen,color.rgbBlue ), getCh(color.rgbRed, color.rgbGreen, color.rgbBlue, i), end);
            }else{   
                printf("%s%c%s",findSimilarColor(color.rgbRed, color.rgbGreen,color.rgbBlue ), getCh(color.rgbRed, color.rgbGreen, color.rgbBlue, i), end);
            }
        }
        //usleep(1000*50);
        printf("\n");
    }

        FreeImage_UnlockPage(bitmap, bitmap1, FALSE);
    }
    return 0;
}
