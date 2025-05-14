// BaseObject.cpp: Quản lý texture cơ bản, vẽ ảnh, text, điểm số và HP
#include "BaseObject.h"

// === Khởi tạo và hủy ===
// Khởi tạo BaseObject với texture null và kích thước 0
BaseObject::BaseObject()
{
    mTexture = nullptr;
    mWidth = 0;
    mHeight = 0;
}

// Hủy texture khi đối tượng bị xóa
BaseObject::~BaseObject()
{
    free();
}

// === Load texture từ file ảnh ===
// Tải ảnh từ file và tạo texture
// Trả về true nếu thành công, ngược lại là false
bool BaseObject::loadFromFile(std::string path)
{
    // Xóa texture cũ nếu có
    free();

    SDL_Texture* newTexture = nullptr;

    // Tải ảnh dưới dạng surface
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr)
    {
        std::cout << "Load hình ảnh thất bại: " << SDL_GetError() << std::endl;
    }
    else
    {
        // Tạo texture từ surface
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture != nullptr)
        {
            // Gán kích thước ảnh
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }
        
        // Xóa surface
        SDL_FreeSurface(loadedSurface);
    }

    // Gán texture mới
    mTexture = newTexture;

    // Kiểm tra kết quả
    return (mTexture != nullptr);
}

// === Load texture từ chuỗi text ===
// Tạo texture từ text
// Trả về true nếu thành công, ngược lại là false
bool BaseObject::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
    // Xóa texture cũ nếu có
    free();

    // Render text ra surface
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFontBig, textureText.c_str(), textColor);
    if (textSurface == nullptr)
    {
        std::cout << "Load font chữ thất bại: " << SDL_GetError() << std::endl;
    }
    else
    {
        // Tạo texture từ surface chứa text
        mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        if (mTexture != nullptr)
        {
            // Gán kích thước text
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        // Xóa surface chứa text
        SDL_FreeSurface(textSurface);
    }

    // Kiểm tra kết quả
    return mTexture != nullptr;
}

// === Vẽ texture ra màn hình ===
// Vẽ texture tại vị trí (x, y)
// clip: phần texture cần vẽ (nếu có)
void BaseObject::render(int x, int y, const SDL_Rect* clip)
{
    // Bỏ qua nếu texture hoặc renderer không hợp lệ
    if (mTexture == nullptr || gRenderer == nullptr)
    {
        std::cout << "Lỗi: Texture hoặc Renderer không hợp lệ." << std::endl;
        return;
    }

    // Tạo vùng cần vẽ
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};

    // Vẽ texture lên màn hình
    SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

// === Giải phóng bộ nhớ ===
// Giải phóng texture nếu có
void BaseObject::free()
{
    if (mTexture != nullptr)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
        mWidth = 0;
        mHeight = 0;
    }
}

// === Gán texture thủ công và cập nhật kích thước ===
// Gán texture mới và cập nhật kích thước
void BaseObject::setTexture(SDL_Texture* textureToRender)
{
    mTexture = textureToRender;

    if (mTexture != nullptr)
    {
        // Cập nhật kích thước
        SDL_QueryTexture(mTexture, nullptr, nullptr, &mWidth, &mHeight);
    }
}

// === Getter ===
// Trả về chiều rộng
int BaseObject::getWidth()
{
    return mWidth;
}

// Trả về chiều cao
int BaseObject::getHeight()
{
    return mHeight;
}

// Trả về texture hiện tại
SDL_Texture* BaseObject::getTexture()
{
    return mTexture;
}

// === Vẽ điểm số từ texture số ===
// Vẽ điểm số bằng các texture số
// renderer: bộ vẽ SDL, score: điểm số cần vẽ, x, y: vị trí bắt đầu, digitTextures: mảng texture chữ số 0-9
void renderScore(SDL_Renderer* renderer, int score, int x, int y, SDL_Texture* digitTextures[10])
{
    std::string scoreStr = std::to_string(score);
    int offsetX = 0;

    // Vẽ từng số
    for (char c : scoreStr)
    {
        int digit = c - '0';
        int w, h;

        // Lấy kích thước của số
        SDL_QueryTexture(digitTextures[digit], nullptr, nullptr, &w, &h);

        // Tạo vùng vẽ cho số
        SDL_Rect renderQuad = {x + offsetX, y, w, h};

        // Vẽ số
        SDL_RenderCopy(renderer, digitTextures[digit], nullptr, &renderQuad);

        // Dịch sang phải để vẽ số tiếp theo
        offsetX += w + 2; // khoảng cách 2 pixel giữa các chữ số
    }
}

// === Vẽ các biểu tượng HP ===
// Vẽ các icon HP
// hp: danh sách các texture HP
void renderHP(std::vector<SDL_Texture*> hp)
{
    int offsetX = 1000; // Vị trí bắt đầu vẽ HP trên trục x

    // Vẽ từng icon HP
    for (SDL_Texture* tex : hp)
    {
        SDL_Rect renderQuad = {offsetX, 30, 40, 40};
        SDL_RenderCopy(gRenderer, tex, NULL, &renderQuad);
        offsetX += 47; // khoảng cách giữa các biểu tượng HP
    }
}
