//
// Created by edukaj on 10/3/2024.
//

module;

#include <vulkan/vulkan.h>

#include <print>
#include <ranges>
#include <sstream>
#include <string_view>
#include <vector>

export module vulkan;

export namespace Vulkan {
    struct LayerPropertiesAndExtensions {
        VkLayerProperties properties;
        std::vector<VkExtensionProperties> extensions;
    };

    struct LayersPropertiesAndExtensions {
        std::vector<VkExtensionProperties> instanceExtensions;
        std::vector<LayerPropertiesAndExtensions> layersPropertiesAndExtensions;
    };

}// namespace Vulkan


template<>
struct std::formatter<VkLayerProperties> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    auto format(const VkLayerProperties& layerProperties, std::format_context& ctx) const {
        return format_to(ctx.out(), "   - layer name: {}\n",
                         "     spec version: {}\n"
                         "     implementation version: {}\n"
                         "     description: {}",
                         layerProperties.layerName, layerProperties.specVersion, layerProperties.implementationVersion,
                         layerProperties.description);
    }
};

template<>
struct std::formatter<VkExtensionProperties> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    auto format(const VkExtensionProperties& extensionProperties, std::format_context& ctx) const {
        return std::format_to(ctx.out(),
                              "     - name: {}\n"
                              "       spec version: {}",
                              extensionProperties.extensionName, extensionProperties.specVersion);
    }
};

template<>
struct std::formatter<std::vector<VkExtensionProperties>> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    auto format(const std::vector<VkExtensionProperties>& extensionsProperties, std::format_context& ctx) const {
        auto out = ctx.out();
        for (const VkExtensionProperties& extension: extensionsProperties) {
            out = std::format_to(std::move(ctx.out()), "{}\n", extension);
        }
        return out;
    }
};

template<>
struct std::formatter<Vulkan::LayerPropertiesAndExtensions> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    auto format(const Vulkan::LayerPropertiesAndExtensions& layerPropertiesAndExtensions,
                std::format_context& ctx) const {
        return std::format_to(ctx.out(),
                              "- layer: {}\n"
                              "    properties: {}\n",
                              "    extensions: {}", layerPropertiesAndExtensions.properties.layerName,
                              layerPropertiesAndExtensions.properties, layerPropertiesAndExtensions.extensions);
    }
};

template<>
struct std::formatter<Vulkan::LayersPropertiesAndExtensions> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    auto format(const Vulkan::LayersPropertiesAndExtensions& layersPropertiesAndExtensions,
                std::format_context& ctx) const {
        auto out = std::format_to(std::move(ctx.out()), "Instance extensions: {}\nLayers:\n",
                                  layersPropertiesAndExtensions.instanceExtensions);
        for (const Vulkan::LayerPropertiesAndExtensions& layerPropertiesAndExtensions:
             layersPropertiesAndExtensions.layersPropertiesAndExtensions) {
            out = std::format_to(std::move(ctx.out()), "{}", layerPropertiesAndExtensions);
        }
        return out;
    }
};

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

    LayersPropertiesAndExtensions enumerateLayerPropertiesAndExtensions() {
        auto toVectorExtensionProperties =
                [](const VkLayerProperties& layerProperties) -> std::vector<VkExtensionProperties> {
            return enumerateExtensionProperties(layerProperties.layerName);
        };

        auto toLayerPropertiesAndExtensions =
                [](const VkLayerProperties& layer,
                   const std::vector<VkExtensionProperties>& extension) -> LayerPropertiesAndExtensions {
            return LayerPropertiesAndExtensions{layer, extension};
        };

        LayersPropertiesAndExtensions layersAndExtensions;

        layersAndExtensions.instanceExtensions = enumerateExtensionProperties("");

        auto layers = enumerateLayerProperties();
        auto extensions = layers | std::views::transform(toVectorExtensionProperties) |
                          std::ranges::to<std::vector<std::vector<VkExtensionProperties>>>();

        layersAndExtensions.layersPropertiesAndExtensions =
                std::views::zip_transform(toLayerPropertiesAndExtensions, layers, extensions) |
                std::ranges::to<std::vector<LayerPropertiesAndExtensions>>();

        return layersAndExtensions;
    }

    void print(const LayerPropertiesAndExtensions& layerPropertiesAndExtensions) {
        std::println(" - layer name: {}", layerPropertiesAndExtensions.properties.layerName);
        std::println("   spec version: {}", layerPropertiesAndExtensions.properties.specVersion);
        std::println("   implementation version: {}", layerPropertiesAndExtensions.properties.implementationVersion);
        std::println("   extensions:");

        for (const auto extension: layerPropertiesAndExtensions.extensions) {
            std::println("     - name: {}", std::string_view{extension.extensionName});
            std::println("       version: {}", extension.specVersion);
        }
    }

    auto print(LayersPropertiesAndExtensions& layersPropertiesAndExtensions) {

        std::println("Instance extensions:");
        for (const auto extension: layersPropertiesAndExtensions.instanceExtensions) {
            std::println("  - name: {}\n"
                         "    version: {}",
                         extension.extensionName, extension.specVersion);
        }

        std::println("Layers:");
        for (const auto& layer: layersPropertiesAndExtensions.layersPropertiesAndExtensions) { Vulkan::print(layer); }
    }
}// namespace Vulkan
