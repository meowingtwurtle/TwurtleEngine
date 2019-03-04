#include <randomcat/util/optional_filesystem.hpp>

#if RC_HAVE_FILESYSTEM
#    include <gsl/gsl_util>
#    include <stb/stb_image.hpp>

#    include "randomcat/engine/graphics/texture/texture_fs.hpp"

namespace randomcat::engine::graphics::textures {
    texture load_texture_file(fs::path const& _path) noexcept(false) {
        // Raw use of int required by stbi
        int width, height, burn;
        auto data = stbi_load(absolute(_path).c_str(), &width, &height, &burn, STBI_rgb_alpha);

        if (data == nullptr) { throw texture_load_error{"Unable to load texture with path: " + _path.string()}; }

        return texture{gsl::narrow<std::int32_t>(width), gsl::narrow<std::int32_t>(height), data};
    }

    texture const& load_texture_file(texture_manager& _manager, fs::path const& _path) noexcept(false) {
        return _manager.add_texture(_path.relative_path().string(), load_texture_file(_path));
    }
}    // namespace randomcat::engine::graphics::textures

#endif
