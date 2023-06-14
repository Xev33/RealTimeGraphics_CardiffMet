# README #

This repo contains a series of 3D rendering and lighting exercises completed for the Cardiff Metropolitan University's second year module: Real-Time Computer Graphics.

### Introduction ###
I was required to produce a 3D scene using basic openGL techniques.
Within the coding part I had to develop and produce my rendering aspects including: texturing, interaction and lighting.
All 3D models have been given.

### List of Features ###
[active]:https://img.shields.io/badge/-Active-success
[depreciated]:https://img.shields.io/badge/-Depreciated-inactive
[updating]:https://img.shields.io/badge/-Updating-purple
[passing]:https://img.shields.io/badge/-Passing-success
[outdated]:https://img.shields.io/badge/-Outdated-blue
[dev]:https://img.shields.io/badge/-Unreleased-important

[version-1.0.0]:https://img.shields.io/badge/Ver.-1.0.0-ff69b4
[version-1.0.1]:https://img.shields.io/badge/Ver.-1.0.1-ff69b4
[version-1.0.2]:https://img.shields.io/badge/Ver.-1.0.2-ff69b4
[version-1.0.3]:https://img.shields.io/badge/Ver.-1.0.3-ff69b4
[version-1.0.4]:https://img.shields.io/badge/Ver.-1.0.4-ff69b4
[version-1.0.5]:https://img.shields.io/badge/Ver.-1.0.5-ff69b4
[version-dev]:https://img.shields.io/badge/Ver.-dev-important

|**Category**|**Feature**|**Description**|**Version**|
|:------:|:------:|:-------------:|:---------:|
| Interaction |First person character controller|A simple first person has been implemented. WASD keyboard can be use in order to move and rotate the camera.|![dev][version-1.0.0]|
| Mesh rendering |3D model rendering|Print a 3D mesh onto the screen.|![dev][version-1.0.0]|
| Mesh rendering |Texture mapping|Apply a texture to a 3D mesh|![dev][version-1.0.0]|
| Lighting |[Directional light](https://docs.unrealengine.com/4.27/en-US/BuildingWorlds/LightingAndShadows/LightTypes/Directional/#:~:text=The%20Directional%20Light%20simulates%20light,ideal%20choice%20for%20simulating%20sunlight.)|The Directional Light simulates light that is being emitted from a source that is infinitely far away. This means that all shadows cast by this light will be parallel, making this the ideal choice for simulating sunlight.|![dev][version-1.0.0]|
| Lighting |[Point light](https://docs.unrealengine.com/4.27/en-US/BuildingWorlds/LightingAndShadows/LightTypes/Point/)|Point Lights work much like a real world light bulb, emitting light in all directions from the light bulb's tungsten filament.|![dev][version-1.0.0]|

# Interaction #

### First person character controller key binding ###

|**Key**|**Tag**|
|:------:|:------:|
| ***UpArrow/W*** | Move forward |
| ***DownArrow/S*** | Move backward |
| ***LeftArrow/A*** | Rotate left |
| ***RightArrow/D*** | Rotate right |

Arrows and WASD key can be use in order to move and rotate the camera. To do so, I had to implement a GetForwardVector function in order to give the correct rotation to the entire world.

Here is a gif showing what it produces.

![FPController](https://github.com/Xev33/RealTimeGraphics_CardiffMet/assets/106018428/d3e67c16-e8b7-4b7b-af31-948339d0f9ce)

# Mesh rendering #

### 3D model rendering ###

To render an object, I used both VBOs and VAOs. Both are setup and bind in a function named “init”. Each objects is rendered by using given properties in an sdf file named scene.sdf. This file gives: A 3D position, a rotation, a scale and allows to select a light and its type for every objects.

Here is an example of a spinning arrow.

![MeshRendering](https://github.com/Xev33/RealTimeGraphics_CardiffMet/assets/106018428/65f1fe24-8f85-41bf-962a-73a3320a8ac4)

### Texture mapping ###

While the object goes through its VBO, it is linked to its textures. Most of my objects have a texture.

The following image contains an example of a tower mesh using a rock texture.

![TextureMapping](https://github.com/Xev33/RealTimeGraphics_CardiffMet/assets/106018428/c7f9256a-823a-4714-8f4d-a15ffd920fda)

# Lighting #

Two types of light have been created through two shaders.
Both lights have a diffuse attribute in order to see the unlighted sides of the meshes using them. Just like the meshes, each lights and their attributes can be find and modify in the scene.sdf file. Each lights are define with: an id number, a position, an attenuation, a direction and a color.

### Directional light ###

A directional light: its brightness is calculated with a direction and an attenuation value.

Here is a picture of the entire scene visible thank to the directional light.
![DirectionalLight](https://github.com/Xev33/RealTimeGraphics_CardiffMet/assets/106018428/1acec8d8-6987-416a-81b8-03eb9c4609a6)

The diffuse attribute allows us to see the unlighted part of any mesh
![DiffuseLight](https://github.com/Xev33/RealTimeGraphics_CardiffMet/assets/106018428/850dfeda-db12-47a9-8664-b67744dc655e)

### Point light ###

A point light: Its brightness is calculated with a position and an attenuation value.

The following gif shows clearly see where the point light is and its effect on the environment.
![PointLight](https://github.com/Xev33/RealTimeGraphics_CardiffMet/assets/106018428/b3674989-6cda-4c1f-8243-cfc78fa909a1)

<a name="acknowledgments"></a>
## Acknowledgments 
I would like to thank Andrew Baker for the time he took to explain all these concepts to me throughout my time at Cardiff Metropolitan University. 
