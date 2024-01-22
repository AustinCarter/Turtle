#include "AssetManager.h"
#include "turtpch.h"

#include "Turtle/Core/Hash.h"

namespace Turtle {

struct AssetManagerData {
  std::unordered_map<uint32_t, Ref<Texture2D>> TextureMap;

  AssetManager::ResourceData Stats;
};

static AssetManagerData s_AssetData;

void AssetManager::Init() { TURT_CORE_INFO("initializing asset manager"); }

Ref<Texture2D> AssetManager::CreateTexture(std::string path) {
  uint32_t textureID = MurmurHash(path.c_str(), path.size());
  Ref<Texture2D> texture;
  if (!Loaded(textureID)) {
    texture = Texture2D::Create(path);
    s_AssetData.Stats.TexturesLoaded++;
    s_AssetData.TextureMap[textureID] = texture;
  } else
    texture = GetTexture(textureID);

  return texture;
}

Ref<Texture2D> AssetManager::GetTexture(uint32_t textureID) {
  TURT_CORE_ASSERT(Loaded(textureID), "Invalid assetID Loaded");
  return s_AssetData.TextureMap[textureID];
}

void AssetManager::Unload() {
  std::unordered_map<uint32_t, Ref<Texture2D>>::iterator it =
      s_AssetData.TextureMap.begin();
  while (it != s_AssetData.TextureMap.end()) {
    if (it->second.use_count() == 1) {
      s_AssetData.Stats.TexturesLoaded--;
      it = s_AssetData.TextureMap.erase(it);
    } else {
      it++;
    }
  }
}

bool AssetManager::Loaded(uint32_t textureID) {
  return !(s_AssetData.TextureMap.find(textureID) ==
           s_AssetData.TextureMap.end());
}

AssetManager::ResourceData AssetManager::GetStats() {
  return s_AssetData.Stats;
}

void AssetManager::ResetStats() { s_AssetData.Stats.TexturesLoaded = 0; }
} // namespace Turtle
