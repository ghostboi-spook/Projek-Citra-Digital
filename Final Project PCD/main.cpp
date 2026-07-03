#include<iostream>
#include<vector>
#include <functional>
#include <algorithm>
#include <set>
#include <cmath>
#include <raylib.h>
#include <raymath.h>
using namespace std;

int width = 1280, height = 720;
int histogram[256] = {0};
float avgAirlight = 0;

class MainMenu;

void darkChannel(Image original, Texture2D* output);
void transmissionFunction(Image darkChannel, Texture2D* output);
void perlinNoise(Image original, Texture2D* output);
void textureOutputFunction(Image original, Image darkChannel, Image transmission, Image perlinNoise, Texture2D* output);

float zoomRatio(Vector2 img)
{
    float scaleX = (float)(width - 280) / img.x;
    float scaleY = (float)(height - 80) / img.y;
    return min(scaleX, scaleY);
}

class MainMenu
{
public:
    FilePathList files;
    Image original, hazeRefrence;
    Texture2D textureOriginal;
    Texture2D textureHazeRefrence;
    Texture2D textureDarkChannel;
    Texture2D textureTransmission;
    Texture2D texturePerlinNoise;
    Texture2D textureOutput;
    Vector2 mousePosition[2] = {0, 0};

    Camera2D dummyCamera;
    int selectImg = 0;
    bool imgSelected = false;
    float mousescroolwheel[2] = {0};
    
    void init();
    void loadImgFromFiles();
    void pickImg();
    void pickImgDraw();
    void update();
    void drawImg();

};

int main()
{
    InitWindow(width, height, "Hello, World");
    MainMenu mainMenu;
    SetTargetFPS(60);

    mainMenu.init();

    while(!WindowShouldClose())
    {
        if(mainMenu.imgSelected == false)
        {
            mainMenu.pickImg();
        }
        else
        {
            mainMenu.update();
        }
        BeginDrawing();
            ClearBackground(BLACK);
            if(mainMenu.imgSelected == false)
            {
                mainMenu.pickImgDraw();
            } else if (mainMenu.imgSelected == true)
            {
                mainMenu.drawImg();
            }
        EndDrawing();
    }
    CloseWindow();
    UnloadDirectoryFiles(mainMenu.files);
    UnloadImage(mainMenu.original);
    UnloadImage(mainMenu.hazeRefrence);
    return 0;
}

void MainMenu::init()
{
    files = LoadDirectoryFiles("img");

    dummyCamera.offset = { 0, 0 };
    dummyCamera.target = { 0, 0 };
    dummyCamera.rotation = 0.0f;
    dummyCamera.zoom = 1.0f;
}

void MainMenu::loadImgFromFiles()
{
    original = LoadImage(files.paths[selectImg]);
    hazeRefrence = LoadImage("haze/haze3.png");
    ImageResize(&hazeRefrence, original.width, original.height);
    textureOriginal = LoadTextureFromImage(original);
    textureHazeRefrence = LoadTextureFromImage(hazeRefrence);
    textureDarkChannel = LoadTextureFromImage(hazeRefrence);
    textureTransmission = LoadTextureFromImage(hazeRefrence);
    texturePerlinNoise = LoadTextureFromImage(hazeRefrence);
    textureOutput = LoadTextureFromImage(hazeRefrence);
}

void MainMenu::pickImg()
{
    if((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)))
    {
        selectImg++;
    }

    if((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)))
    {
        selectImg--;
    }

    if(selectImg < 0)
    {
        selectImg = (int)files.count - 1;
    }

    if(selectImg == (int)files.count)
    {
        selectImg = 0;
    }

    if(IsKeyPressed(KEY_ENTER))
    {
        imgSelected = true;

        loadImgFromFiles();

        //test1
        darkChannel(hazeRefrence, &textureDarkChannel);
        transmissionFunction(LoadImageFromTexture(textureDarkChannel), &textureTransmission);
        perlinNoise(original, &texturePerlinNoise);
        textureOutputFunction(original, LoadImageFromTexture(textureDarkChannel), LoadImageFromTexture(textureTransmission), LoadImageFromTexture(texturePerlinNoise), &textureOutput);

        dummyCamera.offset = {0, 0};
        dummyCamera.zoom = zoomRatio({ (float)textureDarkChannel.width, (float)textureDarkChannel.height });
        dummyCamera.rotation = 0.0f;
        dummyCamera.target = {0, -50};
    }
}

void MainMenu::update()
{
    mousePosition[0] = GetMousePosition();
    mousescroolwheel[0] = GetMouseWheelMove() / 10;
    if(IsMouseButtonDown(0) || IsKeyDown(KEY_H))
    {
        if(mousePosition[0] != mousePosition[1])
        {
            dummyCamera.target -= (mousePosition[0] - mousePosition[1]) / dummyCamera.zoom;
        }
    }
    mousePosition[1] = mousePosition[0];

    dummyCamera.zoom += mousescroolwheel[0];

    if(IsKeyPressed(KEY_BACKSPACE))
    {
        imgSelected = false;
        UnloadImage(original);
        UnloadImage(hazeRefrence);
        UnloadTexture(textureDarkChannel);
        UnloadTexture(textureTransmission);
        UnloadTexture(texturePerlinNoise);
        UnloadTexture(textureOutput);
    }
}

void MainMenu::pickImgDraw()
{
    int offsetY = 23;
    int numberOffset = 1;

    if(files.count == 0)
    {
        DrawText("No Img found", 20, 20, 20, RED);
    } else
    {
        DrawRectangle(23, offsetY*(selectImg+1), 687, 20, GREEN);
    
        for (int i = 0; i < (int)files.count; i++)
        {
            if (IsFileExtension(files.paths[i], ".png"))
            {
                DrawText(files.paths[i], 23, offsetY*numberOffset, 20, WHITE);
                numberOffset++;
            }
        }
    }
}

void MainMenu::drawImg()
{
    //test2
    int w = original.width;
    int offset = 0;
    int offsetX = original.width/20;
    BeginMode2D(dummyCamera);
        DrawTexture(textureDarkChannel, (offset * w) + (offsetX * offset), 0, WHITE);
        DrawText(TextFormat("Dark Channel"), (offset * w) + (offsetX * offset), -50, 50, WHITE);
        offset++;
        DrawTexture(textureTransmission, (offset * w) + (offsetX * offset), 0, WHITE);
        DrawText(TextFormat("Transmission"), (offset * w) + (offsetX * offset), -50, 50, WHITE);
        offset++;
        DrawTexture(texturePerlinNoise, (offset * w) + (offsetX * offset), 0, WHITE);
        DrawText(TextFormat("PerlinNoise"), (offset * w) + (offsetX * offset), -50, 50, WHITE);
        offset++;
        DrawTexture(textureOriginal, (offset * w) + (offsetX * offset), 0, WHITE);
        DrawText(TextFormat("Original"), (offset * w) + (offsetX * offset), -50, 50, WHITE);
        offset++;
        DrawTexture(textureOutput, (offset * w) + (offsetX * offset), 0, WHITE);
        DrawText(TextFormat("Output"), (offset * w) + (offsetX * offset), -50, 50, WHITE);

    EndMode2D();
}

void darkChannel(Image original, Texture2D* output)
{
    //variabel
    Image temp = original;

    int w = temp.width;
    int h = temp.height;

    //histogram untuk menghitung air light
    for(int i = 0; i < 256;i++)
    {
        histogram[i] = {0};
    }

    int radius = 7;
    int minRGB = 0;

    Color* dChannel = new Color[w *h];

    Color* pixel = LoadImageColors(temp);

    for(int y = 0; y < h; y++)
    {
        for(int x = 0; x < w; x++)
        {
            int minVal = 255;

            //scan pixel tetangga untuk mencari nilai RGB terkecil
            for (int dy = -radius; dy <= radius; dy++)
            {
                for (int dx = -radius; dx <= radius; dx++) 
                {
                    //pojok citra
                    int nx = max(0, min(x + dx, w - 1));
                    int ny = max(0, min(y + dy, h - 1));
                    
                    Color p = pixel[ny * w + nx];

                    //mencari nilai terkecil antara RGB pada suati pixel
                    minRGB = min({p.r, p.g, p.b});


                    //membandingkan nilai RGB yang terkecil pada pixel saat ini dengan pixel yang lain pada jendela pixel
                    if (minRGB < minVal)
                    {
                        minVal = minRGB;
                    }
                }
            }

            //menyimpan nilai RGB terkecil pada histogram untuk menghitung rata rata air light

            histogram[minVal]++;
            dChannel[y * w + x ] = {(unsigned char)minVal, (unsigned char)minVal, (unsigned char)minVal, 255};
        }
    }

    UpdateTexture(*output, dChannel);
    UnloadImageColors(pixel);
    delete[] dChannel;
}

void transmissionFunction(Image darkChannel, Texture2D* output)
{
    Image tempDark = darkChannel;

    int w = tempDark.width;
    int h = tempDark.height;

    //mengambil 1% nilai dark channel tertinggi untuk mencari rata rata cahaya pada kabut
    int airlightMax = (w * h) * 0.01f;

    float tempTransmission = 0;
    
    avgAirlight = 0;
    float omega = 0.95f;

    Color* transmission = new Color[w *h];

    Color* darkPixel = LoadImageColors(tempDark);

    for(int i = 255, j = airlightMax; j > 0 && i >= 0; i--)
    {
        cout << "histogram[" << i << "]" << histogram[i] << '\n';
        while(histogram[i] > 0 && j > 0)
        {
            avgAirlight += i;
            histogram[i]--;
            j--;
        }
    }

    avgAirlight /= airlightMax;

    for(int y = 0; y < h;y++)
    {
        for(int x = 0; x < w; x++)
        {
            //omega digunakan agar kabut tidak sepenuhnya transparant
            //dark pixel atau dark channel pada pixel tertentu akan dibagi dengan rata rata airlight untuk menghasilkan nilai ratio
            // 
            tempTransmission = 1.0f - (omega * ((float)darkPixel[y * w + x].r/avgAirlight));

            transmission[y * w + x].r = (unsigned char)(tempTransmission * 255);
            transmission[y * w + x].g = (unsigned char)(tempTransmission * 255);
            transmission[y * w + x].b = (unsigned char)(tempTransmission * 255);
            transmission[y * w + x].a = 255;
        }
    }

    UpdateTexture(*output, transmission);
    UnloadImage(tempDark);
    UnloadImageColors(darkPixel);
    delete[] transmission;
}

void perlinNoise(Image original, Texture2D* output)
{
    int w = original.width;
    int h = original.height;
    //cara kerja perlin noise:
    //1. membuat grid 
    //2. membuat vector dengan arah random pada titik grid
    //3. menentukan posisi vector dan jarak dengan 4 grid
    //4. melakukan dot product pada setiap grid 
    // (cos(theta) * Px) + (sin(theta) *Py)
    //5. masukan kordinat pixel pada rumus 
    //f(t) = 6t^5 - 15t^4 + 10t^3
    // untuk membuat kurva efek penghapus
    //6. atas: 1 - dot horizontal(x) * dor product kanan-atas + dot horizontal(x) * dot product kanan-atas
    //   bawah: 1 - dot horizontal(x) * dor product kiri-bawah + dot horizontal(x) * dot product kanan-bawah
    //  P = 1- dot horizontal(x) * hasil atas + dot horizontal(x) * hasil bawah 

    Image temp =  GenImagePerlinNoise(w, h, 50, 50, 2.41f);
    Color* pixel = LoadImageColors(temp);
    UpdateTexture(*output, pixel);
    UnloadImage(temp);
    UnloadImageColors(pixel);
}

void textureOutputFunction(Image original, Image darkChannel, Image transmission, Image perlinNoise, Texture2D* output)
{
    int w = original.width;
    int h = original.height;

    float lamda = 0.5f;

    Color* tempOriginal = LoadImageColors(original);
    Color* tempDark = LoadImageColors(darkChannel);
    Color* tempTransmission = LoadImageColors(transmission);
    Color* tempPerlinNoise = LoadImageColors(perlinNoise);
    Color* outputPixel = new Color[w * h];

    for(int y = 0; y < h; y++)
    {
        for(int x = 0; x < w; x++)
        {
            float t = tempTransmission[y * w + x].r/255.0f;
            float p = tempPerlinNoise[y * w + x].r/255.0f;

            //normalisasi nilai transmission agar kabut tidak terlalu tebal
            // lambda di pakai pada jurnal dengan perlin noise +50% agar tidak menghasilkan 0 dan mengakibatkan kabut total
            t = pow(t, lamda * (p + 0.5f));

            outputPixel[y * w + x].r = tempOriginal[y * w + x].r * t + (avgAirlight * (1.0f - t));
            outputPixel[y * w + x].g = tempOriginal[y * w + x].g * t + (avgAirlight * (1.0f - t));
            outputPixel[y * w + x].b = tempOriginal[y * w + x].b * t + (avgAirlight * (1.0f - t));
            outputPixel[y * w + x].a = 255;
        }
    }
    UpdateTexture(*output, outputPixel);
    UnloadImageColors(tempOriginal);
    UnloadImageColors(tempDark);
    UnloadImageColors(tempTransmission);
    UnloadImageColors(tempPerlinNoise);
    delete[] outputPixel;   
}
