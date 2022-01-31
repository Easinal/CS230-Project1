#include "reflective_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Reflective_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color;
    //TODO; // determine the color
	vec3 view_ray = -ray.direction.normalized();
	vec3 reflection_vector = (2 * dot(view_ray, normal) * normal) - view_ray;

	Ray reflected_ray;
	reflected_ray.direction = reflection_vector.normalized();
	reflected_ray.endpoint = intersection_point;

	vec3 reflected_color = world.Cast_Ray(reflected_ray, recursion_depth + 1);

	vec3 surface_color =
			shader->Shade_Surface(ray, intersection_point, normal, recursion_depth);

	color = reflected_color * reflectivity + surface_color * (1 - reflectivity);
    return color;
}
