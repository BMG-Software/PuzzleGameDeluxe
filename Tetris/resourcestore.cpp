
#include "resourcestore.h"

ResourceStore& ResourceStore::GetResourceStore()
{
    static ResourceStore rs;
    return rs;
}

std::shared_ptr<SDL_Texture> ResourceStore::LoadTextureColour(SDL_Renderer *ren, Colour colour)
{
    auto itr = m_textureColours.find(colour);
    if (itr != m_textureColours.end())
    {
        return itr->second;
    }
    else
    {
        // Unique logic
        Uint32 colourPixel = 0;
        auto surface = std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)>(SDL_CreateRGBSurface(0, 32, 32, 32, 0, 0, 0, 0), SDL_FreeSurface);
        switch (colour)
        {
        case Colour::RED:       colourPixel = SDL_MapRGB(surface->format,     255,    0,      0);     break;
        case Colour::GREEN:     colourPixel = SDL_MapRGB(surface->format,     0,      255,    0);     break;
        case Colour::BLUE:      colourPixel = SDL_MapRGB(surface->format,     0,      0,      255);   break;
        case Colour::PURPLE:    colourPixel = SDL_MapRGB(surface->format,     255,    0,      255);   break;
        case Colour::ORANGE:    colourPixel = SDL_MapRGB(surface->format,     165,    0,      255);   break;
        case Colour::BROWN:     colourPixel = SDL_MapRGB(surface->format,     127,    127,    255);   break;
        case Colour::PINK:      colourPixel = SDL_MapRGB(surface->format,     0,      127,    127);   break;
        default:                colourPixel = SDL_MapRGB(surface->format,     255,    255,    255);   break;
        }
        SDL_FillRect(surface.get(), NULL, colourPixel);
        // Unique logic end
        
        m_textureColours[colour] = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(ren, surface.get()), &SDL_DestroyTexture);
        return m_textureColours[colour];
    }
}

std::shared_ptr<SDL_Texture> ResourceStore::LoadTextureFile(SDL_Renderer *ren, const std::string& file)
{
    auto itr = m_textureFiles.find(file);
    if (itr != m_textureFiles.end())
    {
        return itr->second;
    }
    else
    {
        // Unique logic
        auto surface = std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)>(SDL_LoadBMP(file.c_str()), &SDL_FreeSurface);
        // Unique logic end
        
        m_textureFiles[file] = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(ren, surface.get()), &SDL_DestroyTexture);
        return m_textureFiles[file];
    }
}
