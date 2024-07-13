Chris Sheard
CSC 305 Assignment 3
Completed:
- I created classes for Spheres, Rays, Lights, Vec3s, and Vec4s.
- The Sphere class contains the values for each sphere and is capable of performing operations required to inverse transform and find the normal of a point on the sphere. The inverse and normal calculations avoid using any matrix operations for optimal performance.
- The Ray class contains the ray origin point and direction vector. The class includes a function to find the point resulting from the input scalar.
- The Lights class contains the values for the lights.
- The Vec3 class contains the functionality for creating 3x1 vectors and perform various operations with them.
- The Vec4 class contains the functionality for creating 4x1 vectors and perform various operations with them.
- For the main RayTracer file, I parse through the input file and assign the provided values to different objects and structs to organize the input values.
- For each ray cast into the scene, the ray_colour() function transforms the ray into the sphere's coordinate system by inversing the sphere's scale and translate, and then uses the quadratic formula on the canonical sphere to determine if the ray intersects with any of the spheres.
	- If the ray is being cast from the camera(origin), the intersection evaluation checks if the distance of the intersection is before the image plane, and ignores the intersection if so.
	- If no intersection is found, the function returns the background colour, or black if the ray is a reflection.
	- The function then recursively finds the colour of reflecting ray, up to three reflections. Then the returned colour intensity is multiplied by the hit sphere's KR value.
	- Then the function finds the ambient light colour using I_a * I_sphere * K_a, sphere. The function then returns the sum of the ambient and reflection intensities with the light_colour() function result.
- The light_colour() function iterates through each light checking for intersections with each sphere. This is done by first finding the light's direction vector and distance from the reflection point. Then each sphere is checked for an intersection using the quadratic formula, with any intersection distance checked against the light distance.
	- If the intersection is closer than the light, the reflection point is considered in shadow. If in shadow, the light is considered to not impact the reflection point's diffuse and specular colour intensities.
	- If not in shadow, the reflection point's diffuse and specular intensities are evaluated using: diffuse = KD * I_sphere * I_light * max(0.0,(L dot N)), and specular = KS * max(0.0,(R dot V))^n * I_light.
		- V = viewing vector which is the negative ray direction vector.
		- R = light reflection vector on ray reflection point.
		- L = light direction vector.
		- N = normal vector of ray reflection point using sphere tranforms.
	- The resulting diffuse and specular values are summed for each light, and then returned to ray_colout().
- The final colour returned to the camera from each ray is placed into a vector<colour>. Each colour value is then bound within [0.0, 1.0] and scaled by 255 before getting writen into the output PPM file in P3 format.

To Run:
- Open solution file in /Assignment3/RayTracer/. Ensure build is in release configuration. Build project. Navigate to /Assignment3/RayTracer/x64/Release. Run executable as: RayTracer.exe <input_file>
- If build does not compile, ensure platform toolset in Deubug->Debug Properties->General->Platform Toolset is set to "Visual Studio 2019 (v142)"