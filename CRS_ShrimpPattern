// 1D Random
float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

// 2D Noise based on 1D Random
float noise(vec2 pos) {
    vec2 floorPos = floor(pos);
    vec2 s = smoothstep(0.0, 1.0, fract(pos));
    
    return mix(
        mix(rand(floorPos), rand(floorPos + vec2(1.0, 0.0)), s.x),
        mix(rand(floorPos + vec2(0.0, 1.0)), rand(floorPos + vec2(1.0, 1.0)), s.x),
        s.y);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord ) 
{
    vec2 uv = fragCoord / iResolution.y;
    
    // Decrease the scale to make the pattern bigger
    float scale = 5.0;

	//float xuxu_delta = 2.0;   //얼룩없음
    //float xuxu_delta = 4 ~ 5; //꽉찬 얼룩
    //float xuxu_delta = 0.0; //절반?
    //float xuxu_delta = 1.2; //얼룩없음
    float xuxu_delta = 3.95;
	
    // Combine multiple noise functions to create a more complex pattern
    float n1 = noise(uv * scale * vec2(1.0, 2.0) + vec2(0.0, xuxu_delta));
    float n2 = noise(uv * scale * 2.0 * vec2(1.0, 2.0) + vec2(0.0, xuxu_delta));

    // Weighted sum of the noise functions
    float n = n1 * 0.5 + n2 * 0.5;
    
    // Create band-like pattern with variable widths
    float threshold = 0.5 + 0.5 * sin(xuxu_delta);
    vec3 color = vec3(step(threshold, fract(n)));
    
    // Apply dark red color
    color *= vec3(0.5, 0.0, 0.0);
    
    fragColor = vec4(color,1.0);
}
