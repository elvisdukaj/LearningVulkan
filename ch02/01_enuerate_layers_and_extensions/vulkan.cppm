//
// Created by edukaj on 10/3/2024.
//

#if 0
import std;
#else
#include <iostream>
#include <ranges>
#include <string_view>
#include <vector>
#endif

#include <vulkan/vulkan.h>

export module vulkan;

export namespace Vulkan {

    struct LayerPropertiesAndExtensions {
        VkLayerProperties properties;
        std::vector<VkExtensionProperties> extensions;
    };

}// namespace Vulkan


//namespace std {
//    template<>
//    struct formatter<VkLayerProperties> {
//        constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
//
//        auto format(const VkLayerProperties &layerProperties, format_context &ctx) const {
//            return format_to(ctx.out(),
//                             "   spec version: {}\n"
//                             "   implementation version: {}\n"
//                             "   description: {}",
//                             layerProperties.specVersion, layerProperties.implementationVersion,
//                             layerProperties.description);
//        }
//    };
//}// namespace std

std::ostream &operator<<(std::ostream &os, const VkLayerProperties &layerProperties) {
    return os << "   spec version: " << layerProperties.specVersion << "\n"
              << "   implementation version: " << layerProperties.implementationVersion << "\n"
              << "   description: " << layerProperties.description;
}

//template<>
//struct std::formatter<VkExtensionProperties> {
//    constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }
//
//    auto format(const VkExtensionProperties &extensionProperties, std::format_context &ctx) const {
//        return std::format_to(ctx.out(),
//                              "   extension name: {}\n"
//                              "   spec version: {}\n",
//                              extensionProperties.extensionName, extensionProperties.specVersion);
//    }
//};

std::ostream &operator<<(std::ostream &os, const VkExtensionProperties &extensionProperties) {
    return os << "     - extension name: " << extensionProperties.extensionName << "\n"
              << "       spec version: " << extensionProperties.specVersion;
}

//template<>
//struct std::formatter<std::vector<VkExtensionProperties>> {
//    constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }
//
//    auto format(const std::vector<VkExtensionProperties> &extensionsProperties, std::format_context &ctx) const {
//        auto it = ctx.out();
//        for (const auto &extension: extensionsProperties) it = std::format_to(it, "{}", extension);
//        return it;
//    }
//};

std::ostream &operator<<(std::ostream &os, const std::vector<VkExtensionProperties> &extensionsProperties) {
    for (const auto &extension: extensionsProperties) os << extension << "\n";
    return os;
}

//using Vulkan::LayerPropertiesAndExtensions;
//
//template<>
//struct std::formatter<LayerPropertiesAndExtensions> {
//    constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }
//
//    auto format(const LayerPropertiesAndExtensions &layerPropertiesAndExtensions,
//                std::format_context &ctx) const {
//        return std::format_to(ctx.out(),
//                              "Layer: {}\n"
//                              " properties:\n{}\n",
//                              " extensions: {}", layerPropertiesAndExtensions.properties.layerName,
//                              layerPropertiesAndExtensions.properties, layerPropertiesAndExtensions.extensions);
//    }
//};

std::ostream &operator<<(std::ostream &os, const Vulkan::LayerPropertiesAndExtensions &layerPropertiesAndExtensions) {
    return os << "Layer " << layerPropertiesAndExtensions.properties.layerName << "\n"
              << " properties:\n"
              << layerPropertiesAndExtensions.properties << "\n"
              << " - extensions:\n"
              << layerPropertiesAndExtensions.extensions;
}

export namespace Vulkan {

    std::vector<VkLayerProperties> enumerateLayerProperties() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> result(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, result.data());

        return result;
    }

    std::vector<VkExtensionProperties> enumerateExtensionProperties(std::string_view layerName) {
        uint32_t extensionCount;
        vkEnumerateInstanceExtensionProperties(layerName.data(), &extensionCount, nullptr);

        std::vector<VkExtensionProperties> result(extensionCount);
        vkEnumerateInstanceExtensionProperties(layerName.data(), &extensionCount, result.data());

        return result;
    }

    std::vector<LayerPropertiesAndExtensions> enumerateLayerPropertiesAndExtensions() {
        auto layers = enumerateLayerProperties();

        auto extensions = layers |
                          std::views::transform(
                                  [](const VkLayerProperties &layerProperties) -> std::vector<VkExtensionProperties> {
                                      return enumerateExtensionProperties(layerProperties.layerName);
                                  }) |
                          std::ranges::to<std::vector<std::vector<VkExtensionProperties>>>();

        auto res = std::views::zip_transform(
                           [](auto &layer, auto &extension) -> LayerPropertiesAndExtensions {
                               return LayerPropertiesAndExtensions{layer, extension};
                           },
                           layers, extensions) |
                   std::ranges::to<std::vector<LayerPropertiesAndExtensions>>();

        return res;
    }

    auto print(const LayerPropertiesAndExtensions &layerPropertiesAndExtensions) {
        std::cout << layerPropertiesAndExtensions << std::endl;
    }

    auto print(std::vector<LayerPropertiesAndExtensions> &layersPropertiesAndExtensions) {
        for (const auto &layer: layersPropertiesAndExtensions) { Vulkan::print(layer); }
    }

}// namespace Vulkan
