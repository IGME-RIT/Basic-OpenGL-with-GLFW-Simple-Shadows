Documentation & Tutorial Author: Sanketh Bhat, 2019

This tutorial was designed for Visual Studio 2017 / 2019
If the solution does not compile, retarget the solution
to a different version of the Windows SDK. If you do not
have any version of the Windows SDK, it can be installed
from the Visual Studio Installer Tool

Simple Dynamic Shadows Tutorial
Reference: https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
Prerequesites: Basic Lighting, Skyboxes, Anisotropic Filtering, MSAA, Mipmapping, Texturing, Rendering to Textures 
Note: This is directional shadows ONLY, no point or spotlight shadows

New Concepts: Depth buffer
Steps: 
>For dynamic shadow mapping, we need to render the scene two times. 
>The first time will be through the perspective of the light in our scene
>The second will be the actual scene with shadows added in the frag(pixel) shader.
>We will also need depth data of the scene from the first pass for our calculations

>To do that we enable depth testing (glEnable(GL_DEPTH_TEST) & create a depth map 2D texture, giving it a predefined shadow height & width(1024x1024 here)
>Anisotropic fitlering is enabled for the shadow texture if need be.
>We also create a frame buffer object and bind it. We then bind our depth buffer to the aforementioned frame buffer (glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0))
>Then the draw and read buffers for our bound framebuffer are set to none(glDrawBuffer(GL_NONE) & glReadBuffer(GL_NONE)), meaning it will render in grayscale with no color and our framebuffer is unbinded.
>Two sets of shader program/material combos are needed. One for our first depth pass, and one for our second, final pass.
>We also enable MSAA using glEnable(GL_MULTISAMPLE) before the game loop.
>In the loop after time calculations we update our light and camera values.

>Then we set our viewport to our shadow resoultion, clear our depth buffer and bind our framebuffer with the attached depth map.
>Our depth pass material is then set to our model. The depth vertex shader takes vertex position as an attribute, and a light space mat4 and world matrix mat4 as uniforms.
>It then multiplies them all together as the built in gl_Position value.The frag shader is empty as we do not need it to do anything.
> After uploading the light matrix and world position, and after rendering the first pass we unbind our framebuffer.

>For the second render pass we set the viewport back to default values & clear our depth and color buffers.
>We then set our model material to the shadow pass material. The vertex shader takes vertex position, uv and normal as attributes, and camera matrix, world matrix and light space matrix as uniforms.
>We calculate vertex position in world space, vertex normal in world space, vertex position in light space and send them all to the frag shader including the uv value.
>gl_Position is set to vertex position in camera space as usual.
>In the vertex shader we take a diffuse texture and depth texture as sampler2D uniforms, and light position as a vec3 uniform
>We use a typical Blinn- Phong lighting model but without the specular component in this example.
>The main difference in shadow mapping is an additional (1-shadow) in the final color calculation.
>If the current depth is greater than the closest depth, we have a shadow.
>In the shader we also apply a depth bias and perform percentage-closer filtering to fix jagged edges






