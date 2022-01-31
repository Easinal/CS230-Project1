#include "light.h"
#include "object.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color;
    vec3 ambient, diffuse, specular;
    ambient = world.ambient_color * world.ambient_intensity * color_ambient;
    
    for(int i=0;i<(int)world.lights.size();++i){
        Light* l = world.lights[i];
        vec3 vec_to_intersect = l->position-intersection_point;
        vec3 norm_intersect = vec_to_intersect.normalized();
        bool flag = true;
        if(world.enable_shadows){
            Ray shadow(intersection_point, norm_intersect);
            Hit hit = world.Closest_Intersection(shadow);
            if(hit.object!=NULL && hit.dist < vec_to_intersect.magnitude()){ //under shadow
                flag = false;
            }
        }
        if(flag){
                vec3 I = l->Emitted_Light(vec_to_intersect);
                diffuse += color_diffuse*I*std::max(0.0, dot(normal, norm_intersect));

                vec3 reflect = ((2*dot(normal, norm_intersect)*normal)-norm_intersect).normalized();
                double NH = std::pow(std::max(0.0, dot(reflect, -ray.direction.normalized())),specular_power);
                specular += color_specular * I * NH;

        }
    }
    color = ambient + diffuse + specular;
    return color;
}
