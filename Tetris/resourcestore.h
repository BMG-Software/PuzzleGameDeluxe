
#ifndef RESOURCE_STORE_H
#define RESOURCE_STORE_H

#include <SDL.h>

#include <unordered_map>
#include <string>
#include <memory>

class ResourceStore final
{
public:
    enum class Colour : unsigned char 
    {
        RED, GREEN, BLUE, PURPLE, ORANGE, BROWN, PINK
    };

    static ResourceStore& GetResourceStore();

    ResourceStore(const ResourceStore&) = delete;
    ResourceStore operator=(const ResourceStore&) = delete;
    ResourceStore(ResourceStore&&) = delete;
    ResourceStore operator=(ResourceStore&&) = delete;

    std::shared_ptr<SDL_Texture> LoadTextureColour(SDL_Renderer *ren, Colour colour);
    std::shared_ptr<SDL_Texture> LoadTextureFile(SDL_Renderer *ren, const std::string& file);

private:
    ResourceStore() = default;

    std::unordered_map<Colour, std::shared_ptr<SDL_Texture>> m_textureColours;
    std::unordered_map<std::string, std::shared_ptr<SDL_Texture>> m_textureFiles;

};

#endif // RESOURCE_STORE_H