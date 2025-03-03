#version 330

// Input from vertex shader
in vec2 fragTexCoord;

// Uniforms
uniform sampler2D texture0; // The texture (bound by Raylib to the quad)
uniform float uTime; // Time value for animation
uniform int uDamage; // Damage state (0 or 1)

// Output color for each pixel
out vec4 finalColor;

void main() {
    // Sample the texture at the current texture coordinates
    vec4 texColor = texture(texture0, fragTexCoord);

    // Apply damage effect if uDamage is 1
    if (uDamage == 1) {
        // Create a flashing effect using sine wave for red/white pulsing
        float flash = sin(uTime * 20.0) * 0.5 + 0.5; // Oscillates between 0 and 1, faster than ripples
        vec3 damageColor = mix(texColor.rgb, vec3(1.0, 0.0, 0.0), flash * 0.7); // Mix between texture and red
        damageColor = mix(damageColor, vec3(1.0), flash * 0.3); // Add white flash on top
        finalColor = vec4(damageColor, texColor.a);
    } else {
        finalColor = vec4(texColor.rgb, texColor.a); // No damage, show normal texture
    }
}
