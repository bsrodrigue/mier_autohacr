import os

# Output file paths
header_file = "include/shaders.h"
source_file = "src/shaders.cc"

# Start with the header content
header_content = "#pragma once\n#include <raylib.h>\n\n"

# Collect shader data
shader_vars = []  # For extern declarations in .h
shader_definitions = []  # For definitions in .cc
shader_entries = []  # For the ShaderInfo array

# Walk through the assets/shaders directory
shaders_dir = "assets/shaders"
for root, dirs, files in os.walk(shaders_dir):
    folder_name = os.path.basename(root)
    if folder_name != "shaders":  # Skip the root "assets/shaders" itself
        vertex_path = None
        fragment_path = None
        for file in files:
            full_path = os.path.join(root, file).replace("\\", "/")
            if file.endswith(".vs"):
                vertex_path = full_path
            elif file.endswith(".fs"):
                fragment_path = full_path
        if vertex_path or fragment_path:
            var_name = folder_name.replace("-", "_") + "_shader"
            shader_vars.append(f"extern Shader {var_name};")
            shader_definitions.append(f"Shader {var_name} = {{0}};")
            vertex_arg = f'"{vertex_path}"' if vertex_path else "NULL"
            fragment_arg = f'"{fragment_path}"' if fragment_path else "NULL"
            shader_entries.append(f'    {{ {vertex_arg}, {fragment_arg}, {{0}} }}')

# Calculate number of shaders
num_shaders = len(shader_entries)

# Complete header content
header_content += f"#define NUM_SHADERS {num_shaders}\n\n"
header_content += "typedef struct {\n    const char *vertexPath;\n    const char *fragmentPath;\n    Shader shader;\n} ShaderInfo;\n\n"
header_content += "\n".join(shader_vars)
header_content += "\n\nbool LoadShaders(void);\nvoid UnloadShaders(void);\n"

# Create source content
source_content = f'#include "../include/shaders.h"\n#include <raylib.h>\n\n'
source_content += "\n".join(shader_definitions)
source_content += f"\n\nShaderInfo shaders[NUM_SHADERS] = {{\n"
source_content += ",\n".join(shader_entries)
source_content += "\n};\n\n"

source_content += "bool LoadShaders(void) {\n"
source_content += "    if (!IsWindowReady()) {\n"
source_content += '        TraceLog(LOG_ERROR, "Cannot load shaders: Window not initialized");\n'
source_content += "        return false;\n"
source_content += "    }\n\n"
source_content += "    bool allLoaded = true;\n\n"

# Shader loading loop
source_content += "    for (int i = 0; i < NUM_SHADERS; i++) {\n"
source_content += "        shaders[i].shader = LoadShader(shaders[i].vertexPath, shaders[i].fragmentPath);\n"
source_content += "        if (shaders[i].shader.id == 0) {\n"
source_content += '            TraceLog(LOG_ERROR, "Failed to load shader: %s, %s", \n'
source_content += '                     shaders[i].vertexPath ? shaders[i].vertexPath : "NULL", \n'
source_content += '                     shaders[i].fragmentPath ? shaders[i].fragmentPath : "NULL");\n'
source_content += "            allLoaded = false;\n"
source_content += "        }\n"
source_content += "    }\n\n"

# Assign individual shader variables properly
if shader_vars:
    source_content += "    // Assign individual shader variables\n"
    for i, var in enumerate([v.split()[1].rstrip(';') for v in shader_vars]):
        source_content += f"    {var} = shaders[{i}].shader;\n"

source_content += "\n    return allLoaded;\n"
source_content += "}\n\n"

source_content += "void UnloadShaders(void) {\n"
source_content += "    for (int i = 0; i < NUM_SHADERS; i++) {\n"
source_content += "        if (shaders[i].shader.id != 0) {\n"
source_content += "            UnloadShader(shaders[i].shader);\n"
source_content += "            shaders[i].shader = (Shader){0};\n"
source_content += "        }\n"
source_content += "    }\n\n"

# Reset individual shader variables
if shader_vars:
    source_content += "    // Reset individual shader variables\n"
    for i, var in enumerate([v.split()[1].rstrip(';') for v in shader_vars]):
        source_content += f"    {var} = (Shader){{0}};\n"

source_content += "}\n"

# Create directories if they don't exist
os.makedirs(os.path.dirname(header_file), exist_ok=True)
os.makedirs(os.path.dirname(source_file), exist_ok=True)

# Write to files
with open(header_file, "w") as f:
    f.write(header_content)
with open(source_file, "w") as f:
    f.write(source_content)

print(f"Generated {header_file} and {source_file} with {num_shaders} shaders.")
