# CS480_Term_Project

<h1>Introduction</h1>
In this assignment you will be implementing a solar system exploration game. If you have played SporeTM, this might be familiar, but we will focus on implementing a basic version of the space stage of the game. The goal is to be able to fly a spaceship through space in the solar system and get to the low orbit of each planet, moon, or asteroid. We will not implement the graphics for flying down the atmosphere of each celestial body - just getting to the orbit is sufficient.<br \><br \>
In order to accomplish this you will need to leverage all aspects of what you have learned in the CS480/680 class, including user interaction with mouse and keyboard, MVP transformations, mesh generation and custom model loading, texturing, and several lighting and shading techniques. You will need to be able to establish and program your shaders, work with the graphics API CPU-side and GPU-side code, object oriented programming, and basic data structures.<br \><br \>
You are free to use some of the basis code (for example the backbone code from you PA1-5) or build your project from scratch with appropriate class structures you feel is appropriate. Please avoid using any extra libraries, you should be able to use the same project template and libraries that we have been using in the class for all of the aspects of this project. However, if you use other libraries in your code please make sure you sufficiently document their installation and use in your report.<br \>

<h1>Gameplay Interactions</h1>
How to switch between game modes:<br \>
[TODO]<br \>
How to interact with the spaceship:<br \>
[TODO]<br \>
spaceship movements:<br \>
[TODO]<br \>
camera movements:<br \>
[TODO]<br \>
Any other interactive elements included in your project:<br \>
[TODO]<br \>

<h1>Models and Textures</h1>
You will need to use both procedural models (e.g. spheres) and custom build models (e.g. asteroids and spacecrafts). For each model you will have to have the following items:

<h2>Materials</h2>
  Material Ambient Color<br \>
  Diffuse Texture<br \>
  Normal Map Texture<br \>
  Specular Texture and/or Color<br \>
  Shininess<br \>
<h2>Geometry</h2>
  Vertex Positions<br \>
  Vertex Normal Vectors<br \>
  Texture Coordinate<br \>
  The following folder contains some textures for the sun and planet as well as spaceship model(s). You will need to either create, download, or program other textures as needed (e.g., for certain asteroids).<br \>

<h1>Planetary Textures</h1>
A canvas folder contains textures for some of the celestial bodies you will need.

<h1>Starship Model</h1>
A canvas folder contains an example model/texture for a starship you may use. However, this model does not have a normal map. You may need to create one if you choose to use this model. However, you are more than welcome to pick or create your own models. The camera-starship should have a hierarchical relationship to implement a Third-Person feel.

<h1>Skybox</h1>
You will need to implement a skybox (or a sky sphere) to simulate the space. An example of some skybox textures/cubemaps is available on canvas. You are more than welcome to find other examples that best suits your own design.

<h1>Interactions and Gameplay</h1>
Your game will have two modes. The Exploration mode and the Planetary Observation mode. You need to implement appropriate mouse and keyboard interactions for each mode. You should have a key mapped to toggle between these modes. Note that you will need to implement a caching mechanism to maintain the appropriate original location of your camera in each mode so that you can switch back and forth.

<h1>The Exploration Mode</h1>
In this mode, you will be in a third person view, observing your starship from above at an angle (Figure 2). You will need to implement roll/pitch/yaw for moving your starship using appropriate keyboard and mouse bindings. You will also need to bind keyboard interactions for speeding up and slowing down your ship. It would be advisable to establish a braking mechanism to stop the ship to a halt.

<h1>The Planetary Observation Mode</h1>
In this mode, you will be in a first person view (the camera to be in front of the ship) and observing the planet. You will need to implement appropriate keyboard and mouse bindings to be able to have the camera look-at various locations on the planet (it should feel like moving your head around). You will also need to implement a mechanism to establish additional lighting, zoom in/out mechanisms, and general panning and rotating of the camera around the planed. You will need to also implement a reset mechanism to move the camera to its original first-person view. See Figure 3 for how an example of this mode should look like. The goal here is to be able to see the planet from its orbit and observe it from afar.	
Planetary Observation Mode.

<h1>The Solar system</h1>
Implement your solar system, with the sun at the center and rotating, each planet orbiting around the sun with its own correct tile and rotational axis, and each moon orbiting its appropriate planet with its own correct orbital and rotational angles. Make sure to have each celestial body use at least one diffuse, one normal map texture, with appropriate specular texture/color and shininess.<br \>
Implement the inner and outer asteroid belts. You will need to use instancing (chapter 2 of the textbook) for this. Basically instead of creating hundreds of meshes you will create a few and instance them for each belt.<br \>
Implement comets. At least one comet, e.g. Halleys comet must be implemented.<br \>
Place the appropriate lighting for the ambient light of the galaxy for objects to be visible enough in long range. Make sure you also establish the sun's light such that the correct side of the planets are lighting up.<br \>
In the planetary observation game mode you will also need additional lighting to be able to observe various parts of the planets even if you are on the night-side of the planet. Make the light colors to be appropriate (day light to be warm color and night light a cooler, dimmer light).<br \>

<h1>Extra credit</h1>
Implement emissive lighting to add to the back-burner of your starship to change from a darker to a brighter color as the starship speeds up.
