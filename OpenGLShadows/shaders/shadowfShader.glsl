

#version 430 core

in vec3 fragPosition;
in vec2 uv;
in vec3 normal;
in vec4 fragLSPosition;

//diffuse texture
uniform sampler2D tex;
//Depth map from first render pass
uniform sampler2D shadowMap;

uniform vec3 lightPos;

out vec4 fragColor;



void main()
{       
    //Get pixel color
    vec3 color = texture(tex, uv).rgb;
    vec3 norm = normalize(normal);

    vec3 lightColor = vec3(0.3);
    vec3 ambient = lightColor * color;

    //Vector from pixel position(in world space) to light position
    vec3 lightDir = normalize(lightPos - fragPosition);
    vec3 diffuse = max(dot(lightDir, norm), 0.0) * lightColor;

    //Shadow Calculation
    /////
    //Convert light space frag position to normalized coordinates
    vec3 projPoint = fragLSPosition.xyz / fragLSPosition.w;
    projPoint = projPoint *0.5 + 0.5;

    //Using above position as a UV, we get closest depth from the light's perspective.
    float closestDepth = texture(shadowMap, projPoint.xy).r;
    float currentDepth = projPoint.z;

    //Apply bias to prevent shadow acne caused due to multiple frags sampling the same 
    //depth value when viewed at an angle
    //Bias offsets depth values based on surface normal and light direction.
    float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005);


    float shadow=0.0;
    //Percentage-closer filtering to fix jagged edges of lower shadow res
    //This is done by sampling surrounding texels of depth map and return average.
    //here we sample surrounding texels with a radius of 1 and add average the total shadow value.
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projPoint.xy + vec2(x, y) * texelSize).r; 
            //Actual shadow check, if the current depth + bias value is greater than the pcfdepth, we have a shadow.
            shadow += (currentDepth - bias) > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    /////

    vec3 litColor = (ambient+(1.0-shadow) * diffuse) * color;
    fragColor = vec4(litColor,1.0);



}  

