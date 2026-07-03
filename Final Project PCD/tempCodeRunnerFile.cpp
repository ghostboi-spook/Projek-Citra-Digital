    // Image temp = LoadImageFromTexture(menu.originalimg);

    // Color* pixel = LoadImageColors(temp);
    // int total = temp.width * temp.height;
    // float tempCr, tempCg, tempCb, k;
    // for(int i = 0; i < total; i++)
    // {
    //     tempCr = 1 - (float)(pixel[i].r/255);
    //     tempCg = 1 - (float)(pixel[i].g/255);
    //     tempCb = 1 - (float)(pixel[i].b/255);
    //     k = min(tempCr, tempCg, tempCb);
    //     pixel[i].r = (tempCr - k / 1 - k)*255;
    //     pixel[i].g = (tempCg - k / 1 - k)*255;
    //     pixel[i].b = (tempCb - k / 1 - k)*255;
    // }

    // UpdateTexture(menu.show, pixel);
    // UnloadImage(temp);
    // UnloadImageColors(pixel);