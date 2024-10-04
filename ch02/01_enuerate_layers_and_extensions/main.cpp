import vulkan;

int main() {

  auto layersAndExtensions = Vulkan::enumerateLayerPropertiesAndExtensions();
  for(const auto& layer : layersAndExtensions) {
      Vulkan::print(layer);
  }
  return 0;
}
