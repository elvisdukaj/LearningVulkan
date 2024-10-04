import vulkan;

int main() {
    auto layersAndExtensions = Vulkan::enumerateLayerPropertiesAndExtensions();
    Vulkan::print(layersAndExtensions);

    return 0;
}
