#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"

extern bool disable_hierarchy;

Render_World::Render_World()
    :background_shader(0),ambient_intensity(0),enable_shadows(true),
    recursion_depth_limit(3)
{}

Render_World::~Render_World()
{
    delete background_shader;
    for(size_t i=0;i<objects.size();i++) delete objects[i];
    for(size_t i=0;i<lights.size();i++) delete lights[i];
}

// Find and return the Hit structure for the closest intersection.  Be careful
// to ensure that hit.dist>=small_t.
Hit Render_World::Closest_Intersection(const Ray& ray)
{
    Hit closest_hit = {NULL, 0, 0};
    double dis = std::numeric_limits<double>::max();
    
    std::vector<int> candidates;
    hierarchy.Intersection_Candidates(ray, candidates);
    //printf("Candidates.size()=%zu\n", candidates.size());
    for(int i = 0; i < (int)candidates.size(); i++){
        Hit tmp_hit = hierarchy.entries[candidates[i]].obj->Intersection(ray, hierarchy.entries[i].part);
    	if(tmp_hit.object){
    		if(tmp_hit.dist < dis && tmp_hit.dist > small_t){
    			closest_hit = tmp_hit;
    			dis = tmp_hit.dist;
    		}
    	}
    }
    //printf("Intersecton done\n");
    return closest_hit;
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
    Ray ray(camera.position,(camera.World_Position(pixel_index)-camera.position).normalized());
    vec3 color=Cast_Ray(ray,1);
    camera.Set_Pixel(pixel_index,Pixel_Color(color));
}

void Render_World::Render()
{
    if(!disable_hierarchy)
        Initialize_Hierarchy();

    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++) {
            //std::cout << "i=" << i << ", j=" << j << ": ";
            Render_Pixel(ivec2(i,j));
        }
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray,int recursion_depth)
{
    vec3 color;
    Hit hit = Closest_Intersection(ray);
    vec3 interSectPoint = ray.Point(hit.dist);
    if(hit.object&&recursion_depth <=recursion_depth_limit){
	    color = hit.object->material_shader->Shade_Surface(ray, interSectPoint, hit.object->Normal(interSectPoint, hit.part), recursion_depth);
    }else{
    	color = background_shader->Shade_Surface(ray, color, color, 0);
    }
    //std::cout<<color<<std::endl;
    return color;
}

void Render_World::Initialize_Hierarchy()
{
    // Fill in hierarchy.entries; there should be one entry for
    // each part of each object.
    int n = objects.size();
    for(int i = 0; i < n; i++) {
        for (int j = 0; j < objects[i]->number_parts; j++) {
            hierarchy.entries.push_back({objects[i], j, objects[i]->Bounding_Box(j)});
        }
    }
    hierarchy.Reorder_Entries();
    hierarchy.Build_Tree();
}
