import os

# Output file paths
header_file = "include/textures.h"
source_file = "src/textures.cc"

# Start with the header content
header_content = "#pragma once\n#include <raylib.h>\n\n"

# Collect texture data
texture_vars = []  # For extern declarations in .h
texture_definitions = []  # For definitions in .cc
texture_entries = []  # For the TextureInfo array

# Walk through the assets/textures directory
textures_dir = "assets/textures"
for root, _, files in os.walk(textures_dir):
    for file in files:
        if file.endswith(".png"):
            # Generate variable name from filename (remove .png and replace - with _)
            var_name = file.replace(".png", "").replace("-", "_") + "_texture"
            # Full path relative to project root
            full_path = os.path.join(root, file).replace("\\", "/")
            # Store for header and source
            texture_vars.append(f"extern Texture2D {var_name};")
            texture_definitions.append(f"Texture2D {var_name};")
            texture_entries.append(f'    {{"{full_path}", &{var_name}}}')

# Calculate number of textures
num_textures = len(texture_entries)

# Complete header content
header_content += f"#define NUM_TEXTURES {num_textures}\n\n"
header_content += "typedef struct {\n  const char *path;\n  Texture2D *texture;\n} TextureInfo;\n\n"
header_content += "\n".join(texture_vars)
header_content += "\n\nvoid load_textures();\n"

# Create source content
source_content = f'#include "textures.h"\n#include <raylib.h>\n\n'
source_content += "\n".join(texture_definitions)
source_content += f"\n\nTextureInfo textures[{num_textures}] = {{\n"
source_content += ",\n".join(texture_entries)
source_content += "\n};\n\n"
source_content += "void load_textures() {\n  for (int i = 0; i < NUM_TEXTURES; i++) {\n"
source_content += "    *textures[i].texture = LoadTexture(textures[i].path);\n  }\n}\n"

# Write to files
with open(header_file, "w") as f:
    f.write(header_content)

with open(source_file, "w") as f:
    f.write(source_content)

print(f"Generated {header_file} and {source_file} with {num_textures} textures.")
